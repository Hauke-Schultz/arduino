#include <AFMotor.h>
AF_DCMotor motor(4);

#define READSOILPIN A0
#define MEASURE_DELAY 500
#define TRIGGER_COUNT 10

#define POTI_PIN A5

#define MIN_SOIL 20
#define MAX_SOIL 90
#define MAX_DIFF_SOIL 8
#define LED_GREEN A3
#define LED_YELLOW A2
#define LED_RED A1

float showGreen = 78;
float showYellow = 69;
float showRed = 60;

byte leds = 0;

bool triggerMotor = true;
int triggerMotorCount = TRIGGER_COUNT;


void setup() {
  Serial.begin(9600);
  pinMode(READSOILPIN, INPUT);
  pinMode(POTI_PIN, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  motor.run(RELEASE);
}

void loop() {
  int sensorValue = analogRead(READSOILPIN);
  float percent = 100 - (sensorValue / 10.23);
  initLedsValues();

  if (percent >= showGreen) {
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
  digitalWrite(leds, HIGH);
  delay(MEASURE_DELAY);
}

void initLedsValues() {
  int tempPoti = analogRead(POTI_PIN);
  int percentPoti = tempPoti / 10.23;
  showRed = percentPoti;
  showRed = (showRed < MAX_SOIL - MAX_DIFF_SOIL) ? showRed : MAX_SOIL - MAX_DIFF_SOIL;
  showRed = (showRed > MIN_SOIL) ? showRed : MIN_SOIL;
  int diff = (MAX_SOIL - showRed) / 3;
  showYellow = showRed + diff;
  showGreen = showYellow + diff;
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  Serial.print("percentPoti: "); Serial.print(percentPoti);
  Serial.print(" R:"); Serial.print(showRed);
  Serial.print(" Y:"); Serial.print(showYellow);
  Serial.print(" G:"); Serial.print(showGreen);
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

