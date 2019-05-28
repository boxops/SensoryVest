#include <ShiftRegister74HC595.h>
#include <elapsedMillis.h>

int numberOfShiftRegisters = 2; //number of shift registers attached in series
int serialDataPin = 2; //DS
int clockPin = 4; //SHCP
int latchPin = 3; //STCP
ShiftRegister74HC595 sr (numberOfShiftRegisters, serialDataPin, clockPin, latchPin);

elapsedMillis timeElapsed;

const int sensorPins[4] = {6, 7, 8, 9};
boolean sensors[4] = {0, 0, 0, 0};
int sensorCounts[4] = {0, 0, 0, 0};
int maxValue = 0;
int maxIndex = 0;
unsigned int interval = 400;
const int activation = 50;
const int tones[3] = {90, 60, 30};
const int duration = 100;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(sensorPins[i], INPUT); //set pin as input
  }
  sr.setAllHigh(); //set all pins HIGH (off)
}

void loop() {
  //record sensor inputs per interval and store them in sensorCounts array
  while (timeElapsed < interval) {
    for (int i = 0; i < 4; i++) {
      sensors[i] = digitalRead(sensorPins[i]);
    }
    if (sensors[0] == LOW) {
      sensorCounts[0] = sensorCounts[0] + 1;
      delay(1);
    }
    else if (sensors[1] == LOW) {
      sensorCounts[1] = sensorCounts[1] + 1;
      delay(1);
    }
    else if (sensors[2] == LOW) {
      sensorCounts[2] = sensorCounts[2] + 1;
      delay(1);
    }
    else if (sensors[3] == LOW) {
      sensorCounts[3] = sensorCounts[3] + 1;
      delay(1);
    }
  }

  //find the maximum sensor value
  for (int i = 0; i < 4; i++) {
    if (sensorCounts[i] > maxValue) {
      maxValue = sensorCounts[i];
      maxIndex = i;
    }
  }

  //useful serial feedback when adjusting potentiometer on sensors
  Serial.println("Sensor 1");
  Serial.println(sensorCounts[0]);
  Serial.println("Sensor 2");
  Serial.println(sensorCounts[1]);
  Serial.println("Sensor 3");
  Serial.println(sensorCounts[2]);
  Serial.println("Sensor 4");
  Serial.println(sensorCounts[3]);

  //find highest recorded sensor values and compare them to activation var
  if (timeElapsed > interval) {
    if (sensorCounts[0] == sensorCounts[maxIndex]) {
      if (maxValue > activation + tones[0]) {
        //activate 4 motors
        Serial.println("Sensor 1 - 4 Motors On");
        motorControl4(4, 3, 2, 1);
      }
      else if (maxValue > activation + tones[1]) {
        //activate 3 motors
        Serial.println("Sensor 1 - 3 Motors On");
        motorControl3(4, 3, 2);
      }
      else if (maxValue > activation + tones[2]) {
        //activate 2 motors
        Serial.println("Sensor 1 - 2 Motors On");
        motorControl2(4, 3);
      }
      else if (maxValue > activation) {
        //activate one motor
        Serial.println("Sensor 1 - 1 Motor On");
        motorControl1(4);
      }
    }
    else if (sensorCounts[1] == sensorCounts[maxIndex]) {
      if (maxValue > activation + tones[0]) {
        //activate 4 motors
        Serial.println("Sensor 2 - 4 Motors On");
        motorControl4(5, 0, 7, 6);
      }
      else if (maxValue > activation + tones[1]) {
        //activate 3 motors
        Serial.println("Sensor 2 - 3 Motors On");
        motorControl3(5, 0, 7);
      }
      else if (maxValue > activation + tones[2]) {
        //activate 2 motors
        Serial.println("Sensor 2 - 2 Motors On");
        motorControl2(5, 0);
      }
      else if (maxValue > activation) {
        //activate one motor
        Serial.println("Sensor 2 - 1 Motor On");
        motorControl1(5);
      }
    }
    else if (sensorCounts[2] == sensorCounts[maxIndex]) {
      if (maxValue > activation + tones[0]) {
        //activate 4 motors
        Serial.println("Sensor 3 - 4 Motors On");
        motorControl4(9, 10, 11, 12);
      }
      else if (maxValue > activation + tones[1]) {
        //activate 3 motors
        Serial.println("Sensor 3 - 3 Motors On");
        motorControl3(9, 10, 11);
      }
      else if (maxValue > activation + tones[2]) {
        //activate 2 motors
        Serial.println("Sensor 3 - 2 Motors On");
        motorControl2(9, 10);
      }
      else if (maxValue > activation) {
        //activate one motor
        Serial.println("Sensor 3 - 1 Motor On");
        motorControl1(9);
      }
    }
    else if (sensorCounts[3] == sensorCounts[maxIndex]) {
      if (maxValue > activation + tones[0]) {
        //activate 4 motors
        Serial.println("Sensor 4 - 4 Motors On");
        motorControl4(13, 14, 15, 8);
      }
      else if (maxValue > activation + tones[1]) {
        //activate 3 motors
        Serial.println("Sensor 4 - 3 Motors On");
        motorControl3(13, 14, 15);
      }
      else if (maxValue > activation + tones[2]) {
        //activate 2 motors
        Serial.println("Sensor 4 - 2 Motors On");
        motorControl2(13, 14);
      }
      else if (maxValue > activation) {
        //activate one motor
        Serial.println("Sensor 4 - 1 Motor On");
        motorControl1(13);
      }
    }
    else {
      Serial.println("No input");
    }
  }

  //reset values and loop again
  if (timeElapsed > interval) {
    for (int i = 0; i < 4; i++) {
      sensorCounts[i] = 0;
    }
    timeElapsed = 0;
    maxValue = 0;
    maxIndex = 0;
  }
}

