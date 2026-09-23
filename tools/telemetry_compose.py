from pathlib import Path
import importlib
import re
import subprocess

Import("env")


# -----------------------------------------------------------------------------
# Guard against non-build PlatformIO invocations
# -----------------------------------------------------------------------------

if env.IsIntegrationDump():
    Return()


# -----------------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------------

PROJECT_DIR = Path(env.subst("$PROJECT_DIR")).resolve()
BUILD_DIR = Path(env.subst("$BUILD_DIR")).resolve()

TELEMETRY_YAML = PROJECT_DIR / "telemetry.yaml"

# Generated source files are deliberately kept inside the PlatformIO build
# tree. They are derived artifacts and can therefore be regenerated at any
# time from telemetry.yaml.
GENERATED_DIR = (
    BUILD_DIR
    / "generated"
    / "telemetry"
)

# Keep generated object/build output separate from the generated source tree.
GENERATED_BUILD_DIR = (
    GENERATED_DIR
    / "build"
)


SUPPORTED_TYPES = {
    "power": {
        "cpp_type": "ENERGY_W",
        "unit": "W",
    },
    "energy": {
        "cpp_type": "ENERGY_WH",
        "unit": "Wh",
    },
}


SUPPORTED_SOURCE_TYPES = {
    "mqtt",
}


CPP_IDENTIFIER_RE = re.compile(
    r"^[A-Za-z_][A-Za-z0-9_]*$"
)


# -----------------------------------------------------------------------------
# Utility functions
# -----------------------------------------------------------------------------

def log(message):
    print(
        f"[telemetry_compose] {message}"
    )


def fail(message):
    raise RuntimeError(
        f"[telemetry_compose] ERROR: {message}"
    )


def ensure_pyyaml():
    """
    Ensure PyYAML is available to the Python environment used by PlatformIO.
    """
    try:
        importlib.import_module("yaml")
        return

    except ImportError:
        pass

    log(
        "PyYAML not found; installing into "
        "PlatformIO's Python environment"
    )

    python_exe = env.subst(
        "$PYTHONEXE"
    )

    result = subprocess.run(
        [
            python_exe,
            "-m",
            "pip",
            "install",
            "PyYAML",
        ],
        check=False,
    )

    if result.returncode != 0:
        fail(
            "Unable to install PyYAML. "
            "Run 'pip install PyYAML' in the "
            "PlatformIO Python environment."
        )


def load_yaml():
    if not TELEMETRY_YAML.exists():
        fail(
            f"Missing composition file: "
            f"{TELEMETRY_YAML}"
        )

    ensure_pyyaml()

    import yaml

    try:
        with TELEMETRY_YAML.open(
            "r",
            encoding="utf-8"
        ) as stream:
            document = yaml.safe_load(
                stream
            )

    except yaml.YAMLError as exc:
        fail(
            f"Invalid YAML in "
            f"{TELEMETRY_YAML}: {exc}"
        )

    if document is None:
        fail(
            "telemetry.yaml is empty"
        )

    if not isinstance(document, dict):
        fail(
            "telemetry.yaml root must be a mapping"
        )

    observations = document.get(
        "observations"
    )

    if observations is None:
        fail(
            "telemetry.yaml must contain "
            "an 'observations' section"
        )

    if not isinstance(observations, dict):
        fail(
            "'observations' must be a mapping"
        )

    return observations


def humanise_alias(alias):
    """
    Convert a composition alias into a reasonable
    fallback UI label.

    Example:
        current_power_production
        -> Current Power Production
    """
    return (
        alias
        .replace("_", " ")
        .strip()
        .title()
    )


def cpp_escape(value):
    """
    Escape a string for use as a C++ string literal.
    """
    return (
        str(value)
        .replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", "\\n")
        .replace("\r", "\\r")
    )


# -----------------------------------------------------------------------------
# Validation / normalisation
# -----------------------------------------------------------------------------

