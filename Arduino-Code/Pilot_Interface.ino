/*

UVM SEED Team #14
Beta Air
Pilot Interface Arduino Sketch

This program reads sensor data from the pilot interface and then
writes values to a hacked RC controller.

Written By: Marco White
Last Edited: 3/14/16

*/

// Pin Definitions
#define LOAD_CELL_CLK 2
#define LOAD_CELL_DAT 3
#define POT1_SHDN 7
#define POT1_SS 10
#define POT2_SHDN 8
#define POT2_SS 9
#define POTS_SDI 11
#define POTS_SDO 12
#define POTS_CLK 13
#define FSR_LEFT A1
#define FSR_RIGHT A0
#define THROTTLE_POT A2
#define HANDLEBAR_POT A3

// Load libraries
#include "HX711.h"
#include <SPI.h>

// Define program constants
// Load cell calibration factor
#define LOAD_CELL_CALIBRATION -7050.0
// Sensor minimum and maximum values
#define LOAD_CELL_MIN -20
#define LOAD_CELL_MAX 20
#define FSR_DIFF_MIN -120
#define FSR_DIFF_MAX 120
#define THROTTLE_POT_MIN 630
#define THROTTLE_POT_MAX 1023
#define HANDLEBAR_POT_MIN 530
#define HANDLEBAR_POT_MAX 870
// Joystick minimum and maximum resistance values
#define PITCH_MIN 10
#define PITCH_MAX 220
#define ROLL_MIN 30
#define ROLL_MAX 240
#define THROTTLE_MIN 40
#define THROTTLE_MAX 220
#define YAW_MIN 20
#define YAW_MAX 240
// Digital potentiometer addresses
#define TCON_WRITE_ADDR B01000000
#define BOTH_WIPERS_ON B11111111
#define WIPER0_ADDR B00000000
#define WIPER1_ADDR B00010000

// Initialize global variables
HX711 scale(LOAD_CELL_DAT, LOAD_CELL_CLK);
int loadCellVal, fsrLeft, fsrRight, throttlePot, handlebarPot;
int pitchVal, rollVal, throttleVal, yawVal;

// Setup
void setup() {
  // Initialize serial monitor
  Serial.begin(9600);
  // Initialize load cell
  scale.set_scale(LOAD_CELL_CALIBRATION);
  scale.tare();
  // Initialize digital potentiometers
  pinMode(POT1_SHDN, OUTPUT);
  pinMode(POT1_SS, OUTPUT);
  pinMode(POT2_SHDN, OUTPUT);
  pinMode(POT2_SS, OUTPUT);
  // Pulse the shutdown pins on both pots
  digitalWrite(POT1_SHDN, LOW);
  digitalWrite(POT2_SHDN, LOW);
  delay(10);
  digitalWrite(POT1_SHDN, HIGH);
  digitalWrite(POT2_SHDN, HIGH);
  // Initialize SPI (communication interface for digital pots)
  SPI.begin();
  // Start with both digital pots de-selected
  digitalWrite(POT1_SS, HIGH);
  digitalWrite(POT2_SS, HIGH);
}

// Infinite loop
void loop() {
  // Get the load cell value
  loadCellVal = (int)(scale.get_units()+0.5);
  // Get FSR values (at the knees)
  fsrLeft = analogRead(FSR_LEFT);
  fsrRight = analogRead(FSR_RIGHT);
  // Get analog potentiometer values
  throttlePot = analogRead(THROTTLE_POT);
  handlebarPot = analogRead(HANDLEBAR_POT);
  
  // Print sensor values
  Serial.print("Load cell:   ");
  Serial.print(loadCellVal, DEC);
  Serial.print("   FSR1: ");
  Serial.print(fsrLeft, DEC);
  Serial.print("   FSR2: ");
  Serial.print(fsrRight, DEC);
  Serial.print("   Diff: ");
  Serial.print((fsrRight-fsrLeft), DEC);
  Serial.print("   Pot1: ");
  Serial.print(throttlePot, DEC);
  Serial.print("   Pot2: ");
  Serial.print(handlebarPot, DEC);
  
  // Map sensor values to corresponding joystick resistances (on the hacked RC controller)
  pitchVal = constrain(map(loadCellVal, LOAD_CELL_MIN, LOAD_CELL_MAX, PITCH_MIN, PITCH_MAX), PITCH_MIN, PITCH_MAX);
  rollVal = constrain(map((fsrRight-fsrLeft), FSR_DIFF_MIN, FSR_DIFF_MAX, ROLL_MIN, ROLL_MAX), ROLL_MIN, ROLL_MAX);
  throttleVal = constrain(map(throttlePot, THROTTLE_POT_MIN, THROTTLE_POT_MAX, THROTTLE_MIN, THROTTLE_MAX), THROTTLE_MIN, THROTTLE_MAX);
  yawVal = constrain(map(handlebarPot, HANDLEBAR_POT_MIN, HANDLEBAR_POT_MAX, YAW_MIN, YAW_MAX), YAW_MIN, YAW_MAX);
  
  // Print digital pot values
  Serial.print("   Pitch: ");
  Serial.print(pitchVal, DEC);
  Serial.print("   Roll: ");
  Serial.print(rollVal, DEC);
  Serial.print("   Throttle: ");
  Serial.print(throttleVal, DEC);
  Serial.print("   Yaw: ");
  Serial.println(yawVal, DEC);
  
  // Turn on both wipers on each digital pot
  digitalPotWrite(POT1_SS, TCON_WRITE_ADDR, BOTH_WIPERS_ON);
  digitalPotWrite(POT2_SS, TCON_WRITE_ADDR, BOTH_WIPERS_ON);
  // Write the mapped resistance values to the digital potentiometers
  digitalPotWrite(POT1_SS, WIPER0_ADDR, throttleVal);
  digitalPotWrite(POT1_SS, WIPER1_ADDR, yawVal);
  digitalPotWrite(POT2_SS, WIPER0_ADDR, rollVal);
  digitalPotWrite(POT2_SS, WIPER1_ADDR, pitchVal);
}

// Method to write to the digital potentiometers
void digitalPotWrite(int pin, int addr, int val) {
  // Set the slave select pin low to select the chip
  digitalWrite(pin, LOW);
  // Send the address and value via SPI
  SPI.transfer(addr);
  SPI.transfer(val);
  // De-select the chip
  digitalWrite(pin, HIGH);
}

