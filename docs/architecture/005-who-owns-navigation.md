# 005 — Who Owns Navigation?

## The Question

When an input event occurs, who decides what happens next?

At first, this appears to be a simple question about navigation.

A user touches the screen.

A button is pressed.

A rotary encoder is turned.

A joystick moves.

Perhaps the application should show another screen.

But screen navigation is only one possible consequence of an interaction.

The same input event might instead:

* scroll through a menu
* select an item
* change a displayed value
* toggle a presentation format
* enter a configuration mode
* reset the device
* enter Wi-Fi access-point mode
* request an OTA update
* restore factory defaults

This leads to a more fundamental question:

>**Who owns the interpretation of an interaction?**

## Input Is Not Navigation

Telemetry deliberately separates the physical interaction from the meaning assigned to it.

The input pipeline is:
```text
Physical Input
      │
      ▼
Input Source
      │
      ▼
InputManager
      │
      ▼
Semantic InputEvent

```
At this point, the system knows what kind of interaction occurred.

For example:
```text
Touch at (120, 20)
        │
        ▼
TAP + position
```
Or:
```text
Joystick moved upward
        │
        ▼
SCROLL_UP
```
Or:
```text
Rotary encoder turned clockwise
        │
        ▼
INCREASE

```
The input system has normalised the physical interaction.

It has not yet decided what the interaction means to the application.

That distinction is deliberate.

>**Input produces interaction. Context produces meaning.**

## The Same Input Can Mean Different Things

Consider:
```text
SCROLL\_DOWN
```
On a weather screen, it might mean:
```text
SCROLL_DOWN
      │
      ▼
WeatherScreen
      │
      └── Move focus to the next area
```
On the Control Panel menu:
```text
SCROLL_DOWN
      │
      ▼
ControlMenuPage
      │
      └── Select the next menu item
```
On an information page:
```text
SCROLL_DOWN
      │
      ▼
AboutPage
      │
      └── Scroll the information
```
The input action is the same.

The interpretation is different.

The context owns the meaning.

This is why application-specific meanings should not be placed directly into the generic input vocabulary.

The input system should not produce:
```text
TOGGLE\_TIME\_FORMAT
OPEN\_ABOUT\_PAGE
RESET\_FACTORY\_DEFAULTS
ENTER\_AP\_MODE
START\_OTA
```
Those are not generic input actions.

They are **contextual interpretations of input actions**.

The input system may produce:
```text
TAP
LONG\_PRESS
SELECT
BACK
SCROLL\_UP
SCROLL\_DOWN
INCREASE
DECREASE
```
The appropriate context decides what those actions mean.

## Navigation Is Only One Possible Outcome

A semantic InputEvent may be handled at several levels.
```text
                         InputEvent
                              │
                              ▼
                  ┌─────────────────────┐
                  │   System Context    │
                  │                     │
                  │ System-level        │
                  │ interpretation?     │
                  └──────────┬──────────┘
                             │ No
                             ▼
                  ┌─────────────────────┐
                  │   ScreenManager     │
                  │                     │
                  │ Screen navigation?  │
                  └──────────┬──────────┘
                             │ No
                             ▼
                  ┌─────────────────────┐
                  │    Active Screen    │
                  │                     │
                  │ In-screen action?   │
                  └──────────┬──────────┘
                             │
                             ▼
                       Active Page
```
The event is not automatically a request to change screens.

The receiving context must first determine whether it owns the interaction.

## System-Level Interaction

Some interactions have nothing to do with the user interface currently being displayed.

For example, a special sequence from a keyboard, button matrix, or other input device might request:
```text
Key Sequence
      │
      ▼
System Context
      │
      ├── Reset device
      ├── Restore factory defaults
      ├── Enter Wi-Fi AP mode
      └── Request OTA update
```
This is conceptually similar to system-level keyboard combinations such as:
```text
CTRL + ALT + DEL
```
The physical keys are not themselves a request to reboot.

The system interprets the combination within a broader context.

The same principle applies to embedded systems.

A hidden button sequence might mean:
```text
LONG_PRESS
      │
      ▼
System Context
      │
      └── Enter recovery mode
```
Or:
```text
SELECT + BACK
      │
      ▼
System Context
      │
      └── Restore factory defaults
```
These interactions should not be routed through a screen merely because a screen happens to be visible.

The active screen is not necessarily the owner of every input event.

## Screen-Level Navigation

If the interaction is relevant to changing the active screen, that responsibility belongs to ScreenManager.

The ownership model is:
```text
InputEvent
      │
      ▼
ScreenManager
      │
      ├── Screen-level navigation?
      │       │
      │       ├── Yes
      │       │    └── Transition to another Screen
      │       │
      │       └── No
      │
      ▼
Active Screen
```
ScreenManager owns:

* which screen is active
* leaving the current screen
* activating the next screen
* entering the new screen
* screen-level lifecycle transitions

The active screen does not directly replace itself with another screen.

This preserves the architectural boundary:

>**Screens may express intent. ScreenManager owns screen transitions.**

## In-Screen Interaction

If the event does not represent screen-level navigation, the active screen receives it.
```text
InputEvent
      │
      ▼
ScreenManager
      │
      └── Not screen navigation
              │
              ▼
        Active Screen
              │
              ▼
        Active Page
```
The active screen may then interpret the interaction locally.

