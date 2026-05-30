#include <Servo.h>

// Define control pins for ULN2003AN driver
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Define servo pin
#define SERVO1_PIN 5

// Steps sequence for 28BYJ-48 (Half-step mode)
const int stepSequence[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

int stepIndex = 0;
int direction = -1;

unsigned long lastStepTime = 0;
unsigned long stepInterval = 2;

Servo servo1;

bool pushing = false;
unsigned long pushStartTime = 0;
unsigned long pushDuration = 500;

int stepsCounter = 0;
int stepsPerRevolution = 512;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servo1.attach(SERVO1_PIN);
  servo1.write(30);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastStepTime >= stepInterval) {
    lastStepTime = currentTime;

    stepMotor(direction);
    stepsCounter++;

    if (stepsCounter >= stepsPerRevolution && !pushing) {
      stepsCounter = 0;
      servo1.write(110);
      pushing = true;
      pushStartTime = currentTime;
    }
  }

  if (pushing && (currentTime - pushStartTime >= pushDuration)) {
    servo1.write(30);
    delay(1000);
    pushing = false;
  }
}

void stepMotor(int dir) {
  stepIndex += dir;

  if (stepIndex > 7) stepIndex = 0;
  if (stepIndex < 0) stepIndex = 7;

  digitalWrite(IN1, stepSequence[stepIndex][0]);
  digitalWrite(IN2, stepSequence[stepIndex][1]);
  digitalWrite(IN3, stepSequence[stepIndex][2]);
  digitalWrite(IN4, stepSequence[stepIndex][3]);
}