/*
 * The encoders aren't working on high speeds.
 * Ab dekhtay hain kya hota hai.
*/


#define CHA 2
#define CHB 3

volatile int master_count2 = 0; // universal count
volatile int master_count3 = 0; // universal count
long int intflag;// interrupt status flag
long int previousmillis;

void setup() {
  Serial.begin(9600);
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  previousmillis = millis();
  attachInterrupt(0, flag2, RISING);
  attachInterrupt(1, flag3, RISING);
  // interrupt 0 digital pin 2 positive edge trigger
}

void flag2() {
  master_count2++;
}

void flag3() {
  master_count3++;
}

void loop() {
  // put your main code here, to run repeatedly:
  intflag = millis();
  if (intflag == previousmillis + 500) {
    Serial.print("2=");
    Serial.println(master_count2);
    Serial.print("3=");
    Serial.println(master_count3);
    intflag = millis();
    previousmillis = millis();
  }
}
