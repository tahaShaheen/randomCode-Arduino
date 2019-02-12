void(* resetFunc) (void) = 0;

#include <AutoPID.h>

#define encoder_L_C1 2 // Base Motor # 1
#define encoder_R_C1 3 // Base Motor # 2

#define BRAKE 0
#define FRONT 1
#define REVERSE 2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6

#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3

#define EN_PIN_1 A0
#define EN_PIN_2 A1

#define MOTOR_1 0
#define MOTOR_2 1

double usSpeedL = 0, usSpeedR = 0;  //default motor speed
unsigned short usMotor_Status = BRAKE;

volatile double countLeft = 0;
volatile double countRight = 0;
volatile double stepsOverall = 0;

double KP = 100;
double KI = 0.0;
double KD = 0.0;

//AutoPID(double *input, double *setpoint, double *output, double outputMin, double outputMax, double Kp, double Ki, double Kd);

//AutoPID leftPID(&countLeft, &countRight, &usSpeedL, 0, 255, KP, KI, KD);
AutoPID rightPID(&countRight, &countLeft, &usSpeedR, 0, 255, KP, KI, KD);

void encoderLeft() {
  countLeft++;
  //stepsOverall = (countLeft + countRight) / 2;
}

void encoderRight() {
  countRight++;
  //stepsOverall = (countLeft + countRight) / 2;
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(encoder_L_C1, INPUT_PULLUP);
  pinMode(encoder_R_C1, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoder_L_C1), encoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_R_C1), encoderRight, RISING);

  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);

  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  Serial2.begin(9600);              // Initiates the serial to do the monitoring
  Serial2.println("Begin motor control");
  Serial2.println(); //Print function list for user selection
  Serial2.println("Enter number for control option:");
  Serial2.println("1. STOP");
  Serial2.println("2. FORWARD");
  Serial2.println("3. REVERSE");
  Serial2.println("+. INCREASE SPEED");
  Serial2.println("-. DECREASE SPEED");
  Serial2.println("R. RESET");
  Serial2.println("F. FETCH VALUES OF PWM");
  Serial2.println();

}

void loop()
{
  Serial.print("X: "); Serial.print(countLeft); Serial.print("  ");
  Serial.print("Y: "); Serial.print(countRight); Serial.print("  ");
  Serial.print("Z: "); Serial.print(stepsOverall); Serial.print("  ");
  Serial.println("uT");

  char user_input;

  while (Serial2.available())
  {
    user_input = Serial2.read(); //Read user input and trigger appropriate function
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(EN_PIN_2, HIGH);

    if (user_input == '1')
    { rightPID.run();
      Stop();
    }
    else if (user_input == '2')
    { rightPID.run();
      Forward();
    }
    else if (user_input == '3')
    { rightPID.run();
      Reverse();
    }
    else if (user_input == '+')
    { rightPID.run();
      usSpeedL = usSpeedL + 10;
      IncreaseSpeed();
    }
    else if (user_input == '-')
    { rightPID.run();
      usSpeedL = usSpeedL - 10;
      DecreaseSpeed();
    }
    //    else if (user_input == ')')
    //    {
    //      usSpeedR = usSpeedR + 10;
    //      IncreaseSpeed();
    //    }
    //    else if (user_input == '(')
    //    {
    //      usSpeedR = usSpeedR - 10;
    //      DecreaseSpeed();
    //    }
    else if (user_input == 'R')
    {
      resetFunc();
    }
    else if (user_input == 'F')
    {
      Serial2.print("Speed L: ");
      Serial2.println(usSpeedL);
      Serial2.print("Speed R: ");
      Serial2.println(usSpeedR);
    }
    else
    {
      Serial2.println("Invalid option entered.");
    }

  }
}

void Stop()
{
  Serial2.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void Forward()
{
  Serial2.println("Forward");
  usMotor_Status = FRONT;
  motorGo(MOTOR_1, usMotor_Status, usSpeedL);
  motorGo(MOTOR_2, usMotor_Status, usSpeedR);
}

void Reverse()
{
  Serial2.println("Reverse");
  usMotor_Status = REVERSE;
  motorGo(MOTOR_1, usMotor_Status, usSpeedL);
  motorGo(MOTOR_2, usMotor_Status, usSpeedR);
}

void IncreaseSpeed()
{
  if (usSpeedL > 191)
  {
    usSpeedL = 191;
  }
  if (usSpeedR > 255)
  {
    usSpeedR = 255;
  }
  Serial2.print("Speed L: ");
  Serial2.println(usSpeedL);
  Serial2.print("Speed R: ");
  Serial2.println(usSpeedR);

  motorGo(MOTOR_1, usMotor_Status, usSpeedL);
  motorGo(MOTOR_2, usMotor_Status, usSpeedR);
}

void DecreaseSpeed()
{
  if (usSpeedL < 0)
  {
    usSpeedL = 0;
  }
  if (usSpeedR < 0)
  {
    usSpeedR = 0;
  }
  Serial2.print("Speed L: ");
  Serial2.println(usSpeedL);
  Serial2.print("Speed R: ");
  Serial2.println(usSpeedR);

  motorGo(MOTOR_1, usMotor_Status, usSpeedL);
  motorGo(MOTOR_2, usMotor_Status, usSpeedR);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (FRONT ou REVERSE) e pwm (entra 0 e 255);
{
  if (motor == MOTOR_1)
  {
    if (direct == FRONT)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }
    else if (direct == REVERSE)
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_1, pwm);
  }
  else if (motor == MOTOR_2)
  {
    if (direct == FRONT)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if (direct == REVERSE)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_2, pwm);
  }
}


