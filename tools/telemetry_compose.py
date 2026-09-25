from pathlib import Path
import importlib
import subprocess

Import("env")


# =============================================================================
# Configuration
# =============================================================================

if env.IsIntegrationDump():
    Return()


PROJECT_DIR = Path(
    env.subst("$PROJECT_DIR")
).resolve()

BUILD_DIR = Path(
    env.subst("$BUILD_DIR")
).resolve()

TELEMETRY_YAML = (
    PROJECT_DIR /
    "telemetry.yaml"
)

GENERATED_DIR = (
    BUILD_DIR /
    "generated" /
    "telemetry"
)

GENERATED_BUILD_DIR = (
    GENERATED_DIR /
    "build"
)


# =============================================================================
# Supported Telemetry capabilities
# =============================================================================
#
# The composer knows capabilities, not application domains.
#
# MQTT is currently the only transport/source type translated into runtime
# source metadata. Observation types themselves remain opaque strings supplied
# by telemetry.yaml.
#

SUPPORTED_SOURCE_TYPES = {
    "mqtt",
}


# =============================================================================
# Logging / errors
# =============================================================================

def log(message):
    print(
        f"[telemetry_compose] {message}"
    )


def fail(message):
    raise RuntimeError(
        f"[telemetry_compose] ERROR: {message}"
    )


# =============================================================================
# Python dependencies
# =============================================================================

def ensure_pyyaml():
    try:
        importlib.import_module(
            "yaml"
        )
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
            "Unable to install PyYAML."
        )


# =============================================================================
# YAML loading
# =============================================================================

def load_document():
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
            f"Invalid YAML: {exc}"
        )

    if document is None:
        fail(
            "telemetry.yaml is empty"
        )

    if not isinstance(
        document,
        dict
    ):
        fail(
            "telemetry.yaml root must be a mapping"
        )

    return document


# =============================================================================
# General helpers
# =============================================================================

def humanise(value):
    return (
        value
        .replace("_", " ")
        .strip()
        .title()
    )


def cpp_escape(value):
    return (
        str(value)
        .replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", "\\n")
        .replace("\r", "\\r")
    )


def cpp_float(value):
    """
    Render a Python numeric value as a C++ floating-point literal.
    """
    rendered = f"{float(value):.9g}"

    if "." not in rendered and "e" not in rendered.lower():
        rendered += ".0"

    return rendered + "f"


# =============================================================================
# Observation validation
# =============================================================================

def validate_display(
    alias,
    definition
):
    display = definition.get(
        "display"
    )

    if display is None:
        return []

    if not isinstance(
        display,
        dict
    ):
        fail(
            f"Observation '{alias}' "
            "display must be a mapping"
        )

    scales = display.get(
        "scales",
        []
    )

    if not isinstance(
        scales,
        list
    ):
        fail(
            f"Observation '{alias}' "
            "display.scales must be a list"
        )

    if not scales:
        return []

    previous_threshold = None
    normalised = []

    for index, scale in enumerate(
        scales
    ):

        if not isinstance(
            scale,
            dict
        ):
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "must be a mapping"
            )

        threshold = scale.get(
            "threshold"
        )

        divisor = scale.get(
            "divisor"
        )

        precision = scale.get(
            "precision"
        )

        unit = scale.get(
            "unit"
        )

        if not isinstance(
            threshold,
            (int, float)
        ):
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "requires numeric threshold"
            )

        if threshold < 0:
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "threshold cannot be negative"
            )

        if not isinstance(
            divisor,
            (int, float)
        ) or divisor <= 0:
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "requires positive divisor"
            )

        if not isinstance(
            precision,
            int
        ) or precision < 0:
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "requires non-negative integer precision"
            )

        if (
            not isinstance(unit, str)
            or not unit.strip()
        ):
            fail(
                f"Observation '{alias}' "
                f"display scale {index} "
                "requires non-empty unit"
            )

        if (
            previous_threshold is not None
            and threshold <= previous_threshold
        ):
            fail(
                f"Observation '{alias}' "
                "display thresholds must be "
                "strictly increasing"
            )

        previous_threshold = threshold

        normalised.append(
            {
                "threshold": float(threshold),
                "divisor": float(divisor),
                "precision": precision,
                "unit": unit,
            }
        )

    if normalised[0]["threshold"] != 0:
        fail(
            f"Observation '{alias}' "
            "first display scale must have "
            "threshold 0"
        )

    return normalised


