#include <AFMotor.h>
AF_DCMotor motor(4);

#define READSOILPIN A0
#define MEASURE_DELAY 500
#define TRIGGER_COUNT 10

#define POTI_PIN A5

#define MIN_SOIL 20
#define MAX_SOIL 90
#define MAX_DIFF_SOIL 8
#define LED_BLUE 2
#define LED_GREEN 8
#define LED_YELLOW 32
#define LED_RED 128

float showBlue = 78;
float showGreen = 72;
float showYellow = 66;
float showRed = 60;

int latchPin = A2; // (11) ST_CP [RCK] on 74HC595
int clockPin = A1; // (9) SH_CP [SCK] on 74HC595
int dataPin = A3;  // (12) DS [S1] on 74HC595
byte leds = 0;

bool triggerMotor = true;
int triggerMotorCount = TRIGGER_COUNT;


void setup() {
  Serial.begin(9600);
  pinMode(READSOILPIN, INPUT);
  pinMode(POTI_PIN, INPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  motor.run(RELEASE);
}

void loop() {
  int sensorValue = analogRead(READSOILPIN);
  float percent = 100 - (sensorValue / 10.23);
  initLedsValues();

  if (percent >= showBlue) {
    Serial.print("showBlue - "); Serial.println(percent);
    if (!triggerMotor) {
      stopWatering();
    }
    leds = LED_BLUE;
  } else if (percent >= showGreen) {
    Serial.print("showGreen - "); Serial.println(percent);
    if (!triggerMotor) {
      stopWatering();
    }
    leds = LED_GREEN;
  } else if (percent >= showYellow) {
    Serial.print("showYellow - "); Serial.println(percent);
    leds = LED_YELLOW;
  } else {
    Serial.print("showRed - "); Serial.println(percent);
    leds = LED_RED;
    triggerMotorCount = (triggerMotorCount > 0) ? triggerMotorCount - 1 : 0;
    if (triggerMotor && triggerMotorCount <= 0) {
      triggerMotor = false;
      startWatering();
    }
  }

  updateShiftRegister();
  delay(MEASURE_DELAY);
}

void initLedsValues() {
  int tempPoti = analogRead(POTI_PIN);
  int percentPoti = tempPoti / 10.23;
  showRed = percentPoti;
  showRed = (showRed < MAX_SOIL - MAX_DIFF_SOIL) ? showRed : MAX_SOIL - MAX_DIFF_SOIL;
  showRed = (showRed > MIN_SOIL) ? showRed : MIN_SOIL;
  int diff = (MAX_SOIL - showRed) / 4;
  showYellow = showRed + diff;
  showGreen = showYellow + diff;
  showBlue = showGreen + diff;
  Serial.print("percentPoti: "); Serial.print(percentPoti);
  Serial.print(" R:"); Serial.print(showRed);
  Serial.print(" Y:"); Serial.print(showYellow);
  Serial.print(" G:"); Serial.print(showGreen);
  Serial.print(" B:"); Serial.print(showBlue);
}

void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void startWatering() {
  Serial.println("startWatering");
  //motor.run(BACKWARD);
  //motor.setSpeed(255);
}

void stopWatering() {
  triggerMotorCount = TRIGGER_COUNT;
  triggerMotor = true;
  Serial.println("stopWatering");
  //motor.run(BACKWARD);
  //motor.setSpeed(0);
}