For example:
```text
TAP + position
      │
      ▼
WeatherScreen
      │
      └── Determine which focus area was tapped
```
Or:
```text
SCROLL_DOWN
      │
      ▼
ControlPanelScreen
      │
      ▼
ControlMenuPage
      │
      └── Select the next item
```
Or:
```text
TAP + position
      │
      ▼
WeatherScreen
      │
      └── Time area
              │
              └── Change time presentation
```
The input event remains generic.

The application context gives it meaning.

## The Ownership Contract

The resulting contract is:
```text
Physical Input
      │
      ▼
Input Source
      │
      └── Detect physical interaction
              │
              ▼
        InputManager
              │
              ├── Normalise interaction
              ├── Queue event
              └── Dispatch event
                      │
                      ▼
                 InputEvent
                      │
                      ▼
             Application Context
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
       System      Screen       Active
       Context     Manager      Screen
          │           │           │
          ▼           ▼           ▼
       System      Screen       Page /
       Action     Transition    Control
```
Each layer has a distinct responsibility.

### Input Source

Answers:

>What physical interaction occurred?

Examples:

* touch
* button press
* encoder movement
* joystick movement

### InputManager

Answers:

>How do we represent that interaction in a device-independent way?

It produces semantic InputEvent objects.

### System Context

Answers:

>Is this interaction a system-level command?

Examples:

* reset
* recovery mode
* AP mode
* factory reset
* OTA request

### ScreenManager

Answers:

>Does this interaction require changing the active screen?

If so, it owns the transition.

### Active Screen

Answers:

>What does this interaction mean within the current screen?

### Active Page or Control

Answers:

>What does this interaction mean within the current local context?

## Why Not Let Screens Handle Everything?

A simpler design might send every event directly to the active screen:
```text
InputEvent
      │
      ▼
Active Screen
```
This appears straightforward.

But it creates a problem.

The active screen becomes responsible for understanding interactions that may not belong to it.

For example:
```text
Factory Reset Sequence
      │
      ▼
WeatherScreen
      │
      └── Why am I responsible for this?
```
Or:
```text
Enter AP Mode
      │
      ▼
ControlPanelScreen
      │
      └── Is this a screen action or a system action?
```
The screen becomes coupled to concerns outside its responsibility.

This violates the principle:

>**One Responsibility Per Capability.**

A screen should render and interpret interactions within its own application context.

It should not become the universal owner of system commands.

## Why Not Let InputManager Decide?

The opposite design is equally problematic.

If InputManager decides:
```text
TAP at (120, 20)
      │
      └── Open About Page
```
then the input system must understand:

* the current screen
* the current page
* the screen layout
* application-specific controls
* system modes

The input system is no longer an input abstraction.

It has become an application controller.

That would violate:

>**Capabilities Before Hardware.**

The input system should provide an input capability.

It should not become the application.

## The Deeper Principle

The architecture therefore separates three distinct concerns:
```text
1. Interaction
      │
      ▼
What did the user physically do?

2. Interpretation
      │
      ▼
What does that interaction mean here?

3. Action
      │
      ▼
What should the system do as a result?
```
For example:
```text
Touch at (120, 20)
      │
      ▼
TAP + position
      │
      ▼
WeatherScreen interprets position
      │
      ▼
Time presentation changes
```
Or:
```text
Button sequence
      │
      ▼
Semantic input events
      │
      ▼
System Context interprets sequence
      │
      ▼
Device resets
```
Or:
```text
Joystick moved upward
      │
      ▼
SCROLL_UP
      │
      ▼
ControlMenuPage interprets action
      │
      ▼
Next menu item selected
```
The same input architecture supports all three.

## The Result

The final architectural boundary is:
```text
Physical Input
      │
      ▼
Input Source
      │
      ▼
InputManager
      │
      ▼
Semantic InputEvent
      │
      ▼
Contextual Interpretation
      │
      ├── System-level action
      │
      ├── Screen-level navigation
      │
      └── In-screen interaction
              │
              ▼
          Active Page
```
The important lesson is not that ScreenManager owns every input event.

It does not.

The important lesson is:

>**The owner of an interaction is determined by the context that gives the interaction meaning.**

ScreenManager owns screen transitions.

The active screen owns in-screen interpretation.

Pages and controls own their local interaction.

The system context owns system-level commands.

InputManager owns none of these meanings.

It only provides the common language through which they can be expressed.

## The Architectural Lesson

Input abstraction is not about supporting every possible input device.

It is about separating:

How the interaction happened

from:

What the interaction means

A touch screen, joystick, button matrix, rotary encoder, or keyboard may all produce different physical signals.

The framework should not force the rest of the application to understand those differences.

Instead:
```text
Different Physical Devices
            │
            ▼
   Common Interaction Model
            │
            ▼
     Contextual Meaning
```
This is why the input boundary exists.

Not because Telemetry must support every input device immediately.

But because the meaning of an interaction should not be permanently coupled to the mechanism that produced it.

>**Input is semantic.**
>
>**Meaning is contextual.**
>
>**Ownership belongs to the context that gives the interaction meaning.**

That is the boundary.
