#include <AutoPID.h>

// Can Range from 0 to 255
#define robot_speed_min 100
#define robot_speed_max 155

#define KP 0.012
#define KI 0.03
#define KD 0.0001

#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9
#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6
#define EN_PIN_1 A0
#define EN_PIN_2 A1
#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3

#define encoder_L_C1 19 // Base Motor # 1
#define encoder_R_C1 18 // Base Motor # 2

#define Front 1
#define Rear 2
#define Right 3
#define Left 4

#define Stop 0
#define Start 1
#define CW 2
#define CCW 3

volatile unsigned int countLeft = 0;
volatile unsigned int countRight = 0;
volatile unsigned int stepsOverall = 0;
double stepsMotion = 0;
double pwmLeft, pwmRight;
int currentMotion = 0;
boolean stringComplete = false;

char Direction;
int LocomotionParameter;

String inputString = "";

//AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
AutoPID leftPID(&countLeft, &countRight, &pwmLeft, robot_speed_min, robot_speed_max, KP, KI, KD);
AutoPID rightPID(&countRight, &countLeft, &pwmRight, robot_speed_min, robot_speed_max, KP, KI, KD);

void serialEvent2() {                                   //Gets input from Tab ... Gets only locomotion command
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    inputString += inChar;                           // add it to the inputString:
    if (inChar == '#') {
      resetMotionSteps(); //Added by Hashir later
      stringComplete = true;
      countLeft = 0;      //added by Hashir later
      countRight = 0;     //Added by Hashir later
      decider();
    }
  }
}

void encoderLeft() {
  countLeft++;
  stepsOverall = (countLeft + countRight) / 2;
}

void encoderRight() {
  countRight++;
  stepsOverall = (countLeft + countRight) / 2;
}

void motor(int LeftRight, int pwm, int CWCCW) {
  //Serial2.print("Motors=");
  switch (LeftRight) {
    case Right:
      //Serial2.println("Right");
      switch (CWCCW) {
        case CW:
          digitalWrite(MOTOR_A1_PIN, LOW);
          digitalWrite(MOTOR_B1_PIN, HIGH);
          break;
        case CCW:
          digitalWrite(MOTOR_A1_PIN, HIGH);
          digitalWrite(MOTOR_B1_PIN, LOW);
          break;
      }
      //digitalWrite(motor_ENA,HIGH);
      analogWrite(PWM_MOTOR_1, pwm);
      break;
    case Left:
      //Serial2.println("Left");
      switch (CWCCW) {
        case CW:
          digitalWrite(MOTOR_A2_PIN, LOW);
          digitalWrite(MOTOR_B2_PIN, HIGH);
          break;
        case CCW:
          digitalWrite(MOTOR_A2_PIN, HIGH);
          digitalWrite(MOTOR_B2_PIN, LOW);
          break;
      }
      //digitalWrite(motor_ENB,HIGH);
      analogWrite(PWM_MOTOR_2, pwm);
      break;
  }
}

void stopMotion() {
  motor(Left, Stop, Stop);
  motor(Right, Stop, Stop);
  currentMotion = Stop;
  Serial2.println("Stop Motion");
}

void resetMotionSteps() {
  stepsMotion = stepsOverall;
}

void moveDirection(int Direction, double steps) {
  Serial2.print("Move direction: ");
  Serial2.print(Direction);
  Serial2.print(" ");
  Serial2.print(steps);
  Serial2.println(" steps");
  if (stepsOverall - stepsMotion < steps) {
    if (currentMotion == Direction) {
      rightPID.run();
      leftPID.run();
      switch (Direction) {
        case Front:
          motor(Right, pwmRight, CCW);
          motor(Left, pwmLeft, CW);
          break;
        case Rear:
          motor(Right, pwmRight, CW);
          motor(Left, pwmLeft, CCW);
          break;
        case Right:
          motor(Right, pwmRight, CW);
          motor(Left, pwmLeft, CW);
          break;
        case Left:
          motor(Right, pwmRight, CCW);
          motor(Left, pwmLeft, CCW);
          break;
      }
    }
    else {
      Serial2.println("else");
      currentMotion = Direction;
      resetMotionSteps();
    }
  }
  else {
    Serial2.println("Stop motion else");
    stopMotion();
  }
}

void turnLeft(double steps) {
  moveDirection(Left, steps);
}
void turnRight(double steps) {
  moveDirection(Right, steps);
}
void moveForward(double steps) {
  moveDirection(Front, steps);
}
void moveBackward(double steps) {
  moveDirection(Rear, steps);
}


void decider() {
  if (stringComplete) {
    Serial2.println(inputString);
    stopMotion();
    Direction = inputString.charAt(0); //eg. "F"_0200_090_045
    LocomotionParameter = (inputString.substring(2, 6)).toInt(); //eg. F_"0200"_090_045
    stringComplete = false;
    inputString = "";
  }
  switch (Direction) {
    case 'F':
      moveForward(LocomotionParameter);
      break;
    case 'B':
      moveBackward(LocomotionParameter);
      break;
    case 'R':
      turnRight(LocomotionParameter);
      break;
    case 'L':
      turnLeft(LocomotionParameter);
      break;
    //      movement(LocomotionParameter, Direction);
    case 'H':
      stopMotion();
      break;
    default:;
      //stopMotion();
  }
}
//void printStatus() {
//  Serial.print(motionText(currentMotion));
//  Serial.print("; Count; L:");
//  Serial.print(countLeft);
//  Serial.print(", R:");
//  Serial.print(countRight);
//  Serial.print(", Steps; Overall:");
//  Serial.print(stepsOverall);
//  Serial.print(", Steps; Motion:");
//  Serial.print(stepsMotion);
//  Serial.print(", PWM; L:");
//  Serial.print(pwmLeft);
//  Serial.print(", R:");
//  Serial.print(pwmRight);
//  Serial.println();
//}
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  //delay(1000);
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  digitalWrite(EN_PIN_1, HIGH);
  digitalWrite(EN_PIN_2, HIGH);

  pinMode(encoder_L_C1, INPUT);
  pinMode(encoder_R_C1, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_L_C1), encoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_R_C1), encoderRight, RISING);

  // If input is more than value mentioned below or above setpoint, PWM will be set to min or max respectively
  leftPID.setBangBang(10);
  rightPID.setBangBang(10);

  // PID update interval
  //  leftPID.setTimeStep(100);
  //  rightPID.setTimeStep(100);
  leftPID.setTimeStep(4);
  rightPID.setTimeStep(4);
}

void loop() {
  //  printStatus();
  decider();
  Serial.print("X: "); Serial.print(pwmRight); Serial.print("  ");
  Serial.print("Y: "); Serial.print(pwmLeft); Serial.print("  ");
  Serial.println("uT");

  //  Serial.print("X: "); Serial.print(countLeft); Serial.print("  ");
  //  Serial.print("Y: "); Serial.print(countRight); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(stepsOverall); Serial.print("  ");
  //  Serial.println("uT");
}
