/*
      NITRO Clubs EU - Network of IcT Robo Clubs
 
 WEB site: https://www.nitroclubs.eu 
 GitHub repositories: https://github.com/nitroclubs?tab=repositories 
 
      NITRObot Line following using 5 line detection sensors

 NITRObot is equipped with different sensors and designed to let you simulate tasks from the industrial robotics world. 
 
 In this program, we will use NITRObot's line detecting sensors to perform the line following task 
 similarly to the way it is done in the big industrial robots.
 
 Find the detailed instructions in NITRObot_Line_following_5_sensors-Instructions_EN.docx
*/


//====== INCLUDE ======
#include <Arduino.h>


//====== DEFINE ======

// Comment the line bellow (put // in front of #define DEBUG statement) in order to Enable/Disable sending messages to the Serial port
// using the macros below.
#define DEBUG // When DEBUG is defined, it Enables the Debug mode, to Disable comment out this line

#ifdef DEBUG
// HERE WE DEFINE MACROS WHICH WILL ALLOW US TO DISABLE PRINTING TO THE SERIAL PORT
// WHEN THE ROBOT WILL NOT BE CONNECTED TO A PC FOR DEBUGGING (IF "DEBUG" IS NOT DEFINED ABOVE).  
// You will find detailed explanation in NITRObot_Line_following_5_sensors-Instructions_EN.odt
#define Debugprintln(a) (Serial.println(a))
#define Debugprint(a) (Serial.print(a))
#define Debugwrite(a) (Serial.write(a))
//Then instead of
//Serial.println("Hello world!");
//write
//Debugprintln("Hello world!");
//etc.
#else
// To deactivate the Serial printing, define the macro empty :
#define Debugprintln(a)
#define Debugprint(a)
#define Debugwrite(a)

#endif // end of #ifdef DEBUG


#define MOTOR_LEFT_FWD_PIN 9
#define MOTOR_LEFT_BKWD_PIN 5
#define MOTOR_RIGHT_FWD_PIN 6
#define MOTOR_RIGHT_BKWD_PIN 10

// Define Cytron Maker Line sensor connection pins
//   Arduino MEGA pin:                Sensor pin:
#define LN_SENS_PIN_RIGHTEDGE 22 // right edge sensor - Connected to D1 pin of the sensor
#define LN_SENS_PIN_RIGHT 25     // right sensor - Connected to D2 pin of the sensor
#define LN_SENS_PIN_MIDDLE 24    // middle sensor - Connected to D3 pin of the sensor
#define LN_SENS_PIN_LEFT 23      // left sensor Connected to D4 pin of the sensor
#define LN_SENS_PIN_LEFTEDGE 26  // left edge sensor - Connected to D5 pin of the sensor
#define LN_SENS_CALIB_PIN 27     // Connected to CAL pin of the sensor
#define LN_SENS_ANALOG_PIN A15   // Connected to AN pin of the sensor

//====== CONSTANTS ======
const int DefaultLeftSpeed =100;   // !Replace the value with the one obtained from calibration! (using NITRObot_motor_calibration.ino)
const int DefaultRightSpeed =100;  // !Replace the value with the one obtained from calibration! (using NITRObot_motor_calibration.ino)
//====== VARIABLES ======
int currentLeftSpeed = DefaultLeftSpeed;    // Variable to hold the left speed changes
int currentRightSpeed = DefaultRightSpeed;  // Variable to hold the right speed changes

int robotPosition = 0; // Variable to hold the position of the robot relative to the line

//====== FORWARD DECLARATIONS ======
void moveForward();  // function prototype includes return type, name, parameters, and semicolon.  No function body!
void moveBackward(); // function prototype includes return type, name, parameters, and semicolon.  No function body!
void turnLeft();     // function prototype includes return type, name, parameters, and semicolon.  No function body!
void turnRight();    // function prototype includes return type, name, parameters, and semicolon.  No function body!
void stopMoving();   // function prototype includes return type, name, parameters, and semicolon.  No function body!

//-----------------------------------------------

