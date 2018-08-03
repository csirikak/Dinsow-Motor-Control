//Based on Segway_wantai2_with_serial_command_v3.ino - Program for control BLDC Motor created by Muchammad Sobrun Ibnu Atfal.
//CT Asia Robotics LTD. 2018

#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>

int sp1 = 0, sp2 = 0, sp3 = 0, sp4 = 0; //Sent speeds, signed integer range: -1000, 1000.
int spa1, spa2, spa3, spa4; //Last actual sent speeds, signed integer range: -1000, 1000.
int max1, max2, max3, max4; //Speed limit, unsigned integer range: 0, 1000.
uint8_t ac1, ac2, ac3, ac4; //Acceleration and deacceleration coefficient, integer range: 0, 255.
//ac and dec coefficients have been combined because ac and dec are mismatched when moving across 0 otherwise resulting in confusion.

uint8_t ID_int; //ID integer.

int speedMotor1, speedMotor2, speedMotor3, speedMotor4; //Set target speed, signed integer range: -1000, 1000.
bool reply1, reply2, reply3, reply4; //Ensures that replies are not repeated.
bool stringComplete;
String inputString;

SoftwareSerial board1(10, 11); //RX, TX (Connect to TX only)
SoftwareSerial board2(5, 6);

void setup() {
  Serial.begin(9600);
  board1.begin(19200);
   
}
void loop(void){

//----------------------------------------------------------------------------SERIAL CAPTURE  
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      stringComplete = false;
    }
  }
//----------------------------------------------------------------------------STRING PROCESSING  
  String ID = inputString.substring(0,1);
  ID_int = ID.toInt();
  if(stringComplete == true && ID_int >= 1 && ID_int <= 4){
    Serial.print("\n");
    Serial.print("OK");
    Serial.print("\n");
    if(ID_int >-1){
      Serial.print("ID : ");
      Serial.print(ID_int);
      Serial.print("\n");
      
    }
  }    
  
//----------------------------------------------------------------------------SPEED MANAGEMENT
    if (stringComplete == true && inputString.substring(1,2) == "V"){
      String ID = inputString.substring(0,1);
      String speedM = inputString.substring(2,7);
      int speedMotor = speedM.toInt();
      Serial.print("speed:  ");
      Serial.print(speedMotor);
      Serial.print("\n");
      if (ID.toInt() == 1) {
        if (speedMotor == sp1) {
          Serial.print("The requested speed equals the current speed");
          reply1 = true;       
        }
        if (abs(speedMotor) <= max1) {
          reply1 = false;
          speedMotor1 = speedMotor;
        }
        else {
          reply1 = true;
          Serial.print("The requested speed exceeds the speed limit!");
        }}
      if (ID.toInt() == 2) {
        if (speedMotor == sp2) {
          Serial.print("The requested speed equals the current speed");
          reply2 = true;
        }
        if (abs(speedMotor) <= max2) {
          reply2 = false;
          speedMotor2 = speedMotor;
        }
        else {
          reply2 = true;
          Serial.print("The requested speed exceeds the speed limit!");
        }}
      if (ID.toInt() == 3) {
        if (speedMotor == sp3) {
          Serial.print("The requested speed equals the current speed");
          reply3 = true;
        }
        if (abs(speedMotor) <= max3) {
          reply3 = false;
          speedMotor3 = speedMotor;
        }
        else {
          reply3 = true;
          Serial.print("The requested speed exceeds the speed limit!");
        }}
      if (ID.toInt() == 4) {
        if (speedMotor == sp4) {
          Serial.print("The requested speed equals the current speed");
          reply4 = true;
        }
        if (abs(speedMotor) <= max4) {
          reply4 = false;
          speedMotor4 = speedMotor;
        }
        else {
          reply4 = true;
          Serial.print("The requested speed exceeds the speed limit!");
        }}
    }

