# Philosophy

Telemetry is a technical project, but it is also a learning project.

The purpose of this documentation is therefore larger than describing how the firmware works. It records how we arrived at the architecture, how we recognise pressure, how we decide where knowledge belongs, and how those decisions can be learned by somebody who was not present when they were made.

The project is guided by a simple ambition:

> **Create Knowledge that can pass on Wisdom to people who care to learn and are eager to understand.**

## Knowledge and Wisdom

The **Knowledge Management Triangle** is a Northern Star for this project:

```text
Storage
   ↓
unstructured Data
   ↓
Information
structured Data
   ↓
Intelligence
   ↓
Knowledge
   ↓
Wisdom
```

Telemetry deliberately operates in the lower part of this journey. It observes, structures, transports, presents and makes information understandable. It should not silently become the system that makes every higher-level decision.

The architecture exists partly to make that boundary visible.

Knowledge tells us what the system is and why it is shaped this way.

Wisdom is what a reader may carry away and apply somewhere else.

## The Four Documents

- [How We Think](how-we-think.md) — the reasoning habits used when designing Telemetry.
- [Walking the Boundary](walking-the-boundary.md) — how we examine a problem from the perspective of every actor affected by it.
- [Design Principles](design-principles.md) — the principles that have survived repeated architectural pressure.

These are not a second architecture reference. They explain the thinking behind the architecture journey.

## The Architecture Journey

The architecture documents record pressure and response:

```text
Pressure
   ↓
Question
   ↓
Evidence
   ↓
Decision
   ↓
New capability
   ↓
New pressure
```

That history matters because a decision without its pressure is easy to cargo-cult.

A reader should be able to disagree with a decision while still understanding why it was reasonable at the time.

## The Teaching Principle

Telemetry is intended to occupy the space between:

> **“I made it work.”**

and:

> **“I built something that can last.”**

That messy middle is where architecture lives.

The documentation should make that middle visible.
