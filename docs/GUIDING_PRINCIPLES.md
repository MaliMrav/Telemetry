# Guiding Principles

These principles guide every architectural decision within Telemetry.

They are intentionally technology-agnostic.

Frameworks change.

Hardware changes.

Programming languages evolve.

Good engineering principles endure.

---

# 1. Architecture Before Implementation

Every significant change should begin with architecture.

A well-designed architecture makes implementation feel inevitable.

Code should be the consequence of good design, not the process used to discover it.

---

# 2. Capabilities Before Hardware

The framework thinks in capabilities.

Not displays.

Not touch controllers.

Not Wi-Fi chips.

Hardware merely provides a capability.

Applications consume capabilities.

---

# 3. Observations Before Information

Firmware should observe.

It should collect measurements faithfully.

Transformation into useful information should be deterministic.

Inference belongs elsewhere.

Telemetry deliberately stops before artificial intelligence.

---

# 4. Information Before Intelligence

Telemetry transforms observations into information.

It intentionally leaves intelligence, automation, analytics and decision making to upstream systems.

Home Assistant.

Node-RED.

Grafana.

Cloud services.

Artificial Intelligence.

Those platforms exist to reason.

Telemetry exists to observe.

---

# 5. Separate Collection From Presentation

Displays should render.

Sensors should measure.

Networking should transport.

Storage should persist.

No component should perform another component's responsibility.

---

# 6. Input Is Semantic

Screens never care whether an action originated from:

- touch
- buttons
- rotary encoders
- keyboards
- gamepads

Screens respond only to semantic actions.

Input devices produce actions.

Screens consume them.

---

# 7. Every Abstraction Has a Cost

Abstractions should reduce complexity.

Never increase it.

If an abstraction makes the system harder to understand, it probably shouldn't exist.

---

# 8. Composition Before Inheritance

Objects should be assembled from smaller responsibilities.

Inheritance should be rare.

Composition keeps systems understandable.

---

# 9. Dependencies Flow Downward

High-level policy should never depend upon hardware.

Applications depend on capabilities.

Capabilities depend on drivers.

Drivers depend on hardware.

Never the other way around.

---

# 10. Firmware Should Be Observable

Firmware should always be able to explain itself.

Health.

Memory.

Connectivity.

Boot progress.

Versions.

Timing.

Failures.

Unknown behaviour is almost always harder to debug than incorrect behaviour.

---

# 11. Good Architecture Should Feel Inevitable

The best architecture rarely surprises people.

Instead, it produces the reaction:

> "Of course...
> why would it be designed any other way?"

That is usually a sign the abstractions are correct.

---

# 12. Teach Through Architecture

Telemetry exists to teach.

Not by simplifying problems.

But by making good engineering visible.

The repository should help developers understand not only *what* the code does...

...but *why* it is designed that way.

---

# 13. Clarity Over Cleverness

Readable firmware is maintainable firmware.

Prefer obvious solutions over clever ones.

Future readers include your future self.

---

# 14. Framework Before Application

The framework should never become tightly coupled to a single application.

Weather is one application.

Telemetry is the framework.

The framework must always remain reusable.

---

# 15. The Knowledge Pyramid

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

---

# 16. The Real Product

Telemetry is not the product.

The dashboard is not the product.

The framework is not the product.

The real product is helping someone say:

> "Ahh... now I understand why."