
#define motor_ENA 8
#define motor_IN1 10
#define motor_IN2 11
#define motor_IN3 12
#define motor_IN4 13
#define motor_ENB 9

#define encoder_L_C1 3 // Base Motor # 1
#define encoder_R_C1 2 // Base Motor # 2

volatile double countLeft = 0;
volatile double countRight = 0;
volatile double stepsOverall = 0;

void encoderLeft() {
  countLeft++;
  stepsOverall = (countLeft + countRight) / 2;
}
void encoderRight() {
  countRight++;
  stepsOverall = (countLeft + countRight) / 2;
}

void printStatus() {
  Serial.print("; Count; L:");
  Serial.print(countLeft);
  Serial.print(", R:");
  Serial.print(countRight);
  Serial.print(", Steps; Overall:");
  Serial.println(stepsOverall);
}

void setup() {
  Serial.begin(9600);

  pinMode(motor_ENA, OUTPUT);
  pinMode(motor_IN1, OUTPUT);
  pinMode(motor_IN2, OUTPUT);
  pinMode(motor_IN3, OUTPUT);
  pinMode(motor_IN4, OUTPUT);
  pinMode(motor_ENB, OUTPUT);

  pinMode(encoder_L_C1, INPUT);
  pinMode(encoder_R_C1, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_L_C1), encoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_R_C1), encoderRight, RISING);

  digitalWrite(motor_IN1, HIGH);
  digitalWrite(motor_IN2, LOW);
  digitalWrite(motor_IN3, HIGH);
  digitalWrite(motor_IN4, LOW);
  analogWrite(motor_ENA, 255);
  analogWrite(motor_ENB, 255);
}

void loop() {
  if (stepsOverall < 2500)
    printStatus();
  else {
    digitalWrite(motor_IN1, HIGH);
    digitalWrite(motor_IN2, LOW);
    digitalWrite(motor_IN3, HIGH);
    digitalWrite(motor_IN4, LOW);
    analogWrite(motor_ENA, 0);
    analogWrite(motor_ENB, 0);
  }
}
