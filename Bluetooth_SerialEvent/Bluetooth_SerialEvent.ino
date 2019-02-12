/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";         // a String to hold incoming data
String inputString1 = "";         // a String to hold incoming data
String inputString2 = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean string1Complete = false;  // whether the string is complete
boolean string2Complete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial2.begin(9600);
  Serial1.begin(9600);
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  inputString1.reserve(200);
  inputString2.reserve(200);
}

void loop() {
  // print the string when a newline arrives:

  if (stringComplete) {
    Serial2.print("Serial 0: ");
    Serial1.print("Serial 0: ");
    Serial.print("Serial 0: ");
    Serial2.println(inputString);
    Serial1.println(inputString);
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  if (string1Complete) {
    Serial2.print("Serial 1: ");
    Serial1.print("Serial 1: ");
    Serial.print("Serial 1: ");
    Serial2.println(inputString1);
    Serial1.println(inputString1);
    Serial.println(inputString1);
    inputString1 = "";
    string1Complete = false;
  }

  if (string2Complete) {
    Serial2.print("Serial 2: ");
    Serial1.print("Serial 2: ");
    Serial.print("Serial 2: ");
    Serial2.println(inputString2);
    Serial1.println(inputString2);
    Serial.println(inputString2);
    inputString2 = "";
    string2Complete = false;
  }
}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString1 += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      string1Complete = true;
    }
  }
}
void serialEvent2() {
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString2 += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      string2Complete = true;
    }
  }
}