//---------------------------------------------------------------------------SPEED LIMIT PROCESSING
else if (inputString.substring(1,3) == "SP" && stringComplete == true){
  String speedM = inputString.substring(3,8);
  int speedMotor = speedM.toInt();
    switch (ID_int){
      case 1:
        max1 = speedMotor;
      break;
      case 2:
        max2 = speedMotor;
      break;
      case 3:
        max3 = speedMotor;
      break;
      case 4:
        max4 = speedMotor;
      break;
    }
  Serial.print("Set Max Speed: ");
  Serial.print(speedMotor);
  Serial.print("\n");
}
//--------------------------------------------------------------------------ENABLE
else if (inputString.substring(1,3) == "EN" && stringComplete == true){
  Serial.print("ENABLE ");
  Serial.print("\n");
}
//-------------------------------------------------------------------------DISABLE
else if (inputString.substring(1,3) == "DI" && stringComplete == true){
  Serial.print("DISABLE ");
  switch(ID_int) {
  case 1:
    sp1 = 0;
    speedMotor1 = 0;
    ac1 = 0;
    max1 = 0;
  case 2:
    sp2 = 0;
    speedMotor2 = 0;
    ac2 = 0;
    max2 = 0;
  case 3:
    sp3 = 0;
    speedMotor3 = 0;
    ac3 = 0;
    max3 = 0;
  case 4:
    sp4 = 0;
    speedMotor4 = 0;
    ac4 = 0;
    max4 = 0;
}}
//--------------------------------------------------------------------------AC PROCESSING
else if (inputString.substring(1,3) == "AC" && stringComplete == true){
  String speedM = inputString.substring(3,8);
  int speedMotor = speedM.toInt();
  Serial.print("ACCELERATION to: ");
  Serial.print(speedMotor);
  Serial.print("\n");
    switch (ID_int){
      case 1:
        ac1 = speedMotor;
      break;
      case 2:
        ac2 = speedMotor;
      break;
      case 3:
        ac3 = speedMotor;
      break;
      case 4:
        ac4 = speedMotor;
      break;
    }
}
//---------------------------------------------------------------------------------AC and DEC
  if (speedMotor1 > sp1) {
    if (speedMotor1 >= 0 && reply1 == false) {
      reply1 = true;
      Serial.print("Positive Acceleration");
      }
    if (speedMotor1 <= 0 && reply1 == false) {
      reply1 = true;
      Serial.print("Negative Deacceleration");
      }
    sp1 = sp1 + ac1;
    }
  if (speedMotor1 < sp1) {
    if (speedMotor1 >= 0 && reply1 == false) {
      reply1 = true;
      Serial.print("Positive Deacceleration");
      }
    if (speedMotor1 <= 0 && reply1 == false) {
      reply1 = true;
      Serial.print("Negative Acceleration");
      }
    sp1 = sp1 - ac1;
  }
  if (speedMotor2 > sp2) {
    if (speedMotor2 >= 0 && reply2 == false) {
      reply2 = true;
      Serial.print("Positive Acceleration");
      }
    if (speedMotor2 <= 0 && reply2 == false) {
      reply2 = true;
      Serial.print("Negative Deacceleration");
      }
    sp2 = sp2 + ac2;
    }
  if (speedMotor2 < sp2) {
    if (speedMotor2 >= 0 && reply2 == false) {
      reply2 = true;
      Serial.print("Positive Deacceleration");
      }
    if (speedMotor2 <= 0 && reply2 == false) {
      reply2 = true;
      Serial.print("Negative Acceleration");
      }
    sp2 = sp2 - ac2;
  }
  if (speedMotor3 > sp3) {
    if (speedMotor3 >= 0 && reply3 == false) {
      reply3 = true;
      Serial.print("Positive Acceleration");
      }
    if (speedMotor3 <= 0 && reply3 == false) {
      reply3 = true;
      Serial.print("Negative Deacceleration");
      }
    sp3 = sp3 + ac3;
    }
  if (speedMotor3 < sp3) {
    if (speedMotor3 >= 0 && reply3 == false) {
      reply3 = true;
      Serial.print("Positive Deacceleration");
      }
    if (speedMotor3 <= 0 && reply3 == false) {
      reply3 = true;
      Serial.print("Negative Acceleration");
      }
    sp3 = sp3 - ac3;
    }
  if (speedMotor4 > sp4) {
    if (speedMotor4 >= 0 && reply4 == false) {
      reply4 = true;
      Serial.print("Positive Acceleration");
      }
    if (speedMotor4 <= 0 && reply4 == false) {
      reply4 = true;
      Serial.print("Negative Deacceleration");
      }
    sp4 = sp4 + ac4;
    }
  if (speedMotor4 < sp4) {
    if (speedMotor4 >= 0 && reply4 == false) {
      reply4 = true;
      Serial.print("Positive Deacceleration");
      }
    if (speedMotor4 <= 0 && reply4 == false) {
      reply4 = true;
      Serial.print("Negative Acceleration");
      }
    sp4 = sp4 - ac4;   
  }
