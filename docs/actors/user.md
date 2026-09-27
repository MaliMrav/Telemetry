# Actor — The User

The user is the reason the information is presented at all.

They should not need to know how Telemetry obtains or stores an observation.

## Questions the User Asks

- What is happening right now?
- Can I understand this quickly?
- Can I trust the value?
- What does this colour or arrow mean?
- What happens if I interact with the display?

## Architectural Consequence

The presentation layer must communicate meaning without exposing implementation details.

A transport name such as an MQTT topic is not a user-facing identity.

A signed battery value may be mathematically precise but still require a visual convention so the user understands the direction of energy flow.

## What Good Architecture Protects

The user should experience the consequences of architectural quality without having to see the architecture itself.
