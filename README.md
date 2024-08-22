# Arduino Joystick Mouse Control with Toggle and Click Features

This project enables control of your computer mouse using a joystick and additional buttons connected to an Arduino. The joystick controls the mouse movement, a toggle switch activates or deactivates mouse control, and a separate button simulates left mouse clicks.

## Components Required

- Arduino Leonardo, Micro, or Due
- Joystick with X and Y axes
- Two pushbuttons
- Breadboard and jumper wires
- LED (optional, for status indication)

## Circuit Diagram

1. **Joystick X Axis**: Connect to **Analog Pin A0** on the Arduino.
2. **Joystick Y Axis**: Connect to **Analog Pin A1** on the Arduino.
3. **Toggle Switch**: Connect to **Digital Pin 2** on the Arduino.
4. **Left Click Button**: Connect to **Digital Pin 3** on the Arduino.
5. **Status LED**: Connect to **Digital Pin 5** on the Arduino (optional).

## Code Description

```cpp
#include "Mouse.h"

// Pin configuration for switches, joystick, and LED indicator:
const int toggleSwitchPin = 2;    // Pin for toggling mouse control
const int leftClickPin = 3;       // Pin for left mouse click
const int joyXPin = A0;           // X-axis of joystick
const int joyYPin = A1;           // Y-axis of joystick
const int statusLedPin = 5;       // LED for showing mouse control status

// Joystick parameters:
int movementRange = 12;           // Range for mouse movement
int movementDelay = 5;            // Delay in mouse response (milliseconds)
int idleThreshold = movementRange / 4;  // Threshold for idle joystick
int midpoint = movementRange / 2;  // Center position value for joystick

bool isMouseActive = false;       // Status of mouse control
int previousSwitchState = LOW;    // Previous state of the toggle switch

void setup() {
  pinMode(toggleSwitchPin, INPUT);  // Set toggle switch pin as input
  pinMode(statusLedPin, OUTPUT);    // Set LED pin as output
  Mouse.begin();                    // Initialize mouse control
}

void loop() {
  // Check the state of the toggle switch:
  int currentSwitchState = digitalRead(toggleSwitchPin);
  
  // Toggle mouse control if the switch state changes:
  if (currentSwitchState != previousSwitchState) {
    if (currentSwitchState == HIGH) {
      isMouseActive = !isMouseActive;  // Toggle mouse control
      digitalWrite(statusLedPin, isMouseActive);  // Update LED
    }
  }
  previousSwitchState = currentSwitchState;  // Update the switch state

  // Get and scale joystick readings:
  int xMovement = getAxisMovement(joyXPin);
  int yMovement = getAxisMovement(joyYPin);

  // Move mouse if control is active:
  if (isMouseActive) {
    Mouse.move(xMovement, yMovement, 0);
  }

  // Handle the left-click button:
  if (digitalRead(leftClickPin) == HIGH) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
  } else {
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  }

  delay(movementDelay);  // Add response delay
}

// Function to read and scale joystick axis data:
int getAxisMovement(int axisPin) {
  int axisValue = analogRead(axisPin);  // Read analog value from joystick
  axisValue = map(axisValue, 0, 1023, 0, movementRange);  // Scale the reading

  int distanceFromCenter = axisValue - midpoint;
  if (abs(distanceFromCenter) < idleThreshold) {
    distanceFromCenter = 0;  // Ignore small movements
  }

  return distanceFromCenter;  // Return the final movement value
}
```
## How It Works

- **Joystick Control:** The joystick controls the mouse movement along the X and Y axes.
- **Mouse Activation:** A toggle switch activates or deactivates mouse control. The LED status indicator reflects whether the mouse control is active.
- **Left Click:** A separate button simulates left mouse clicks. When pressed, the button triggers a mouse click event; when released, it releases the click.

## How to Use

1. **Upload the Code:** Upload the provided code to your Arduino board using the Arduino IDE.
2. **Set Up the Circuit:** Connect the joystick, toggle switch, click button, and LED as described in the circuit diagram.
3. **Test the Setup:** Use the toggle switch to activate or deactivate mouse control and the joystick to move the mouse cursor. The left-click button will simulate mouse clicks.

## Important Notes

- The code uses the `Mouse.h` library, which is compatible with Arduino boards that support USB HID (Leonardo).
- Ensure the joystick and buttons are connected correctly to avoid any issues with mouse control or clicking.
- The `Mouse.move()` command takes over the mouse control, so ensure you can regain control before using the command.
