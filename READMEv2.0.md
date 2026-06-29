# Field Notes in Systems Engineering

## A living engineering notebook for understanding connected systems

This repository is not a product.

It is not a framework.

It is not a library.

It is a **record of engineering thought in motion**.

It documents how complex systems are designed, observed, debugged, and understood—from UX considerations and architectural decisions, all the way down to packets, registers, and electrons.

---

## Philosophy (Read This First)

Modern systems are often presented as finished artefacts.

Clean abstractions. Stable APIs. Perfect diagrams.

Real engineering is not like that.

Real engineering is iterative, messy, exploratory, and deeply human.

This repository exists to demonstrate a different approach:

> Systems are not built. They are *discovered through feedback*.

Every subsystem here is:

- Designed with intent
- Tested through experimentation
- Revised based on evidence
- Documented with reasoning, not just outcomes

The goal is not to present "best practice".

The goal is to show **why decisions were made, and what trade-offs were accepted**.

---

## What This Repository Is

This project explores:

- Embedded systems (ESP8266 / ESP32)
- Telemetry pipelines
- MQTT-based architectures
- Wi-Fi provisioning flows
- BLE device interaction
- UI/UX considerations for constrained devices
- Network architecture and segmentation
- Observability and debugging techniques

But none of these are the real subject.

They are **case studies** in systems thinking.

---

## What Success Looks Like

This project succeeds if:

- Someone builds their first embedded system and understands *why it behaves the way it does*
- Someone learns to debug systems by observing, not guessing
- Someone challenges an architectural decision and improves it
- Someone moves from “it doesn’t work” to “I understand why it doesn’t work”
- Someone applies these ideas outside IoT—in backend systems, cloud architecture, or distributed systems

If that happens, the code has done its job.

---

## Structure
/src → firmware and embedded logic
/lib → reusable components
/docs → architecture and system design
/adr → architectural decisions
/captains-log → engineering narrative and experiments
/telemetry → data models and pipelines
/tools → debugging and analysis utilities


---

## Core Ideas

### 1. Systems are state machines

Everything is a state machine:
- provisioning flows
- network connectivity
- UI state
- sensor pipelines

Understanding state is more important than understanding code.

---

### 2. Observability is a first-class requirement

If you cannot observe it, you cannot debug it.

Telemetry is not optional—it is foundational.

---

### 3. Every abstraction has a cost

Abstractions are not free.

They:
- obscure behaviour
- introduce latency
- hide failure modes

We treat them as design trade-offs, not defaults.

---

### 4. UX is part of engineering

User experience is not just UI.

It includes:
- onboarding flows
- failure states
- recovery paths
- cognitive load during setup

A system that works but cannot be understood is incomplete.

---

## Captain’s Log

This repository includes a parallel narrative:


/captains-log

These are not formal architecture decisions.

They are engineering field notes.

Each entry includes:
- hypothesis
- experiments
- observations
- failures
- conclusions

Think of them as engineering diaries.

---

## Architecture Decision Records (ADR)

Formal decisions live in:
/adr

These documents explain *what was chosen and why*.

They are concise, structured, and final.

---

## Why “Telemetry”?

Because everything here is about feedback:

- Devices emit data
- Networks reveal behaviour
- Systems expose state
- Users reveal friction
- Failures reveal design flaws

Telemetry is how systems tell you the truth.

---

## Audience

This repository is written for:

> The curious engineer.

Not beginners. Not experts.

Engineers who want to understand how systems behave in the real world.

---

## License

Open for learning, reuse, and adaptation.

If it helps you understand systems better, it has succeeded.