void motorControl1(int i) {
  sr.set(i, LOW); // set single pin LOW
  delay(duration);
  sr.set(i, HIGH); // set single pin HIGH
}

void motorControl2(int i, int j) {
  sr.set(i, LOW); // set single pin LOW
  delay(duration);
  sr.set(j, LOW); // set single pin LOW
  delay(duration);
  sr.set(i, HIGH); // set single pin HIGH
  sr.set(j, HIGH); // set single pin HIGH
}

void motorControl3(int i, int j, int k) {
  sr.set(i, LOW); // set single pin LOW
  delay(duration);
  sr.set(j, LOW); // set single pin LOW
  delay(duration);
  sr.set(k, LOW); // set single pin LOW
  delay(duration);
  sr.set(i, HIGH); // set single pin HIGH
  sr.set(j, HIGH); // set single pin HIGH
  sr.set(k, HIGH); // set single pin HIGH
}

void motorControl4(int i, int j, int k, int l) {
  sr.set(i, LOW); // set single pin LOW
  delay(duration);
  sr.set(j, LOW); // set single pin LOW
  delay(duration);
  sr.set(k, LOW); // set single pin LOW
  delay(duration);
  sr.set(l, LOW); // set single pin LOW
  delay(duration);
  sr.set(i, HIGH); // set single pin HIGH
  sr.set(j, HIGH); // set single pin HIGH
  sr.set(k, HIGH); // set single pin HIGH
  sr.set(l, HIGH); // set single pin HIGH
}

/*
  //ShiftRegister74HC595.h library content

  // setting all pins at the same time to either HIGH or LOW
  sr.setAllHigh(); // set all pins HIGH
  delay(500);

  sr.setAllLow(); // set all pins LOW
  delay(500);


  // setting single pins
  for (int i = 0; i < 8; i++) {

    sr.set(i, HIGH); // set single pin HIGH
    delay(250);
  }


  // set all pins at once
  uint8_t pinValues[] = { B10101010 };
  sr.setAll(pinValues);
  delay(1000);


  // read pin (zero based, i.e. 6th pin)
  uint8_t stateOfPin5 = sr.get(5);


  // set pins without immediate update
  sr.setNoUpdate(0, HIGH);
  sr.setNoUpdate(1, LOW);
  // at this point of time, pin 0 and 1 did not change yet
  sr.updateRegisters(); // update the pins to the set values

  //List of all functions
  ShiftRegister74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
  void setAll(uint8_t * digitalValues);
  uint8_t * getAll();
  void set(int pin, uint8_t value);
  void setAllLow();
  void setAllHigh();
  uint8_t get(int pin);
*/