def validate_observations(
    observations
):
    if not isinstance(
        observations,
        dict
    ):
        fail(
            "'observations' must be a mapping"
        )

    normalised = []

    aliases = set()
    semantic_keys = set()
    source_values = set()

    for alias, definition in observations.items():

        # ---------------------------------------------------------------------
        # Alias
        # ---------------------------------------------------------------------

        if (
            not isinstance(alias, str)
            or not alias.strip()
        ):
            fail(
                "Observation aliases must "
                "be non-empty strings"
            )

        if alias in aliases:
            fail(
                f"Duplicate observation alias: "
                f"{alias}"
            )

        aliases.add(alias)

        # ---------------------------------------------------------------------
        # Definition
        # ---------------------------------------------------------------------

        if not isinstance(
            definition,
            dict
        ):
            fail(
                f"Observation '{alias}' "
                "must be a mapping"
            )

        # ---------------------------------------------------------------------
        # Semantic identity
        # ---------------------------------------------------------------------

        key = definition.get(
            "key"
        )

        if (
            not isinstance(key, str)
            or not key.strip()
        ):
            fail(
                f"Observation '{alias}' "
                "must define a non-empty key"
            )

        if key in semantic_keys:
            fail(
                f"Duplicate semantic observation key: "
                f"{key}"
            )

        semantic_keys.add(key)

        # ---------------------------------------------------------------------
        # Meaning
        # ---------------------------------------------------------------------

        observation_type = definition.get(
            "type"
        )

        if (
            not isinstance(
                observation_type,
                str
            )
            or not observation_type.strip()
        ):
            fail(
                f"Observation '{alias}' "
                "must define a non-empty type"
            )

        unit = definition.get(
            "unit"
        )

        if (
            not isinstance(
                unit,
                str
            )
            or not unit.strip()
        ):
            fail(
                f"Observation '{alias}' "
                "must define a non-empty unit"
            )

        # ---------------------------------------------------------------------
        # Presentation
        # ---------------------------------------------------------------------

        label = definition.get(
            "label"
        )

        if label is None:
            label = humanise(alias)

        if (
            not isinstance(label, str)
            or not label.strip()
        ):
            fail(
                f"Observation '{alias}' "
                "has an invalid label"
            )

        display_scales = validate_display(
            alias,
            definition
        )

        # ---------------------------------------------------------------------
        # Source
        # ---------------------------------------------------------------------

        source = definition.get(
            "source"
        )

        if not isinstance(
            source,
            dict
        ):
            fail(
                f"Observation '{alias}' "
                "must define a source"
            )

        source_type = source.get(
            "type"
        )

        if source_type not in \
            SUPPORTED_SOURCE_TYPES:

            supported = ", ".join(
                sorted(
                    SUPPORTED_SOURCE_TYPES
                )
            )

            fail(
                f"Observation '{alias}' "
                f"has unsupported source type "
                f"'{source_type}'. "
                f"Supported sources: {supported}"
            )

        source_topic = source.get(
            "topic"
        )

        if (
            not isinstance(
                source_topic,
                str
            )
            or not source_topic.strip()
        ):
            fail(
                f"Observation '{alias}' "
                "MQTT source requires a "
                "non-empty topic"
            )

        source_identity = (
            source_type,
            source_topic
        )

        if source_identity in source_values:
            fail(
                f"Duplicate source mapping: "
                f"{source_type}:{source_topic}"
            )

        source_values.add(
            source_identity
        )

        normalised.append(
            {
                "alias": alias,
                "key": key,
                "type": observation_type,
                "unit": unit,
                "label": label,
                "displayScales": display_scales,
                "sourceType": source_type,
                "sourceTopic": source_topic,
            }
        )

    if not normalised:
        fail(
            "No observations were declared"
        )

    return normalised, aliases


# =============================================================================
# Screen validation
# =============================================================================

