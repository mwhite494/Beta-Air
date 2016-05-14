/*

UVM SEED Team #14
Beta Air
Simulator Platform Arduino Sketch

This program prints live acceleration data from an Xbee module communicating
with the Xbee module located on the quadcopter. If we had more time to develop
our simulator platform, this program would have also been responsible for a motion
cueing algorithm to determine the step and direction of each of the three DC stepper motors.

Written By: Marco White
Last Edited: 5/8/16

*/

// String to hold incoming data
String data;

// Setup loop
void setup() {
  // Initialize serial
  Serial.begin(9600);
  
}

// Main loop
void loop() {
  // Check for incoming data
  if (Serial.available() > 0) {
    // Read data
    data = Serial.readString();
    // Print received data to the serial monitor
    Serial.println(data);
  }
}