void setup()
{
  pinMode(LN_SENS_PIN_RIGHTEDGE, INPUT);
  pinMode(LN_SENS_PIN_RIGHT, INPUT);
  pinMode(LN_SENS_PIN_MIDDLE, INPUT);
  pinMode(LN_SENS_PIN_LEFT, INPUT);
  pinMode(LN_SENS_PIN_LEFTEDGE, INPUT);
  // pinMode(LN_SENS_CALIB_PIN, OUTPUT); // We will not use sensor calibration in this program
  pinMode(LN_SENS_ANALOG_PIN, INPUT);

  pinMode(MOTOR_LEFT_FWD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BKWD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BKWD_PIN, OUTPUT);

  #ifdef DEBUG          // Enable Serial port communication, Disable if "DEBUG" was not defined above
  Serial.begin(115200); // 
  #endif                //You will find detailed explanation in NITRObot_Line_following_5_sensors-Instructions_EN.odt
  
  delay(1000); //???????????????????????????????
}

//---------------------------------------------------------

void loop()
{
  // Read all sensors:
  int leftEdge = digitalRead(LN_SENS_PIN_RIGHTEDGE);
  int left = digitalRead(LN_SENS_PIN_RIGHT);
  int mid = digitalRead(LN_SENS_PIN_MIDDLE);
  int right = digitalRead(LN_SENS_PIN_LEFT);
  int rightEdge = digitalRead(LN_SENS_PIN_LEFTEDGE);

  if ((leftEdge == 0) && (left == 0) && (mid == 1) && (right == 0) && (rightEdge == 0))      //    0 0 1 0 0
  {
    robotPosition = 1;
    Debugprintln("case1");
  }
  else if ((leftEdge == 0) && (left == 1) && (mid == 1) && (right == 0) && (rightEdge == 0)) //   0 1 1 0 0
  {
    robotPosition = 2;
    Debugprintln("case2");
  }
  else if ((leftEdge == 0) && (left == 1) && (mid == 0) && (right == 0) && (rightEdge == 0)) //   0 1 0 0 0
  {
    robotPosition = 3;
    Debugprintln("case3");
  }
  else if ((leftEdge == 1) && (left == 1) && (mid == 0) && (right == 0) && (rightEdge == 0)) //   1 1 0 0 0
  {
    robotPosition = 4;
    Debugprintln("case4");
  }
  else if ((leftEdge == 1) && (left == 0) && (mid == 0) && (right == 0) && (rightEdge == 0)) //   1 0 0 0 0
  {
    robotPosition = 5;
    Debugprintln("case5");
  }
  else if ((leftEdge == 0) && (left == 0) && (mid == 1) && (right == 1) && (rightEdge == 0)) //   0 0 1 1 0
  {
    robotPosition = 6;
    Debugprintln("case6");
  }
  else if ((leftEdge == 0) && (left == 0) && (mid == 0) && (right == 1) && (rightEdge == 0)) //   0 0 0 1 0
  {
    robotPosition = 7;
    Debugprintln("case7");
  }
  else if ((leftEdge == 0) && (left == 0) && (mid == 0) && (right == 1) && (rightEdge == 1)) //   0 0 0 1 1
  {
    robotPosition = 8;
    Debugprintln("case8");
  }
  else if ((leftEdge == 0) && (left == 0) && (mid == 0) && (right == 0) && (rightEdge == 1)) //   0 0 0 0 1
  {
    robotPosition = 9;
    Debugprintln("case9");
  }
  else if (((leftEdge == 1) && (left == 1) && (mid == 0) && (right == 1) && (rightEdge == 1)) || // 1 1 0 1 1
           ((leftEdge == 1) && (left == 0) && (mid == 0) && (right == 1) && (rightEdge == 1)) || // 1 0 0 1 1
           ((leftEdge == 1) && (left == 1) && (mid == 0) && (right == 0) && (rightEdge == 1)) || // 1 1 0 0 1
           ((leftEdge == 1) && (left == 1) && (mid == 1) && (right == 0) && (rightEdge == 1)) || // 1 1 1 0 1
           ((leftEdge == 1) && (left == 0) && (mid == 1) && (right == 1) && (rightEdge == 1)))   // 1 0 1 1 1
  {
    robotPosition = 10;
  }
  else
  {
    moveForward();
  }

  switch (robotPosition)
  {
  case 1:
    currentLeftSpeed = DefaultLeftSpeed;
    currentRightSpeed = DefaultRightSpeed;
    moveForward();
    Debugprintln("1");
    break;
  case 2:
    currentLeftSpeed = DefaultLeftSpeed * .6;
    currentRightSpeed = 180;
    moveForward();
    Debugprintln("2");
    break;
  case 3:
    currentLeftSpeed = DefaultLeftSpeed * .4;
    currentRightSpeed = 200;
    moveForward();
    Debugprintln("3");
    break;
  case 4:
    currentLeftSpeed = DefaultLeftSpeed * .3;
    currentRightSpeed = 220;
    Debugprintln("4");
    moveForward();
    break;
  case 5:
    currentLeftSpeed = DefaultLeftSpeed * .2;
    currentRightSpeed = 255;
    moveForward();
    Debugprintln("5");
    break;
  case 6:
    currentLeftSpeed = 180;
    currentRightSpeed = DefaultRightSpeed * .6;
    moveForward();
    Debugprintln("6");
    break;
  case 7:
    currentLeftSpeed = 200;
    currentRightSpeed = DefaultRightSpeed * .4;
    moveForward();
    Debugprintln("7");
    break;
  case 8:
    currentLeftSpeed = 220;
    currentRightSpeed = DefaultRightSpeed * .3;
    moveForward();
    Debugprintln("8");
    break;
  case 9:
    currentLeftSpeed = 255;
    currentRightSpeed = DefaultRightSpeed * .2;
    moveForward();
    Debugprintln("9");
    break;
  case 10:
    stopMoving();
    Debugprintln("10");
    break;
  default:
    break;
  }
}

