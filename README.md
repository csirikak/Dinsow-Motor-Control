# Dinsow Motor Control
Currently with this version, it is possible to control the mainboard using the Arduino, however the python version intended to replace the Arduino is not working. 

Instructions for flashing the mainboard can be found here:
https://github.com/NiklasFauth/hoverboard-firmware-hack/blob/master/README.md


You must use flash one of the .bin files, or compile the code in the hoverboard-firmware-hack directory in this repository if you wish to not have errors.

Note: Check the logic voltage before connecting anything to the Aduino, the color coding is sometimes reversed.

To wire the board and Arduino look for the UART-L cable (The one next to the buzzer) and connect the GND and RX pins to the corresponding pins on the Arduino (GND, 6 and 11).

The Arduino sketch has been configured for one board, uncomment the comments @ ln 425-426 to use two boards.

# Instructions for control:
```
All speeds fall within 1000, -1000.
Enable wheel: wheel ID + EN
Set speed limit: wheel ID + SP + desired speed limit
Set acceleration rate: wheel ID + AC + desired rate
Set desired speed: wheel ID + V + speed

Get speed limit: wheel ID + GLMT
Get last requested speed: wheel ID + LST
Get acceleration rate: wheel ID + GAC
Get last actual speed: wheel ID + GSP
Emergency stop: STOP
```
---

Code based on Segway_wantai2_with_serial_command_v3.ino - Program for control BLDC Motor created by Muchammad Sobrun Ibnu Atfal.

Copyright 2018 CT Asia Robotics CO., Ltd
