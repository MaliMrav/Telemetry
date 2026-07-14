# How We Think

These principles guide every architectural decision made within Telemetry.

They are intentionally technology-agnostic.

Frameworks change.

Hardware changes.

Programming languages evolve.

Good engineering principles endure.

# 1. Architecture Before Implementation

Every significant change should begin with architecture.

A well-designed architecture makes implementation feel inevitable.

Code should be the consequence of good design, not the process used to discover it.

# 2. The Repository Should Reflect the Architecture

A well-structured repository should communicate the architecture before a single source file is opened.

Developers should be able to browse the directory tree and immediately understand:

- what the major applications are
- how responsibilities are separated
- where capabilities belong
- how the framework is organised

Documentation should explain the architecture, not compensate for it.

Good architecture should be visible.

Before a single line of code is read.

# 3. Capabilities Before Hardware

The framework thinks in capabilities.

Not displays.

Not touch controllers.

Not Wi-Fi chips.

Hardware provides capabilities.

The framework exposes them.

Applications consume them.

# 4. Observations Before Information

Firmware should observe.

It should collect measurements faithfully.

Transformation into useful information should be deterministic.

Inference belongs elsewhere.

Telemetry deliberately stops before artificial intelligence.

# 5. Information Before Intelligence

Telemetry transforms observations into information.

It intentionally leaves intelligence, automation, analytics and decision making to upstream systems.

Home Assistant.

Node-RED.

Grafana.

Cloud services.

Artificial Intelligence.

Those platforms exist to reason.

Telemetry exists to observe.

# 6. One Responsibility Per Capability

Displays render.

Sensors observe.

Storage persists.

Connectivity transports.

Diagnostics observe.

Each capability should have one responsibility, and perform it well.

# 7. Input Is Semantic

Screens never care whether an action originated from:

- touch
- buttons
- rotary encoders
- keyboards
- gamepads

Screens respond only to semantic actions.

Input devices produce actions.

Screens consume them.

# 8. Every Abstraction Has a Cost

Abstractions should reduce complexity.

Never increase it.

If an abstraction makes the system harder to understand, it probably shouldn't exist.

# 9. Composition Before Inheritance

Objects should be assembled from smaller responsibilities.

Inheritance should be rare.

Composition keeps systems understandable.

# 10. Dependencies Flow Downward

High-level policy should never depend upon hardware.

Applications depend on capabilities.

Capabilities depend on drivers.

Drivers depend on hardware.

Never the other way around.

Dependencies should become simpler as they move downward.

# 11. Firmware Should Be Observable

Firmware should always be able to explain itself.

If the firmware cannot explain what it is doing,

neither can the engineer.

Health.

Memory.

Connectivity.

Boot progress.

Versions.

Timing.

Failures.

Unknown behaviour is almost always harder to debug than incorrect behaviour.

# 12. Good Architecture Should Feel Inevitable

The best architecture rarely feels surprising.

Instead, it produces the reaction:

> "Of course...
> why would it be designed any other way?"

That is usually a sign the abstractions are correct.

# 13. Architecture is User Experience

Not just for users.

For contributors.

For future maintainers.

For the engineer reading the code five years from now.

Good architecture reduces cognitive load.

A system should feel discoverable long before it feels familiar.

# 14. Teach Through Architecture

Telemetry exists to teach.

Not by simplifying problems.

But by making good engineering visible.

The repository should help developers understand not only *what* the code does...

...but *why* it is designed that way.

# 15. Clarity Over Cleverness

Clever code impresses today's developer.

Clear code teaches tomorrow's developer.

Readable firmware is maintainable firmware.

Prefer obvious solutions over clever ones.

Future readers include your future self.

# 16. Framework Before Application

The framework should never become tightly coupled to a single application.

Weather is one implementation of the framework.

Telemetry is the framework.

The framework must always remain reusable.

# 17. The Knowledge Pyramid

Telemetry occupies only part of the engineering knowledge pyramid.

            Wisdom
               ▲
          Knowledge
               ▲
         Intelligence
               ▲
         Information
               ▲
             Data
               ▲
        Observations

Telemetry is responsible for moving observations toward information.

It deliberately stops there.

Everything above belongs to higher-level systems.

# 18. The Real Product

Telemetry is not the product.

The dashboard is not the product.

The framework is not the product.

The real product is helping someone say:

> "Ahh... now I understand why."

---

# A Final Thought

These principles are not rules.

They are reminders.

They exist to help us make consistent decisions as Telemetry evolves.

If a future design forces us to challenge one of these principles, the discussion itself is valuable.

Good engineering is not about following rules.

It is about understanding why they exist.