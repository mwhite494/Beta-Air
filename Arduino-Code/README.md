Pilot_Interface.ino - This program is run on the Arduino located underneath the gas tank and handles reading sensor values
and transmitting flight controls to the quadcopter

Quadcopter.ino - This program is run on the Arduino located on the quadcopter and transmits acceleration data.

Simulator_Platform.ino - This program is receives acceleration data from the quadcopter and displays it on a serial monitor. Here is where motion cueing would be implemented to send direction and step signals to the three DC stepper motors located on the simulator platform.
