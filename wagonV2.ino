#include <debug.h>
#include <makros.h>
#include <RCReceive.h>

#include <SoftwareSerial9.h>

//#define MOSI 11
//#define MISO 12
//#define TX MOSI
//#define RX MISO
//#define LEDPIN 13

#define TXL 5
#define RXL 6

#define TXR 3
#define RXR 4

#define led 13

SoftwareSerial9 mySerialL(RXL, TXL);
//SoftwareSerial9 mySerialR(RXR, TXR);
RCReceive rcReceiver;

const byte PIN_RC = 12;
int i = 0;
byte value = 127;
signed int mapVal = 0;

void setup() {
  pinMode(led, OUTPUT);
  mySerialL.begin(26315);
  //  mySerialR.begin(26315);
  Serial.begin(115200);
  rcReceiver.attach(PIN_RC);

}

void loop() {

  if (rcReceiver.hasNP()) {
    rcReceiver.poll();
    value = rcReceiver.getValue();
  }

  //    signed int mapVal = map(value, 32, 225, -1550, 1550);

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

  if (i % 10 == 0) {
    rcReceiver.poll();
    value = rcReceiver.getValue();
    mapVal = map(value, 0, 250, -500, 500);

    //    long value = pulseIn(PIN_RC, HIGH);

    Serial.print("mapVal ");
    Serial.print(mapVal);

    Serial.print("   value ");
    Serial.println(value);
    i = 0;
  }
  i ++;

  sendToMotors(mapVal, mapVal);

  delayMicroseconds(300);
  //delay(50);
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)

  if (!rcReceiver.hasNP() && rcReceiver.hasError()) {
    Serial.println("****** ERROR");;
  }
}

void sendToMotors(signed int motorL, signed int motorR) {

  mySerialL.write9(256);
  mySerialL.write9(motorL & 0xFF);
  mySerialL.write9((motorL >> 8) & 0xFF);
  mySerialL.write9(motorL & 0xFF);
  mySerialL.write9((motorL >> 8) & 0xFF);
  mySerialL.write9(85);

  //  mySerialR.write9(256);
  //  mySerialR.write9(motorR & 0xFF);
  //  mySerialR.write9((motorR >> 8) & 0xFF);
  //  mySerialR.write9(motorR & 0xFF);
  //  mySerialR.write9((motorR >> 8) & 0xFF);
  //  mySerialR.write9(85);

}

/*

    Serial.println(c);
    if (c == ' ') {
      sp = 0;
    } else if (c == 'q') {
      sp -= 10;
    } else if (c == 'w') {
      sp += 10;
    } else if (c == '2') {
      sp += 100;
    }  else if (c == '1') {
      sp -= 100;
    }

    if (false) {
      Serial.print("mapVal ");
      Serial.println(mapVal);

      Serial.print("receiver ");
      Serial.println(value);

      Serial.print("speed ");
      Serial.println(sp);
      Serial.print(" low byte ");
      Serial.print((sp & 0xFF), HEX);
      Serial.print(" high byte ");
      Serial.println((sp >> 8) & 0xFF, HEX);
    }

 */
