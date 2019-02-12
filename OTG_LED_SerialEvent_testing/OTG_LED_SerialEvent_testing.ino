/*
  This is a Serial Event working code.
  Sends and receives code perfectly.
  Perfect code
*/

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString == "ONt")
      digitalWrite(LED_BUILTIN, HIGH);
    else if (inputString == "OFFt")
      digitalWrite(LED_BUILTIN, LOW);
    else if (inputString == "Ahmedt") {
      while (inputString == "Ahmedt") {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        serialEvent();
      }
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  //delay(300); /*added this because the phone had trouble earlier, leaving it here for that reason*/
}

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/

void serialEvent() {
  //  stringComplete=false;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:

    if (inChar == 't') { //this is the terminator. You can chage this to "\n" or whatever.
      stringComplete = true;
    }
  }
}
