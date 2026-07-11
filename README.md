**Telemetry**

**An embedded observability framework.**

Not for building weather stations.

For understanding how good firmware is designed.

Telemetry started life as a simple ESP8266 weather display.

Then something unexpected happened.

Every new feature raised a better architectural question.

Should displays know about MQTT?

Should touch know about screens?

Should calibration interrupt boot?

Could the same firmware run on completely different hardware?

Eventually it stopped being a weather project.

It became an exploration of embedded architecture.

Today Telemetry is an engineering notebook disguised as firmware.

If you've ever looked at a project and wondered...

"Why was it built like that?"

...you're in the right place.

**What you'll find**

- reusable embedded architecture
- hardware abstraction
- board profiles
- capability-driven design
- display abstraction
- input abstraction
- observability
- OTA
- MQTT
- documentation explaining **why**, not just **how**

**Philosophy**

Good engineers eventually stop asking

"How do I write this?"

and start asking

"Why is this designed that way?"

Telemetry exists to help bridge that gap.

If something in this repository makes you stop and say

**"Ahh... now I understand why."**

then it's done its job.