def validate_observations(observations):
    normalised = []

    aliases = set()
    semantic_keys = set()
    mqtt_topics = set()

    for alias, definition in observations.items():

        # ---------------------------------------------------------------------
        # Alias
        # ---------------------------------------------------------------------

        if not isinstance(alias, str):
            fail(
                "Observation aliases must be strings"
            )

        if not alias.strip():
            fail(
                "Observation aliases must not be empty"
            )

        if not CPP_IDENTIFIER_RE.match(alias):
            fail(
                f"Observation alias '{alias}' is not "
                "a valid C++ identifier"
            )

        if alias in aliases:
            fail(
                f"Duplicate observation alias: {alias}"
            )

        aliases.add(alias)

        # ---------------------------------------------------------------------
        # Observation definition
        # ---------------------------------------------------------------------

        if not isinstance(definition, dict):
            fail(
                f"Observation '{alias}' must be a mapping"
            )

        # ---------------------------------------------------------------------
        # Semantic identity
        # ---------------------------------------------------------------------

        semantic_key = definition.get(
            "key"
        )

        if (
            not isinstance(semantic_key, str)
            or not semantic_key.strip()
        ):
            fail(
                f"Observation '{alias}' must define "
                "a non-empty 'key'"
            )

        if semantic_key in semantic_keys:
            fail(
                f"Duplicate semantic observation key: "
                f"{semantic_key}"
            )

        semantic_keys.add(
            semantic_key
        )

        # ---------------------------------------------------------------------
        # Type
        # ---------------------------------------------------------------------

        observation_type = definition.get(
            "type"
        )

        if observation_type not in SUPPORTED_TYPES:
            supported = ", ".join(
                sorted(SUPPORTED_TYPES)
            )

            fail(
                f"Observation '{alias}' has unsupported "
                f"type '{observation_type}'. "
                f"Supported types: {supported}"
            )

        type_info = SUPPORTED_TYPES[
            observation_type
        ]

        # ---------------------------------------------------------------------
        # Canonical unit
        # ---------------------------------------------------------------------

        declared_unit = definition.get(
            "unit"
        )

        if declared_unit != type_info["unit"]:
            fail(
                f"Observation '{alias}' declares unit "
                f"'{declared_unit}', but type "
                f"'{observation_type}' requires canonical "
                f"unit '{type_info['unit']}'"
            )

        # ---------------------------------------------------------------------
        # Source
        # ---------------------------------------------------------------------

        source = definition.get(
            "source"
        )

        if not isinstance(source, dict):
            fail(
                f"Observation '{alias}' must define "
                "a 'source' mapping"
            )

        source_type = source.get(
            "type"
        )

        if source_type not in SUPPORTED_SOURCE_TYPES:
            supported = ", ".join(
                sorted(SUPPORTED_SOURCE_TYPES)
            )

            fail(
                f"Observation '{alias}' has unsupported "
                f"source type '{source_type}'. "
                f"Supported sources: {supported}"
            )

        mqtt_topic = source.get(
            "topic"
        )

        if (
            not isinstance(mqtt_topic, str)
            or not mqtt_topic.strip()
        ):
            fail(
                f"Observation '{alias}' MQTT source "
                "must define a non-empty 'topic'"
            )

        if mqtt_topic in mqtt_topics:
            fail(
                f"Duplicate MQTT topic: {mqtt_topic}"
            )

        mqtt_topics.add(
            mqtt_topic
        )

        # ---------------------------------------------------------------------
        # Presentation metadata
        # ---------------------------------------------------------------------

        label = definition.get(
            "label"
        )

        if label is None:
            label = humanise_alias(
                alias
            )

        if (
            not isinstance(label, str)
            or not label.strip()
        ):
            fail(
                f"Observation '{alias}' has "
                "an invalid 'label'"
            )

        # ---------------------------------------------------------------------
        # Normalised representation
        # ---------------------------------------------------------------------

        normalised.append(
            {
                "alias": alias,
                "key": semantic_key,
                "type": observation_type,
                "cpp_type": type_info["cpp_type"],
                "unit": type_info["unit"],
                "label": label,
                "source_type": source_type,
                "topic": mqtt_topic,
            }
        )

    return normalised


# -----------------------------------------------------------------------------
# Generated file helpers
# -----------------------------------------------------------------------------

def write_file(
    path,
    content
):
    path.parent.mkdir(
        parents=True,
        exist_ok=True
    )

    path.write_text(
        content,
        encoding="utf-8"
    )

    log(
        f"Generated "
        f"{path.relative_to(PROJECT_DIR)}"
    )


def generate_composition_header():
    return """\
#pragma once

namespace TelemetryComposition
{
    bool registerObservations();
}
"""


