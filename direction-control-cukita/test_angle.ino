#include <Wire.h>
#include <TM1650.h>

TM1650 d;
int incomingByte = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Wire.begin(); //Join the bus as master
  d.init();
}

void loop() {// send data only when you receive data:
  d.displayOn();
  d.setBrightness(TM1650_MIN_BRIGHT);

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    char line[] = "-5";
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    d.displayString(incomingByte);

  }
}
