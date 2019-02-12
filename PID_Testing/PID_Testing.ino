#include <AutoPID.h>

// Can Range from 0 to 255
#define robot_speed_min 100
#define robot_speed_max 255

#define KP 7
#define KI 5
#define KD 7

#define RF_encoder 3
#define LB_encoder 2

#define RF1 4
#define RF2 9
#define RF_PWM 6  //changed from 5

#define LB1 7
#define LB2 8
#define LB_PWM 5 //changed from 6

#define constantSpeed 140


volatile double countLB = 0;
volatile double countRF = 0;
volatile double stepsOverall = 0;
double stepsMotion = 0;
volatile double pwmLB, pwmRF = constantSpeed;
unsigned long previousMillis = 0;
const long interval = 1000;
volatile double temp; 

//AutoPID(double *input, double *setpoint, double *output, double outputMin, double outputMax, double Kp, double Ki, double Kd);


AutoPID LB_PID(&countLB, &countRF/*(temp = countRF+12)*/, &pwmLB, robot_speed_min, robot_speed_max, KP, KI, KD);
//AutoPID RF_PID(&countRF, &countLB, &pwmRF, robot_speed_min, robot_speed_max, KP, KI, KD);

void setup() {

  pinMode(RF1, OUTPUT);
  pinMode(RF2, OUTPUT);
  pinMode(LB1, OUTPUT);
  pinMode(LB2, OUTPUT);
  pinMode(RF_PWM, OUTPUT);
  pinMode(LB_PWM, OUTPUT);

  pinMode(RF_encoder, INPUT);
  pinMode(LB_encoder, INPUT);

  attachInterrupt(digitalPinToInterrupt(LB_encoder), encoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RF_encoder), encoderRight, RISING);

  LB_PID.setTimeStep(4);
  //RF_PID.setTimeStep(4);

  Serial.begin(9600);
  Serial2.begin(9600);

  delay(5000);

  digitalWrite(LB1, HIGH);
  digitalWrite(LB2, LOW);

  digitalWrite(RF1, LOW);
  digitalWrite(RF2, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();

  //RF_PID.run();
  LB_PID.run();

  analogWrite(LB_PWM, pwmLB);
  analogWrite(RF_PWM, pwmRF);

  Serial.print("L count: ");
  Serial.print(countLB);
  Serial.print(" R count: ");
  Serial.print(countRF);
  Serial.print(" L PWM: ");
  Serial.print(pwmLB);
  Serial.print(" R PWM: ");
  Serial.print(pwmRF);
  Serial.print(" stepsOverall: ");
  Serial.println(stepsOverall);

  Serial2.print("L count: ");
  Serial2.print(countLB);
  Serial2.print(" R count: ");
  Serial2.print(countRF);
  Serial2.print(" L PWM: ");
  Serial2.print(pwmLB);
  Serial2.print(" R PWM: ");
  Serial2.println(pwmRF);
  Serial2.print(" stepsOverall: ");
  Serial2.println(stepsOverall);

  if (stepsOverall >= 2500) {
    digitalWrite(LB1, LOW);
    digitalWrite(LB2, LOW);

    digitalWrite(RF1, LOW);
    digitalWrite(RF2, LOW);

    delay (100);
    
    countLB = 0;
    countRF = 0;
  }
}

void encoderLeft() {
  countLB++;
  stepsOverall = (countLB + countRF) / 2;
}
void encoderRight() {
  countRF++;
  stepsOverall = (countLB + countRF) / 2;
}
