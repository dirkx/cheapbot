#define CHEAPBOT

/* using 28BYj - 48 motors from http://www.ebay.co.uk/itm/131410728499 */

#define STEPS_PER_REV (4096.0 / 2.0) /* these motore come in two different gear rations, \
                                       * see http://42bots.com/tutorials/28byj-48-stepper-motor-with-uln2003-driver-and-arduino-uno/ 
                                       * and http://forum.arduino.cc/index.php?topic=71964.15
                                       */

#define MAX_SPEED 350         // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
#define MAX_ACCELERATION 400  // 1600 on 5v, 400 on 3v

#define WHEEL_DIAMETER 48.0  // radius of your wheel in mm
#define WHEEL_SPACING 150.0  // distance from one wheel to the other in mm

#ifdef CHEAPBOT
#include <Stepper.h>
Stepper(leftStepper, STEPS_PER_REV, 8, 9, 10, 11);
Stepper(rightStepper, STEPS_PER_REV, 8, 9, 10, 11);
#else
#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper leftStepper(AccelStepper::FULL4WIRE, 2, 4, 3, 5);  // note middle two pins are swapped to work with 28BYJ-48
                                                                // wire arduino pin D2 to driver board IN1, and so on

AccelStepper rightStepper(AccelStepper::FULL4WIRE, 6, 8, 7, 9);  // note middle two pins are swapped!
#endif

#ifndef CHEAPBOT
// Current position in mm, assuming we started pointing up the Y axis
float currentX = 0.0;
float currentY = 0.0;
float currentDirection = 0.0;  // in radians clockwise from the Y axis
#endif

typedef struct movement {
  int turn;
  int distance;
} movement;
#define MAX_MOVEMENTS 40


#ifndef CHEAPBOT
void goToXY(float x, float y);
#endif
void go(float distance);
void turn(float degrees);

movement movements[MAX_MOVEMENTS];

int nextMovement = 0;
boolean doneTurn = false;
boolean doneDistance = false;
int numMovements = 0;

void doNextMovement() {

  if (doneTurn && doneDistance) {
    doneTurn = false;
    doneDistance = false;
    nextMovement++;
  }

  if (nextMovement >= numMovements)
    return;  // finished


  if (!doneTurn) {
    doneTurn = true;
    turn(movements[nextMovement].turn);
  } else if (!doneDistance) {
    doneDistance = true;
    go(movements[nextMovement].distance);
  }
}

void addMovement(float turn, float distance) {
  if (numMovements >= MAX_MOVEMENTS)
    return;

  movements[numMovements].turn = turn;
  movements[numMovements].distance = distance;
  numMovements++;
}

inline void buildMovements() {
  // S
  addMovement(90, 40);
  addMovement(-90, 40);
  addMovement(-90, 40);
  addMovement(90, 40);
  addMovement(90, 40);

  addMovement(0, 10);

  //h
  addMovement(90, 80);
  addMovement(180, 40);
  addMovement(90, 40);
  addMovement(90, 40);

  addMovement(-90, 20);

  //o
  addMovement(0, 40);
  addMovement(-90, 40);
  addMovement(-90, 40);
  addMovement(-90, 40);
  addMovement(-90, 40);

  addMovement(0, 20);

  //n
  addMovement(-90, 50);
  addMovement(180, 10);
  addMovement(-90, 40);
  addMovement(90, 40);

  addMovement(-90, 20);

  //k
  addMovement(-90, 80);
  addMovement(180, 40);
  addMovement(-135, 40);
  addMovement(180, 40);
  addMovement(-90, 40 * 1.414);

  addMovement(-45, 20);

  //y
  addMovement(-90, 40);
  addMovement(180, 40);
  addMovement(-90, 40);
  addMovement(-90, 40);
  addMovement(180, 80);
  addMovement(90, 20);
}

void setup() {

#ifndef CHEAPBOT
  leftStepper.setMaxSpeed(MAX_SPEED);              // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  rightStepper.setMaxSpeed(MAX_SPEED);             // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  leftStepper.setAcceleration(MAX_ACCELERATION);   // 1600 on 5v
  rightStepper.setAcceleration(MAX_ACCELERATION);  // 1600 on 5v
#endif

  buildMovements();
#ifndef CHEAPBOT
  doNextMovement();
#endif
}

void loop() {
#ifdef CHEAPBOT
  doNextMovement();
#else
  if (leftStepper.distanceToGo() == 0 && rightStepper.distanceToGo() == 0)
    doNextMovement();

  leftStepper.run();
  rightStepper.run();
#endif
}

float stepsForDistance(float distance) {
  return distance * STEPS_PER_REV / (3.1415 * WHEEL_DIAMETER);
}


#ifndef CHEAPBOT
void goToXY(float x, float y) {
  float distance = sqrt((x - currentX) * (x - currentX) + (y - currentY) * (y - currentY));

  float theta = atan2((y - currentY), (x - currentX)) - currentDirection;
}
#endif

void go(float distance) {
  float steps = stepsForDistance(distance);
#ifndef CHEAPBOT
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(steps);
  rightStepper.move(-steps);
#else
  leftStepper_step(-steps);
  rightStepper_step(-steps);
#endif

#ifndef CHEAPBOT
  currentX += distance * sin(currentDirection);
  currentY += distance * cos(currentDirection);
#endif
}

void turn(float degrees)  //  clockwise
{
  float theta = radians(degrees);
  float distance = WHEEL_SPACING * 0.5 * theta;
  float steps = stepsForDistance(distance);
#ifdef CHEAPBOT
  leftStepper_step(-steps);
  rightStepper_step(-steps);
#else
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-steps);
  rightStepper.move(-steps);
  currentDirection += theta;
#endif
}