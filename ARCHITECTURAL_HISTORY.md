# Architectural History

_Software evolves._

_Architecture evolves differently._

This document records the major architectural milestones in Telemetry-not simply what changed, but why those changes mattered.

# The Prototype

### _"Can I make this work?"_

Telemetry didn't begin as a framework.

It began as a personal weather display.

The original objective was simple:

- connect to Wi-Fi
- subscribe to MQTT topics
- display useful information
- make it pleasant to look at

Like many hobby projects, the first version focused entirely on making something useful.

It wasn't designed to be reusable.

It was designed to solve a problem.

That was enough.

# The Application

### _"Can I keep this maintainable?"_

As more features appeared, so did more coupling.

Weather logic knew about displays.

Touch knew about screens.

Screens knew about hardware.

Configuration became scattered.

Every new feature became slightly harder than the previous one.

The project still worked.

But something didn't feel right.

This stage taught an important lesson:

Working software is not necessarily sustainable software.

# The Separation

### _"What actually belongs together?"_

Rather than adding more features, the project took a step backwards.

Responsibilities were examined.

Hardware.

Input.

Screens.

Services.

Data.

Presentation.

Each component was asked a simple question:

What is your responsibility?

If the answer contained "and…"

the class probably had more than one.

This marked the beginning of Telemetry's architectural refactor.

# The Framework

### _"Can the hardware disappear?"_

The next goal became hardware independence.

Application code should not know:

- which display is connected
- which touch controller is fitted
- whether input comes from touch, buttons or a rotary encoder

Hardware became providers.

Application became consumers.

Input became semantic.

Instead of responding to touch coordinates, screens respond to intentions:

- Previous Screen
- Next Screen
- Select
- Scroll Up
- Scroll Down

The user interface no longer cares _how_ those actions were generated.

# The Teaching Project

### _"Can architecture be learned through code?"_

At some point, the project quietly changed purpose.

The firmware stopped being the primary product.

The learning became the product.

The repository now exists to demonstrate ideas that are often difficult to explain in isolation:

- separation of concerns
- abstraction
- state
- ownership
- layering
- configuration
- hardware independence
- user experience
- observability

Every architectural decision attempts to answer one question:

Why is it built this way?

The code is simply the evidence.

# Today

Telemetry continues to evolve.

Not by chasing features.

Not by adding complexity.

But by asking better questions.

The goal isn't to produce the most sophisticated firmware.

The goal is to produce firmware that explains itself.

# Tomorrow

Future releases will continue refining the framework.

Examples include:

- board profiles
- hardware abstraction
- display abstraction
- input devices
- calibration improvements
- settings
- diagnostics
- configuration

These are not independent features.

They are chapters in the same story.

# A Different Kind of Repository

Telemetry is intentionally different from many open-source projects.

It doesn't attempt to teach only **what** to build.

Nor only **how** to build it.

It aims to explain **why** certain engineering decisions lead to systems that are easier to understand, maintain and extend.

If, while reading the code or documentation, you ever find yourself saying:

**"Ahh… now I understand why."**

then Telemetry has achieved exactly what it set out to do.