def validate_screens(
    screens,
    observation_aliases
):
    if screens is None:
        return []

    if not isinstance(
        screens,
        dict
    ):
        fail(
            "'screens' must be a mapping"
        )

    normalised = []

    for screen_id, definition in screens.items():

        if (
            not isinstance(
                screen_id,
                str
            )
            or not screen_id.strip()
        ):
            fail(
                "Screen IDs must be "
                "non-empty strings"
            )

        if not isinstance(
            definition,
            dict
        ):
            fail(
                f"Screen '{screen_id}' "
                "must be a mapping"
            )

        title = definition.get(
            "title"
        )

        if title is None:
            title = humanise(
                screen_id
            )

        if (
            not isinstance(title, str)
            or not title.strip()
        ):
            fail(
                f"Screen '{screen_id}' "
                "has an invalid title"
            )

        layout = definition.get(
            "layout"
        )

        if not isinstance(
            layout,
            dict
        ):
            fail(
                f"Screen '{screen_id}' "
                "must define a layout"
            )

        rows = layout.get(
            "rows"
        )

        if not isinstance(
            rows,
            list
        ) or not rows:
            fail(
                f"Screen '{screen_id}' "
                "layout.rows must be a "
                "non-empty list"
            )

        normalised_rows = []
        max_columns = 0

        for row_index, row in enumerate(
            rows
        ):

            if not isinstance(
                row,
                dict
            ):
                fail(
                    f"Screen '{screen_id}' "
                    f"row {row_index} "
                    "must be a mapping"
                )

            row_title = row.get(
                "title"
            )

            if (
                not isinstance(
                    row_title,
                    str
                )
                or not row_title.strip()
            ):
                fail(
                    f"Screen '{screen_id}' "
                    f"row {row_index} "
                    "requires a title"
                )

            items = row.get(
                "items"
            )

            if not isinstance(
                items,
                list
            ) or not items:
                fail(
                    f"Screen '{screen_id}' "
                    f"row '{row_title}' "
                    "requires at least one item"
                )

            for alias in items:
                if (
                    not isinstance(alias, str)
                    or not alias.strip()
                ):
                    fail(
                        f"Screen '{screen_id}' "
                        f"row '{row_title}' "
                        "contains an invalid item"
                    )

                if alias not in \
                    observation_aliases:

                    fail(
                        f"Screen '{screen_id}' "
                        f"references unknown "
                        f"observation '{alias}'"
                    )

            max_columns = max(
                max_columns,
                len(items)
            )

            normalised_rows.append(
                {
                    "title": row_title,
                    "items": items,
                }
            )

        normalised.append(
            {
                "id": screen_id,
                "title": title,
                "columns": max_columns,
                "rows": normalised_rows,
            }
        )

    return normalised


# =============================================================================
# Generated header
# =============================================================================

def generate_composition_header():

    return """\\
#pragma once

#include <stdint.h>

#include "data/ObservationHandle.h"
#include "models/SensorPresentation.h"

namespace TelemetryComposition
{

struct ObservationDefinition
{
    const char* alias;
    const char* key;
    const char* type;
    const char* unit;
    const char* label;

    const char* sourceType;
    const char* sourceTopic;

    ObservationHandle handle;

    const SensorDisplayScale* displayScales;
    uint8_t displayScaleCount;
};


struct ScreenRow
{
    const char* title;
    const char* const* items;
    uint8_t itemCount;
};


struct ScreenDefinition
{
    const char* id;
    const char* title;

    const ScreenRow* rows;
    uint8_t rowCount;
    uint8_t columns;
};


bool registerObservations();

const ObservationDefinition* observations();
uint8_t observationCount();

const ObservationDefinition*
findObservation(
    const char* alias);

const ObservationDefinition*
findObservationBySource(
    const char* sourceType,
    const char* source);

const ScreenDefinition* screens();
uint8_t screenCount();

const ScreenDefinition*
findScreen(
    const char* id);

}
"""


# =============================================================================
# Generated implementation
# =============================================================================

