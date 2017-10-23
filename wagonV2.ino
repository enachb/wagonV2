#include <debug.h>
#include <makros.h>
#include <RCReceive.h>

#include <SoftwareSerial9.h>

//#define MOSI 11
//#define MISO 12
//#define TX MOSI
//#define RX MISO
//#define LEDPIN 13

#define TXL 4
#define RXL 5

#define TXR 2
#define RXR 3

#define led 13

SoftwareSerial9 mySerialL(RXL, TXL);
SoftwareSerial9 mySerialR(RXR, TXR);
RCReceive rcReceiver;

const byte PIN_RC = 7;

void setup() {
  pinMode(led, OUTPUT);
  mySerialL.begin(26315);
  mySerialR.begin(26315);
  Serial.begin(115200);
  rcReceiver.attach(PIN_RC);
}

char c = ' ';
signed int sp = 0;

void loop() {

  rcReceiver.poll();

  if (rcReceiver.hasNP() && !rcReceiver.hasError()) {

    byte value = rcReceiver.getValue();
    
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

    Serial.print("receiver ");
    Serial.println(value);
    
    Serial.print("speed ");
    Serial.println(sp);
    Serial.print(" low byte ");
    Serial.print((sp & 0xFF), HEX);
    Serial.print(" high byte ");
    Serial.println((sp >> 8) & 0xFF, HEX);
    do {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      mySerialL.write9(256);
      mySerialL.write9(sp & 0xFF);
      mySerialL.write9((sp >> 8) & 0xFF);
      mySerialL.write9(sp & 0xFF);
      mySerialL.write9((sp >> 8) & 0xFF);
      mySerialL.write9(85);

      mySerialR.write9(256);
      mySerialR.write9(sp & 0xFF);
      mySerialR.write9((sp >> 8) & 0xFF);
      mySerialR.write9(sp & 0xFF);
      mySerialR.write9((sp >> 8) & 0xFF);
      mySerialR.write9(85);

      delayMicroseconds(300);
      digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
    } while (!Serial.available());
    c = Serial.read();
  } else if (rcReceiver.hasError()) {
    // Fehlerbehandlung failsafe oder sowas...
  }
}