//------------------------------------------------------------------------ENSURE FULL STOP
if (speedMotor1 == 0 && sp1 == spa1) {
  sp1 = 0;
}
if (speedMotor2 == 0 && sp2 == spa2) {
  sp2 = 0;
}
if (speedMotor3 == 0 && sp3 == spa3) {
  sp3 = 0;
}
if (speedMotor4 == 0 && sp4 == spa4) {
  sp4 = 0;
}
//------------------------------------------------------------------------GET SPEED LIMIT
if (inputString.substring(1,5) == "GLMT"){
  Serial.print("\n");
    switch (ID_int){
      case 1:
        Serial.print("Max speed on wheel ID 1: ");
        Serial.print(max1);
      break;
      case 2:
        Serial.print("Max speed on wheel ID 2: ");
        Serial.print(max2);
      break;
      case 3:
        Serial.print("Max speed on wheel ID 3: ");
        Serial.print(max3);
      break;
      case 4:
        Serial.print("Max speed on wheel ID 4: ");
        Serial.print(max4);
      break;
    }
  Serial.print("\n");
}
//-------------------------------------------------------------------------GET LAST SPEED
if (inputString.substring(1,4) == "GSP"){
  Serial.print("\n");
    switch (ID_int){
      case 1:
        Serial.print("Last speed on wheel ID 1: ");
        Serial.print(speedMotor1);
      break;
      case 2:
        Serial.print("Last speed on wheel ID 2: ");
        Serial.print(speedMotor2);
      break;
      case 3:
        Serial.print("Last speed on wheel ID 3: ");
        Serial.print(speedMotor3);
      break;
      case 4:
        Serial.print("Last speed on wheel ID 4: ");
        Serial.print(speedMotor4);
      break;
    }
  Serial.print("\n");
}
//------------------------------------------------------------------------ACCELERATION COEFFICIENT
if (inputString.substring(1,4) == "GAC"){
    Serial.print("\n");
    switch (ID_int){
      case 1:
        Serial.print("Acceleration for wheel ID 1: ");
        Serial.print(ac1);
        Serial.print("\n");
      break;
      case 2:
        Serial.print("Acceleration for wheel ID 2: ");
        Serial.print(ac2);
        Serial.print("\n");
      break;
      case 3:
        Serial.print("Acceleration for wheel ID 3: ");
        Serial.print(ac3);
        Serial.print("\n");
      break;
      case 4:
        Serial.print("Acceleration for wheel ID 4: ");
        Serial.print(ac4);
        Serial.print("\n");
      break;
    }
}

//------------------------------------------------------------------------EMERGENCY STOP

if (inputString.substring(0,4) == "STOP") {
  Serial.print("\n");
  Serial.print("Emergency stop activated");
  Serial.print("\n");
  sp1 = 0;
  sp2 = 0;
  sp3 = 0;
  sp4 = 0;
  speedMotor1 = 0;
  speedMotor2 = 0;
  speedMotor3 = 0;
  speedMotor4 = 0;
}

//------------------------------------------------------------------------HELP
/*
if (inputString.substring(0,6) == "--help" || inputString.substring(0,1) == "?") {
  Serial.print("\n");
  Serial.print("To use a wheel, first set the speed limit by sending the wheel ID + SP + desired speed limit, e.g. 1SP300");
  Serial.print("\n");
  Serial.print("Next, set the desired acceleration by sending the wheel ID + AC + the desired acceleration coefficient e.g. 1AC10");
  Serial.print("\n");
  Serial.print("Note that the acceleration is number of speed increments per second * 20 e.g. if acceleration is 5, then speed will change by 100/s");
  Serial.print("\n");
  Serial.print("Finally to set the wheel speed send the wheel ID + V + desired speed, e.g. 1V250");
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("Type 'more help' for more help");
  Serial.print("\n");
}
if (inputString.substring(0,9) == "more help") {
  Serial.print("To get set speed limit use GSP, e.g. 1GSP");
  Serial.print("\n");
  Serial.print("To get last requested speed use LST, e.g. 1LST");
  Serial.print("\n");
  Serial.print("To get set acceleration coefficient use GAC, e.g. 1GAC");
  Serial.print("\n");
  Serial.print("For an emergency stop type STOP.");
  Serial.print("\n");
  Serial.print("To control the mainboard directly, use 19200 baud and send two 8 bit unsigned integers 20 times per second");
  Serial.print("\n");
  Serial.print("from range 1000, -1000 to control each wheel.");
  Serial.print("\n");
  Serial.print("Board firmware a modified version of the firmware found at: https://github.com/NiklasFauth/hoverboard-firmware-hack");
}
*/
//------------------------------------------------------------------------CLEAR INPUT
if (stringComplete == true) {
  stringComplete = false;
  inputString = "";
  }
delay(50);
//---------------------------------------------------------------------------SEND SPEED COMMAND
board1.write((uint8_t *) &sp1, sizeof(sp1));
board1.write((uint8_t *) &sp2, sizeof(sp2));
//board2.write((uint8_t *) &sp3, sizeof(sp3));
//board2.write((uint8_t *) &sp4, sizeof(sp4));
delay(50);
//---------------------------------------------------------------------------SET LAST SPEED IN MEMORY
spa1 = sp1;
spa2 = sp2;
spa3 = sp3;
spa4 = sp4;
}
