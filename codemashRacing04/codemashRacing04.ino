// pulsesensors
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>

// pin declarations
// motor pins: 4,7,8,9,10,12,A0,A1
int lightBarrierPins[] = {2, 3};
boolean lightBarrier[] = {false, false};
int trottlePins[] = {A4, A5};
int pulsePins[] = {A2, A3};                                                                 // the pins the heartrate sensors are connected to
int ledPins[] = {5,6};

void setup() {
  Serial.begin(250000);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
}

void loop() {
  pulseUpdate();  // updating the pulse sensors
  
  for (int i = 0; i < 2; i++) {
    Serial.print(lightBarrier[i]);Serial.print("\t");
    Serial.print(analogRead(trottlePins[i]));Serial.print("\t");
    lightBarrier[i]=false;
  }
  
  sendBPMs();
  
  
  
  Serial.println("\t");
  delay(10);
}

void carOn1() {
  lightBarrier[0]=true;
}

void carOn2() {
  lightBarrier[1]=true;
}
