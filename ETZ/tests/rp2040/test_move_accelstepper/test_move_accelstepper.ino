#include <AccelStepper.h>

// Motor pins (same as test_move)
#define MOTOR_A_STEP 14
#define MOTOR_A_DIR 15
#define MOTOR_B_STEP 12
#define MOTOR_B_DIR 13

// CoreXY kinematics
// Motor A: X+Y, Motor B: X-Y
// X = (A + B) / 2, Y = (A - B) / 2

// Working area: 8000 x 8000 steps
const long FIELD_WIDTH = 8000;
const long FIELD_HEIGHT = 8000;

// Speed and acceleration settings
const float MAX_SPEED = 200;     // steps per second
const float ACCELERATION = 5000;  // steps per second^2

// Create AccelStepper objects
AccelStepper stepperA(AccelStepper::DRIVER, MOTOR_A_STEP, MOTOR_A_DIR);
AccelStepper stepperB(AccelStepper::DRIVER, MOTOR_B_STEP, MOTOR_B_DIR);

// Convert Cartesian coordinates to CoreXY motor positions
void moveToXY(long targetX, long targetY) {
  // CoreXY: Motor A = X + Y, Motor B = X - Y
  long motorA = targetX + targetY;
  long motorB = targetX - targetY;
  
  stepperA.moveTo(motorA);
  stepperB.moveTo(motorB);
}

// Run both motors until they reach their targets
void runToPosition() {
  while (stepperA.distanceToGo() != 0 || stepperB.distanceToGo() != 0) {
    stepperA.run();
    stepperB.run();
  }
}

// Move in a rectangle pattern
void drawRectangle() {
  Serial.println("Drawing rectangle...");
  
  // Start at (0, 0)
  moveToXY(0, 0);
  runToPosition();
  delay(500);
  
  // Move to (FIELD_WIDTH, 0) - right
  moveToXY(FIELD_WIDTH, 0);
  runToPosition();
  delay(500);
  
  // Move to (FIELD_WIDTH, FIELD_HEIGHT) - up
  moveToXY(FIELD_WIDTH, FIELD_HEIGHT);
  runToPosition();
  delay(500);
  
  // Move to (0, FIELD_HEIGHT) - left
  moveToXY(0, FIELD_HEIGHT);
  runToPosition();
  delay(500);
  
  // Move back to (0, 0) - down
  moveToXY(0, 0);
  runToPosition();
  delay(500);
}

// Move in diagonal patterns
void drawDiagonals() {
  Serial.println("Drawing diagonals...");
  
  // Diagonal 1: (0, 0) to (FIELD_WIDTH, FIELD_HEIGHT)
  moveToXY(0, 0);
  runToPosition();
  delay(500);
  
  moveToXY(FIELD_WIDTH, FIELD_HEIGHT);
  runToPosition();
  delay(500);
  
  // Diagonal 2: (FIELD_WIDTH, 0) to (0, FIELD_HEIGHT)
  moveToXY(FIELD_WIDTH, 0);
  runToPosition();
  delay(500);
  
  moveToXY(0, FIELD_HEIGHT);
  runToPosition();
  delay(500);
}

void setup() {
  Serial.begin(921600);
  
  // Configure stepper motors
  stepperA.setMaxSpeed(MAX_SPEED);
  stepperA.setAcceleration(ACCELERATION);
  stepperB.setMaxSpeed(MAX_SPEED);
  stepperB.setAcceleration(ACCELERATION);
  
  Serial.println("CoreXY test_move_accelstepper starting...");
  delay(1000);
}

void loop() {
  // Draw rectangle pattern
  drawRectangle();
  delay(1000);
  
  // Draw diagonal patterns
  drawDiagonals();
  delay(1000);
}
