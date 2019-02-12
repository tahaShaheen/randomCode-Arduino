#define encoder_L_C1 50 // Base Motor # 1
#define encoder_R_C1 51 // Base Motor # 2

volatile double countLeft = 0;
volatile double countRight = 0;
volatile double stepsOverall = 0;

void setup() {
  Serial1.begin(9600);
    
  pinMode(encoder_L_C1, INPUT);
  pinMode(encoder_R_C1, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_L_C1), encoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_R_C1), encoderRight, RISING);
}

void loop() {
  Serial1.print("countLeft: "); Serial1.println(countLeft);
  Serial1.print("countRight: "); Serial1.println(countRight);
  Serial1.print("stepsOverall: "); Serial1.println(stepsOverall);

  delay(1000);

}

void encoderLeft() {
  countLeft++;
  stepsOverall = (countLeft + countRight) / 2;
}
void encoderRight() {
  countRight++;
  stepsOverall = (countLeft + countRight) / 2;
}
