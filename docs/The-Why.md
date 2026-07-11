**The Why**

_"The real product is helping someone say..._

**_'Ahh... now I understand why.'_**"

**This isn't really a repository about firmware.**

At first glance it probably looks like one.

There's an ESP8266.

A TFT display.

MQTT.

Wi-Fi.

OTA updates.

Sensors.

Touch input.

The usual embedded things.

But those aren't actually why this repository exists.

**It started with a weather station.**

Like so many hobby projects, I simply wanted to display some weather information on a small TFT screen.

That worked.

Then I wanted to add another sensor.

Then another.

Then another screen.

Then OTA updates.

Then calibration.

Then MQTT.

Then different displays.

Then different input devices.

Before long I realised something.

Every new feature wasn't creating a coding problem.

It was creating an architecture problem.

**I kept asking myself the same question.**

Not...

"How do I make this work?"

Instead...

**"Why does this feel messy?"**

Why does the display know about MQTT?

Why does the touch driver know about weather?

Why does calibration interrupt boot?

Why does changing a display require changing application code?

Why can't this screen work with buttons instead of touch?

Those questions slowly changed the project.

**Somewhere along the way I realised something.**

I wasn't building a weather station anymore.

I was trying to understand how good systems are built.

Telemetry simply became the vehicle.

**Throughout my career I've worked in several different worlds.**

Electronics.

Networking.

Infrastructure.

Enterprise Architecture.

Solution Architecture.

User Experience.

At first those disciplines seemed completely unrelated.

Years later I realised they all shared the same principles.

Observe.

Understand.

Simplify.

Separate concerns.

Reduce coupling.

Make change easy.

Those ideas don't belong to firmware.

They belong to engineering.

**One lesson keeps appearing.**

Whenever I solve a problem, people often ask me:

"How did you know to do that?"

The honest answer is...

I usually didn't.

I simply followed the same process.

Observe.

Form a hypothesis.

Run an experiment.

Collect evidence.

Update my understanding.

Repeat.

It works for debugging firmware.

It works for routing problems.

It works for distributed systems.

It even works for life surprisingly often.

**This repository isn't trying to impress anyone.**

Quite the opposite.

You'll probably find code that could be shorter.

Cleaner.

More clever.

I often leave the less clever solution if it's easier to understand.

Because someone reading the code six months from now matters more than saving three lines today.

That "someone" is usually me.

Sometimes it might be you.

**If you're just starting...**

Welcome.

Seriously.

You don't need to know embedded systems.

You don't need to know C++.

You don't need to know MQTT.

You certainly don't need to know architecture.

Nobody starts there.

I certainly didn't.

Take your time.

Ask questions.

Break things.

Fix them.

That's how engineers grow.

**If you've been doing this for years...**

Welcome to you too.

You'll probably disagree with some of my decisions.

I hope you do.

Engineering advances because people challenge ideas respectfully.

If Telemetry starts a good architectural discussion, then it's already doing its job.

**What Telemetry tries to teach**

Not electronics.

Not ESP8266s.

Not displays.

Not MQTT.

Those are just examples.

Telemetry tries to teach something much more transferable.

How to think about systems.

Because today's TFT display becomes tomorrow's CAN bus.

Today's weather station becomes tomorrow's industrial controller.

Today's hobby project becomes tomorrow's production system.

Technology changes.

Good thinking lasts.

**So why publish all of this?**

Because I owe an enormous debt to the engineers who took the time to explain things to me over the years.

Not just what they were doing.

Why.

That single extra sentence often saved me days.

Sometimes years.

This repository is simply my way of paying that debt forward.

**If there's one thing I'd like you to take away...**

Don't copy the code.

Question it.

Challenge it.

Improve it.

Make it your own.

If, somewhere along the journey, you find yourself smiling and thinking,

**"Ahh... now I understand why."**

then Telemetry has already succeeded.

Everything else is just firmware.