def generate_composition_cpp(
    observations
):
    lines = [
        '#include "TelemetryComposition.h"',
        "",
        '#include "data/ObservationKey.h"',
        '#include "data/ObservationRegistry.h"',
        '#include "data/ObservationHandle.h"',
        '#include "models/SensorRepository.h"',
        '#include "models/SensorTile.h"',
        "",
        "namespace TelemetryComposition",
        "{",
        "",
        "bool registerObservations()",
        "{",
    ]

    for observation in observations:
        alias = observation["alias"]
        key = cpp_escape(
            observation["key"]
        )
        label = cpp_escape(
            observation["label"]
        )
        unit = cpp_escape(
            observation["unit"]
        )
        cpp_type = observation["cpp_type"]

        lines.extend(
            [
                f'    constexpr ObservationKey key_{alias}{{"{key}"}};',
                "",
                f'    const ObservationHandle handle_{alias} =',
                "        ObservationRegistry::registerObservation(",
                f"            key_{alias});",
                "",
                f'    if (!handle_{alias}.isValid())',
                "    {",
                "        return false;",
                "    }",
                "",
                "    if (!SensorRepository::registerObservation(",
                f"            handle_{alias},",
                "            SensorTile{",
                f'                "{label}",',
                f'                "{unit}",',
                f"                SensorType::{cpp_type},",
                "                0.0f,",
                "                0.0f,",
                "                0.0f,",
                "                TREND_NONE,",
                "                false",
                "            }))",
                "    {",
                "        return false;",
                "    }",
                "",
            ]
        )

    lines.extend(
        [
            "    return true;",
            "}",
            "",
            "}",
            "",
        ]
    )

    return "\n".join(
        lines
    )


def generate_mqtt_mapping_header():
    return """\
#pragma once

#include "data/ObservationKey.h"

struct TelemetryMqttMapping
{
    const char* topic;
    ObservationKey observation;
};

extern const TelemetryMqttMapping telemetryMqttMappings[];
extern const unsigned int telemetryMqttMappingCount;
"""


def generate_mqtt_mapping_cpp(
    observations
):
    lines = [
        '#include "TelemetryMqttMappings.h"',
        "",
        "const TelemetryMqttMapping telemetryMqttMappings[] =",
        "{",
    ]

    for observation in observations:
        topic = cpp_escape(
            observation["topic"]
        )
        key = cpp_escape(
            observation["key"]
        )

        lines.append(
            f'    {{"{topic}", '
            f'ObservationKey{{"{key}"}}}},'
        )

    lines.extend(
        [
            "};",
            "",
            "const unsigned int telemetryMqttMappingCount =",
            "    sizeof(telemetryMqttMappings) /",
            "    sizeof(telemetryMqttMappings[0]);",
            "",
        ]
    )

    return "\n".join(
        lines
    )


# -----------------------------------------------------------------------------
# Compose
# -----------------------------------------------------------------------------

def compose():
    log(
        f"Reading {TELEMETRY_YAML}"
    )

    observations = load_yaml()

    normalised = validate_observations(
        observations
    )

    if not normalised:
        fail(
            "No observations were declared"
        )

    GENERATED_DIR.mkdir(
        parents=True,
        exist_ok=True
    )

    write_file(
        GENERATED_DIR / "TelemetryComposition.h",
        generate_composition_header()
    )

    write_file(
        GENERATED_DIR / "TelemetryComposition.cpp",
        generate_composition_cpp(
            normalised
        )
    )

    write_file(
        GENERATED_DIR / "TelemetryMqttMappings.h",
        generate_mqtt_mapping_header()
    )

    write_file(
        GENERATED_DIR / "TelemetryMqttMappings.cpp",
        generate_mqtt_mapping_cpp(
            normalised
        )
    )

    log(
        f"Composition complete: "
        f"{len(normalised)} observation(s)"
    )


# -----------------------------------------------------------------------------
# Execute
# -----------------------------------------------------------------------------

compose()


# -----------------------------------------------------------------------------
# Add generated sources and headers to this build
# -----------------------------------------------------------------------------

env.Append(
    CPPPATH=[
        str(PROJECT_DIR / "src"),
        str(GENERATED_DIR),
    ]
)

env.BuildSources(
    str(GENERATED_BUILD_DIR),
    str(GENERATED_DIR),
)
