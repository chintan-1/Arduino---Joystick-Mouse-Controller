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
