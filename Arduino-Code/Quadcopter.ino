/*

UVM SEED Team #14
Beta Air
Quadcopter Arduino Sketch

This program reads three linear accelerations from a 3-axis accelerometer
and three angular velocities from a 3-axis gyroscope located on a
6 Degree of Freedom Internal Measurement Unit shield (6 DOF IMU).
The shield has an embeeded Xbee socket which is used to transmit
these values to an Arduino located on the simulator platform.

Written By: Marco White
Last Edited: 5/8/16

*/

// Load the 6 DOF IMU shield libraries
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
// Load Wire library
#include <Wire.h>
 
// An array that holds the angles for pitch and roll
int16_t angle[2];
  
// Define the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();
// Array to hold the raw accelerometer and gyroscope data
int rawSixDof[6];
// Array to hold some rough calibration values
int calib[6];
int count = 0;

// Setup loop
void setup() 
{ 
  // Initialize serial and wire
  Serial.begin(9600);
  Wire.begin();
  // Initialize cailbration values
  for (int i = 0; i < 6; i++) {
    calib[i] = 0;
  }
  // Set LED pin 13 as an output and write it low
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Initialize the IMU
  delay(5);
  sixDOF.init();
  delay(5);
}

// Main loop
void loop() { 
  // Read the raw sensor values
  sixDOF.getRawValues(rawSixDof);
  // Obtain some average calibration values from 10 loop iterations
  // Assume during this time the quadcopter is level and at rest
  count++;
  if (count >= 10 && count < 20) {
    if (count == 10) {
      for (int i = 0; i < 6; i++) {
        calib[i] = rawSixDof[i];
      }
    }
    else {
      for (int i = 0; i < 6; i++) {
        calib[i] = (calib[i] + rawSixDof[i])/2;
      }
    }
  }
  // Once the calibration is complete begin transmitting acceleration data
  if (count >= 20) {
    // Write LED pin 13 high to denote that calibration is done
    digitalWrite(13, HIGH);
    // Calculate angles for pitch and roll
    angle[0] = _atan2(rawSixDof[0],rawSixDof[2]);
    angle[1] = _atan2(rawSixDof[1],rawSixDof[2]);
    // Transmit data
    Serial.print("Acc.x: ");
    Serial.print(rawSixDof[0] - calib[0]);
    Serial.print("   Acc.y: ");
    Serial.print(rawSixDof[1] - calib[1]);
    Serial.print("   Acc.z: ");
    Serial.print(rawSixDof[2] - calib[2]);
    Serial.print("   Gyro.x: ");
    Serial.print(rawSixDof[3] - calib[3]);
    Serial.print("   Gyro.y: ");
    Serial.print(rawSixDof[4] - calib[4]);
    Serial.print("   Gyro.z: ");
    Serial.print(rawSixDof[5] - calib[5]);
    Serial.print("   Pitch(X): ");
    Serial.print(angle[0]/10.0);
    Serial.print("   Roll(Y): ");
    Serial.print(angle[1]/10.0);
    Serial.println("");
    // Delay
    //delay(1000);
  }
}

// Get atan2
int16_t _atan2(int32_t y, int32_t x)
{ 
  float z = (float)y / x;
  int16_t a;
  if ( abs(y) < abs(x) )
  {
    a = 573 * z / (1.0f + 0.28f * z * z);
    if (x<0)
    {
    if (y<0) a -= 1800;
    else a += 1800;
    }
  }
  else 
  {
    a = 900 - 573 * z / (z * z + 0.28f);
    if (y<0) a -= 1800;
  }
  return a;
}

