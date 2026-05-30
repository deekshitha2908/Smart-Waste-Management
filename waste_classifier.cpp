#include <CheapStepper.h>
#include <Servo.h>

// Define pins
#define IR_SENSOR 2
#define PROXIMITY_SENSOR 3
#define BUZZER 12
#define SOIL_SENSOR A0
#define SERVO2_PIN 6

// Initialize components
Servo servo2;
CheapStepper stepper(8, 9, 10, 11);

// Track current stepper angle
int currentAngle = 0;

void setup() {
  Serial.begin(9600);

  servo2.attach(SERVO2_PIN);

  pinMode(IR_SENSOR, INPUT);
  pinMode(PROXIMITY_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  stepper.setRpm(17);
}

void loop() {

  if (digitalRead(IR_SENSOR) == LOW) {

    Serial.println("Object Detected");

    tone(BUZZER, 1000, 300);
    delay(500);

    int proximityState = digitalRead(PROXIMITY_SENSOR);

    Serial.print("Proximity: ");
    Serial.println(proximityState);

    if (proximityState == LOW) {

      Serial.println("Metal Detected");

      rotateToBin(0);
      openBin();
    }
    else {

      int moisture = readSoilMoisture();

      Serial.print("Soil Moisture: ");
      Serial.print(moisture);
      Serial.println("%");

      if (moisture > 30) {

        Serial.println("Wet Waste");

        rotateToBin(200);
        openBin();
      }
      else {

        Serial.println("Dry Waste");

        tone(BUZZER, 1500, 500);

        rotateToBin(120);
        openBin();
      }
    }

    delay(1000);
  }
}

void openBin() {
  servo2.write(30);
  delay(1000);

  servo2.write(110);
  delay(1000);
}

void rotateToBin(int targetAngle) {

  int angleToMove = targetAngle - currentAngle;

  if (angleToMove == 0) {
    return;
  }

  if (angleToMove > 0) {
    stepper.moveDegreesCW(angleToMove);
  }
  else {
    stepper.moveDegreesCCW(-angleToMove);
  }

  currentAngle = targetAngle;

  delay(1000);
}

int readSoilMoisture() {

  int total = 0;

  for (int i = 0; i < 3; i++) {

    int raw = analogRead(SOIL_SENSOR);

    raw = constrain(raw, 485, 1023);

    total += map(raw, 485, 1023, 100, 0);

    delay(100);
  }

  return total / 3;
}