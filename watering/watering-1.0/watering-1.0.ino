#define IS_WET 90
#define IS_OKAY 75
#define IS_DRY 60
#define RED 3
#define ORANGE 5
#define GREEN 6
#define BLUE 9

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:

  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(A0); // read the input on analog pin 0:
  float percent = 100 - (sensorValue / 10.24);
  
  analogWrite(RED, 0);
  analogWrite(ORANGE, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  
  if (percent >= IS_WET) {
    Serial.print("IS_WET - "); Serial.println(percent);
    analogWrite(BLUE, 255);
  } else if (percent >= IS_OKAY) {
    Serial.print("IS_OKAY - "); Serial.println(percent);
    analogWrite(GREEN, 255);
  } else if (percent >= IS_DRY) {
    Serial.print("IS_DRY - "); Serial.println(percent);
    analogWrite(ORANGE, 255);
  } else { 
    Serial.print("TOO_DRY - "); Serial.println(percent);
    analogWrite(RED, 255);
  }
  
  delay(500); // delay
}
