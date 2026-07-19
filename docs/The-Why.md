# The Why

> *"The real product is helping someone say:
>
> **'Ahh... now I understand why.'**"*

---

## Why this repository exists

If you've ended up here because you're looking for an ESP8266 weather station...

...you've probably taken a wrong turn.

There are already hundreds of fantastic projects that will get pixels onto a screen, publish values over MQTT, or connect a sensor to Home Assistant.

Telemetry isn't trying to compete with any of them.

Instead, it's trying to answer a different question.

**Why are they built that way?**

---

## It didn't start like this

Like most hobby projects, Telemetry started with a simple goal.

Show some weather data on a display.

That sounded easy enough.

Then one small change led to another.

Could I separate the display from the data?

Could touch input become reusable?

Should calibration interrupt boot?

What happens if the hardware changes?

Could the same application run on a completely different display?

Before long, I realised I wasn't building a weather station anymore.

I was learning architecture.

This repository simply became my notebook.

---

## What I learned the hard way

Over the years I've worked in electronics, networking, enterprise architecture and user experience.

Different industries.

Different technologies.

Different scales.

Yet I kept discovering the same lessons over and over again.

Good systems usually aren't clever.

They're understandable.

Good abstractions make complexity disappear.

Poor abstractions make simple things complicated.

Names matter.

Comments should explain intent.

Debugging isn't guessing.

It's science.

Those lessons weren't unique to embedded systems.

They applied everywhere.

Eventually I realised they belonged together in one place.

That's what this repository became.

---

## Telemetry isn't the product

This is probably the most important thing I can say.

Telemetry isn't really about firmware.

The firmware is just the vehicle.

The real product is understanding.

If something in this repository helps you stop for a moment and think,

> "Ahh... now I understand why they did it that way."

then it's done exactly what I hoped it would.

---

## You don't need to know everything

One of the biggest myths in engineering is that everyone else already knows what they're doing.

They don't.

We're all figuring things out.

Some people simply have more years of mistakes behind them.

If you're just getting started with embedded systems...

...welcome.

If you're an experienced firmware engineer...

...welcome.

If you're a software developer who's never touched a microcontroller before...

...welcome.

If you're an architect wondering how the same design principles scale all the way down to a few hundred kilobytes of RAM...

...you're especially welcome.

This repository is for curious people.

---

## This isn't about copying code

I genuinely hope you don't copy much of the code.

Seriously.

I hope you understand it well enough to write your own.

Because that's how engineering grows.

Not by memorising solutions.

By understanding why those solutions exist.

---

## If you only take one thing away...

Technology changes.

Displays change.

Microcontrollers change.

Protocols change.

Languages change.

Good engineering thinking doesn't.

If Telemetry leaves you with one thing, I hope it's this:

Always ask **why** before asking **how**.

The "how" changes every few years.

The "why" lasts a career.

---

If something in these pages makes you smile and quietly say,

> **"Ahh... now I understand why."**

then this repository has achieved exactly what it was built to do.