def generate_composition_cpp(
    observations,
    screens
):

    lines = [
        '#include "TelemetryComposition.h"',
        "",
        '#include <cstring>',
        "",
        '#include "data/ObservationKey.h"',
        '#include "data/ObservationRegistry.h"',
        '#include "models/SensorRepository.h"',
        '#include "models/SensorTile.h"',
        "",
        "namespace",
        "{",
    ]

    # -------------------------------------------------------------------------
    # Display scales
    # -------------------------------------------------------------------------

    for index, observation in enumerate(
        observations
    ):
        scales = observation[
            "displayScales"
        ]

        if not scales:
            continue

        lines.extend(
            [
                f"    constexpr SensorDisplayScale "
                f"displayScales_{index}[] =",
                "    {",
            ]
        )

        for scale in scales:
            lines.append(
                "        {"
                f"{cpp_float(scale['threshold'])}, "
                f"{cpp_float(scale['divisor'])}, "
                f"{scale['precision']}, "
                f'\"{cpp_escape(scale["unit"])}\"'
                "},"
            )

        lines.extend(
            [
                "    };",
                "",
            ]
        )

    # -------------------------------------------------------------------------
    # Observation definitions
    # -------------------------------------------------------------------------
    #
    # IMPORTANT:
    # This is a namespace-scope generated table. Runtime registration code is
    # emitted only after the table has been completely closed.
    #

    lines.extend(
        [
            "    TelemetryComposition::"
            "ObservationDefinition observationDefinitions[] =",
            "    {",
        ]
    )

    for index, observation in enumerate(
        observations
    ):
        alias = cpp_escape(
            observation["alias"]
        )

        key = cpp_escape(
            observation["key"]
        )

        observation_type = cpp_escape(
            observation["type"]
        )

        unit = cpp_escape(
            observation["unit"]
        )

        label = cpp_escape(
            observation["label"]
        )

        source_type = cpp_escape(
            observation["sourceType"]
        )

        source_topic = cpp_escape(
            observation["sourceTopic"]
        )

        scales = observation[
            "displayScales"
        ]

        if scales:
            scale_pointer = (
                f"displayScales_{index}"
            )
            scale_count = str(
                len(scales)
            )
        else:
            scale_pointer = "nullptr"
            scale_count = "0"

        lines.append(
            "        {"
            f'"{alias}", '
            f'"{key}", '
            f'"{observation_type}", '
            f'"{unit}", '
            f'"{label}", '
            f'"{source_type}", '
            f'"{source_topic}", '
            "ObservationHandle{}, "
            f"{scale_pointer}, "
            f"{scale_count}"
            "},"
        )

    lines.extend(
        [
            "    };",
            "",
        ]
    )

    # -------------------------------------------------------------------------
    # Screen item arrays
    # -------------------------------------------------------------------------

    for screen_index, screen in enumerate(
        screens
    ):

        for row_index, row in enumerate(
            screen["rows"]
        ):

            lines.extend(
                [
                    f"    constexpr const char* const "
                    f"screen_{screen_index}_row_"
                    f"{row_index}_items[] =",
                    "    {",
                ]
            )

            for alias in row["items"]:
                lines.append(
                    f'        "{cpp_escape(alias)}",'
                )

            lines.extend(
                [
                    "    };",
                    "",
                ]
            )

        lines.extend(
            [
                f"    constexpr "
                f"TelemetryComposition::ScreenRow "
                f"screen_{screen_index}_rows[] =",
                "    {",
            ]
        )

        for row_index, row in enumerate(
            screen["rows"]
        ):
            title = cpp_escape(
                row["title"]
            )

            count = len(
                row["items"]
            )

            lines.append(
                "        {"
                f'"{title}", '
                f"screen_{screen_index}_row_"
                f"{row_index}_items, "
                f"{count}"
                "},"
            )

        lines.extend(
            [
                "    };",
                "",
            ]
        )

    # -------------------------------------------------------------------------
    # Screen definitions
    # -------------------------------------------------------------------------

    if screens:
        lines.extend(
            [
                "    constexpr "
                "TelemetryComposition::"
                "ScreenDefinition screenDefinitions[] =",
                "    {",
            ]
        )

        for index, screen in enumerate(
            screens
        ):
            screen_id = cpp_escape(
                screen["id"]
            )

            title = cpp_escape(
                screen["title"]
            )

            row_count = len(
                screen["rows"]
            )

            columns = screen[
                "columns"
            ]

            lines.append(
                "        {"
                f'"{screen_id}", '
                f'"{title}", '
                f"screen_{index}_rows, "
                f"{row_count}, "
                f"{columns}"
                "},"
            )

        lines.extend(
            [
                "    };",
                "",
            ]
        )

    # -------------------------------------------------------------------------
    # Namespace state
    # -------------------------------------------------------------------------

    lines.extend(
        [
            "}",
            "",
            "namespace TelemetryComposition",
            "{",
            "",
        ]
    )

    # -------------------------------------------------------------------------
    # Observation registration
    # -------------------------------------------------------------------------
    #
    # This section consumes the generated definition table. It does not emit
    # more initialiser entries into that table.
    #

    lines.extend(
        [
            "bool registerObservations()",
            "{",
            "",
            "    for (",
            "        auto& observation : observationDefinitions",
            "    )",
            "    {",
            "",
            "        const ObservationKey key{",
            "            observation.key",
            "        };",
            "",
            "        observation.handle =",
            "            ObservationRegistry::"
            "registerObservation(key);",
            "",
            "        if (!observation.handle.isValid())",
            "        {",
            "            return false;",
            "        }",
            "",
            "        SensorTile tile{};",
            "        tile.label = observation.label;",
            "        tile.unit = observation.unit;",
            "        tile.displayScales = observation.displayScales;",
            "        tile.displayScaleCount = observation.displayScaleCount;",
            "",
            "        if (!SensorRepository::registerObservation(",
            "                observation.handle,",
            "                tile))",
            "        {",
            "            return false;",
            "        }",
            "    }",
            "",
            "    return true;",
            "}",
            "",
        ]
    )

    # -------------------------------------------------------------------------
    # Observation access
    # -------------------------------------------------------------------------

    lines.extend(
        [
            "const ObservationDefinition* observations()",
            "{",
            "    return observationDefinitions;",
            "}",
            "",
            "uint8_t observationCount()",
            "{",
            "    return static_cast<uint8_t>(",
            "        sizeof(observationDefinitions) /",
            "        sizeof(observationDefinitions[0])",
            "    );",
            "}",
            "",
            "const ObservationDefinition* findObservation(",
            "    const char* alias)",
            "{",
            "    if (!alias)",
            "    {",
            "        return nullptr;",
            "    }",
            "",
            "    for (",
            "        const auto& observation : observationDefinitions",
            "    )",
            "    {",
            "        if (std::strcmp(",
            "                observation.alias,",
            "                alias) == 0)",
            "        {",
            "            return &observation;",
            "        }",
            "    }",
            "",
            "    return nullptr;",
            "}",
            "",
            "const ObservationDefinition* findObservationBySource(",
            "    const char* sourceType,",
            "    const char* source)",
            "{",
            "    if (!sourceType || !source)",
            "    {",
            "        return nullptr;",
            "    }",
            "",
            "    for (",
            "        const auto& observation : observationDefinitions",
            "    )",
            "    {",
            "        if (std::strcmp(",
            "                observation.sourceType,",
            "                sourceType) == 0 &&",
            "            std::strcmp(",
            "                observation.sourceTopic,",
            "                source) == 0)",
            "        {",
            "            return &observation;",
            "        }",
            "    }",
            "",
            "    return nullptr;",
            "}",
            "",
        ]
    )

    # -------------------------------------------------------------------------
    # Screen access
    # -------------------------------------------------------------------------

    if screens:
        lines.extend(
            [
                "const ScreenDefinition* screens()",
                "{",
                "    return screenDefinitions;",
                "}",
                "",
                "uint8_t screenCount()",
                "{",
                "    return static_cast<uint8_t>(",
                "        sizeof(screenDefinitions) /",
                "        sizeof(screenDefinitions[0])",
                "    );",
                "}",
                "",
                "const ScreenDefinition* findScreen(",
                "    const char* id)",
                "{",
                "    if (!id)",
                "    {",
                "        return nullptr;",
                "    }",
                "",
                "    for (",
                "        const auto& screen : screenDefinitions",
                "    )",
                "    {",
                "        if (std::strcmp(",
                "                screen.id,",
                "                id) == 0)",
                "        {",
                "            return &screen;",
                "        }",
                "    }",
                "",
                "    return nullptr;",
                "}",
                "",
            ]
        )

    else:
        lines.extend(
            [
                "const ScreenDefinition* screens()",
                "{",
                "    return nullptr;",
                "}",
                "",
                "uint8_t screenCount()",
                "{",
                "    return 0;",
                "}",
                "",
                "const ScreenDefinition* findScreen(",
                "    const char*)",
                "{",
                "    return nullptr;",
                "}",
                "",
            ]
        )

    lines.extend(
        [
            "}",
            "",
        ]
    )

    return "\n".join(lines)