//============== FUNCTION DEFINITIONS ==============

void moveForward() // Move forward
{
  analogWrite(MOTOR_LEFT_FWD_PIN, abs(currentLeftSpeed));
  analogWrite(MOTOR_LEFT_BKWD_PIN, LOW);
  analogWrite(MOTOR_RIGHT_FWD_PIN, abs(currentRightSpeed));
  analogWrite(MOTOR_RIGHT_BKWD_PIN, LOW);
}

void moveBackward() // Move backward
{
  analogWrite(MOTOR_LEFT_FWD_PIN, LOW);
  analogWrite(MOTOR_LEFT_BKWD_PIN, abs(currentLeftSpeed));
  analogWrite(MOTOR_RIGHT_FWD_PIN, LOW);
  analogWrite(MOTOR_RIGHT_BKWD_PIN, abs(currentRightSpeed));
}

void turnLeft() // Turn Left
{
  analogWrite(MOTOR_LEFT_FWD_PIN, LOW);
  analogWrite(MOTOR_LEFT_BKWD_PIN, currentLeftSpeed);
  analogWrite(MOTOR_RIGHT_FWD_PIN, currentLeftSpeed);
  analogWrite(MOTOR_RIGHT_BKWD_PIN, LOW);
}

void turnRight() // Turn Right
{
  analogWrite(MOTOR_LEFT_FWD_PIN, currentRightSpeed);
  analogWrite(MOTOR_LEFT_BKWD_PIN, LOW);
  analogWrite(MOTOR_RIGHT_FWD_PIN, LOW);
  analogWrite(MOTOR_RIGHT_BKWD_PIN, currentRightSpeed);
}

void stopMoving() // Stop movement
{
  analogWrite(MOTOR_LEFT_FWD_PIN, HIGH);
  analogWrite(MOTOR_LEFT_BKWD_PIN, HIGH);
  analogWrite(MOTOR_RIGHT_FWD_PIN, HIGH);
  analogWrite(MOTOR_RIGHT_BKWD_PIN, HIGH);
}
