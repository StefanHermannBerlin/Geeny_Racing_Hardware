// include the library code:
#include <LiquidCrystal.h>

// pulsesensors
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;
const int PULSE_SENSOR_COUNT = 2;

const int PIN_INPUT0 = A0;
const int PIN_BLINK0 = 13;    // Pin 13 is the on-board LED

const int PIN_INPUT1 = A1;
const int PIN_BLINK1 = 12;

const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;
PulseSensorPlayground pulseSensor(PULSE_SENSOR_COUNT);

// LCD stuff
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long time1 = 0;
long time2 = 0;

// config
int sensorPin1 = 2;
int sensorPin2 = 3;

long lcdTimeout;
long myTimer1 = 0;
long myTimer2 = 0;
int timeout = 100;
int sensorTimeout = 300;

int potiPin1 = 0;
int potiPin2 = 1;

int outputPin1 = 10;
int outputPin2 = 11;

void setup() {
  Serial.begin(250000);

  pulseSensor.analogInput(PIN_INPUT0, 0);
  pulseSensor.blinkOnPulse(PIN_BLINK0, 0);

  pulseSensor.analogInput(PIN_INPUT1, 1);
  pulseSensor.blinkOnPulse(PIN_BLINK1, 1);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
    /*
       PulseSensor initialization failed,
       likely because our particular Arduino platform interrupts
       aren't supported yet.

       If your Sketch hangs here, try ProcessEverySample.ino,
       which doesn't use interrupts.
    */
  }

  lcd.begin(16, 2);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin1), carOn1, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensorPin2), carOn2, HIGH);
}

void loop() {
  if (pulseSensor.sawNewSample()) {

    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      //pulseSensor.outputSample();

      /*
         If a beat has happened on a given PulseSensor
         since we last checked, write the per-beat information
         about that PulseSensor to Serial.
      */

      for (int i = 0; i < PULSE_SENSOR_COUNT; ++i) {
        if (pulseSensor.sawStartOfBeat(i)) {
          pulseSensor.outputBeat(i);
          Serial.print("Beat: ");Serial.println(i);
        }
      }
    }

    lcdOutput();
    analogWrite(outputPin1, map(analogRead(potiPin1), 0, 1023, 0, 255));
    analogWrite(outputPin2, map(analogRead(potiPin2), 0, 1023, 0, 255));

  }
}

void lcdOutput() {
  lcd.setCursor(0, 0);
  lcd.print(time1);

  lcd.setCursor(0, 1);
  lcd.print(time2);

  lcd.setCursor(8, 0);
  lcd.print(millis() - myTimer1);

  lcd.setCursor(8, 1);
  lcd.print(millis() - myTimer2);

  if (lcdTimeout < millis() - 50) {
    lcd.clear();
    lcdTimeout = millis();
  }
}

void carOn1() {
  if (myTimer1 + sensorTimeout < millis()) {
    time1 = millis() - myTimer1;
    myTimer1 = millis();
  }
}

void carOn2() {
  if (myTimer2 + sensorTimeout < millis()) {
    time2 = millis() - myTimer2;
    myTimer2 = millis();
  }
}
