# Walking the Boundary

A framework is experienced differently by every person who touches it.

An architect who considers only the code may build a technically elegant system that nobody understands, nobody enjoys using, or nobody can afford to learn from.

Telemetry therefore uses a simple discipline:

> **Before defining a boundary, walk through it from every side.**

## The User

The user does not see `ObservationHandle`, MQTT mappings or PlatformIO environments.

They see a display.

They need information that is:

- readable at a glance;
- trustworthy;
- visually coherent;
- responsive to interaction;
- understandable without knowing the implementation.

A technically correct screen can still be a poor user experience.

The user's question is often simply:

> “Can I understand what this is telling me?”

## The Beginner

The beginner may not know why a repository exists, why a source should be abstracted, or why an integer ID is dangerous.

They should be able to follow a chain of reasoning rather than being told to memorise a pattern.

The documentation should answer:

> “Why would I want this boundary?”

before asking:

> “How do I implement it?”

## The Young Enthusiast

A young enthusiast may not have the money to buy a commercial device.

They may have a cheap ESP board, a small display, a few sensors and an enormous amount of curiosity.

That matters.

Telemetry should demonstrate that serious engineering is accessible through understanding, experimentation and disciplined iteration—not only through expensive equipment.

The framework should make it possible to start small while learning principles that remain useful when the system becomes large.

## The Application Developer

The application developer wants to describe meaning.

They should be able to declare an observation once and compose it into a Screen without learning the repository's storage layout.

Their concern is:

> “What does my application know, and how do I express that knowledge?”

That is why `telemetry.yaml` is an application composition source of truth.

## The Framework Developer

The framework developer protects reusable capabilities from application-specific knowledge.

Their concern is:

> “What should Telemetry provide, and what should the application provide?”

The framework should know how to provide capabilities.

It should not quietly become the owner of every application's vocabulary.

## The UX Designer

The UX perspective asks questions that source code cannot answer by itself:

- Can this be understood at a glance?
- Does colour have a consistent meaning?
- Is the hierarchy visible?
- Is the interaction discoverable?
- Does the screen still make sense when information comes from several sources?

The Energy screen's signed-flow arrow is a useful example. The sign of a battery value is technically correct, but the user needs a visual interpretation: energy leaving the battery and energy entering it.

Presentation semantics are therefore real architecture, not decoration.

## The Platform Engineer

The platform engineer sees constraints that higher layers may never notice.

RAM, framebuffer depth, flash, CPU time, connectivity and peripheral availability are not theoretical.

The ESP8266 four-bit framebuffer experiment demonstrated this directly: compilation succeeded, but the running device rebooted.

The lesson is not “never use four-bit colour.”

The lesson is:

> **A platform capability must be established by evidence on the target that matters.**

## The Maintainer

The maintainer arrives later.

They did not experience the original pressure.

They need to know why an apparently indirect design exists.

A maintainer asks:

> “What breaks if I simplify this?”

Architecture history exists partly to answer that question.

## The Architect

The architect has to hold all these perspectives at once.

That does not mean pleasing everyone equally.

It means understanding the different responsibilities and making their conflicts explicit.

The architect asks:

```text
Who needs this?
Who owns this knowledge?
Who changes this?
Who must not need to know this?
What happens when the implementation changes?
What happens when the platform changes?
What happens when the user changes?
```

## The Shared Boundary

Walking the boundary eventually leads back to one question:

> **Which knowledge should cross this boundary, and which knowledge should stop here?**

That is the architectural question behind much of Telemetry.