# =============================================================================
# Remove obsolete generated files
# =============================================================================

def remove_stale_outputs():
    stale = [
        GENERATED_DIR /
        "TelemetryMqttMappings.h",

        GENERATED_DIR /
        "TelemetryMqttMappings.cpp",
    ]

    for path in stale:
        if path.exists():
            path.unlink()

            log(
                f"Removed stale generated file "
                f"{path.name}"
            )


# =============================================================================
# File writing
# =============================================================================

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


# =============================================================================
# Compose
# =============================================================================

def compose():

    document = load_document()

    observations_document = document.get(
        "observations",
        {}
    )

    screens_document = document.get(
        "screens",
        {}
    )

    observations, observation_aliases = (
        validate_observations(
            observations_document
        )
    )

    screens = validate_screens(
        screens_document,
        observation_aliases
    )

    GENERATED_DIR.mkdir(
        parents=True,
        exist_ok=True
    )

    remove_stale_outputs()

    write_file(
        GENERATED_DIR /
        "TelemetryComposition.h",
        generate_composition_header()
    )

    write_file(
        GENERATED_DIR /
        "TelemetryComposition.cpp",
        generate_composition_cpp(
            observations,
            screens
        )
    )

    log(
        f"Composition complete: "
        f"{len(observations)} observation(s), "
        f"{len(screens)} screen(s)"
    )


# =============================================================================
# Execute
# =============================================================================

compose()


# =============================================================================
# Add generated sources / headers to PlatformIO
# =============================================================================

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
