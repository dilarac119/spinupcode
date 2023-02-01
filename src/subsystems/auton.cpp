#include "auton.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "indexer.hpp"
#include "intake.hpp"
#include "main.h"
#include "okapi/api/chassis/controller/odomChassisController.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include <cstdio>

auto opticalSensor = OpticalSensor(opticalPort);

void print1() {
  int i = 0;
  int its = 0;

  pros::screen::set_pen(COLOR_BLUE);
  while (1) {
    // Will print seconds started since program started.
    pros::screen::print(pros::E_TEXT_MEDIUM, 3, ("1"), i++);
    its++;
    pros::delay(1000);
  }
}

void autonIndexer() {
  indexer.set_value(true);
  pros::delay(500);
  indexer.set_value(false);
  pros::delay(20);
}

bool isRed(double hue) {
  bool red = false;
  if (hue >= 0 && hue <= 50) {
    red = true;
  }
  return red;
}

void rollerRed() {
  drive->getModel()->tank(0.2, 0.2);
  conveyor.moveVelocity(600);
  pros::delay(300);
  conveyor.moveVelocity(0);
  drive->getModel()->tank(0, 0);
  pros::delay(600);
  if (isRed(opticalSensor.getHue())) {
    while (isRed(opticalSensor.getHue())) {
      drive->getModel()->tank(0.2, 0.2);
      conveyor.moveVelocity(100);
    }
    conveyor.moveVelocity(300);
    pros::delay(150);
    drive->getModel()->tank(0, 0);
   
  } else {
    drive->getModel()->tank(0.2, 0.2);
    conveyor.moveVelocity(300);
    pros::delay(150);
    drive->getModel()->tank(0, 0);
  }
}

void rollerBlue() {
  drive->getModel()->tank(0.2, 0.2);
  conveyor.moveVelocity(600);
  pros::delay(300);
  conveyor.moveVelocity(0);
  drive->getModel()->tank(0, 0);
  pros::delay(600);
  if (!isRed(opticalSensor.getHue())) {
    while (!isRed(opticalSensor.getHue())) {
      drive->getModel()->tank(0.2, 0.2);
      conveyor.moveVelocity(100);
    }
    conveyor.moveVelocity(300);
    pros::delay(200);
    drive->getModel()->tank(0, 0);
  }
}

void rollUntilColor(int color) {
  // red = 1, blue = 2

  if (color == 1) {
    rollerRed();
  } else if (color == 2) {
    rollerBlue();
  }
}

void autonRoller(int color) {
  // red = 1, blue = 2

  drive->getModel()->tank(.2, .2);
  rollUntilColor(color);
  pros::delay(1000);
  drive->getModel()->tank(0, 0);
}

// void autonDirect(int color) {

//   // auton initialization
//   FlywheelState autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);
//   // IntakeState currentIntakeState = IntakeState::STOPPED;
//   // setIntakeState(currentIntakeState);

//   // first roller
//   autonRoller(color);
//   pros::delay(20);

//   // square up
//   driveForward(0.6, true);
//   pros::delay(500);

//   drive->getModel()->tank(0.5, -0.5);
//   pros::delay(620);
//   drive->getModel()->tank(0, 0);

//   // pros::delay(200);
//   // driveForward(2, true);
//   // driveForward(2, true);
//   // driveForward(1.5, true);
//   // pros::delay(500);
//   // rotate(-40);
//   // pros::delay(100);

//   // start flywheel
//   autonFlywheelState = FlywheelState::HALF_SPEED;
//   setFWState(autonFlywheelState);
//   pros::delay(2900);
//   drive->getModel()->tank(0, 0);

//   // firts shoot
//   autonIndexer();
//   pros::delay(2000);

//   // second shoot
//   autonIndexer();
//   pros::delay(20);

//   // flywheel off
//   autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);
// }

// void autonIndirect(int color) {

//   FlywheelState autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);
//   // IntakeState currentIntakeState = IntakeState::STOPPED;
//   // setIntakeState(currentIntakeState);

//   // move to roller
//   driveForward(1, false);
//   pros::delay(1000);
//   rotate(80);
//   pros::delay(500);
//   // driveForward(0.8, false);

//   // first roller
//   autonRoller(color);
//   pros::delay(20);

//   // reverse reverse
//   driveForward(0.8, true);
//   pros::delay(200);
//   drive->getModel()->tank(0.2, -0.2);
//   pros::delay(495);
//   drive->getModel()->tank(0, 0);

//   // start flywheel
//   autonFlywheelState = FlywheelState::FULL_SPEED;
//   setFWState(autonFlywheelState);
//   pros::delay(2900);

//   // firts shoot
//   autonIndexer();
//   pros::delay(2000);

//   // second shoot
//   autonIndexer();
//   pros::delay(20);

//   // flywheel off
//   autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);
// }

// void skills(int color) {

//   FlywheelState autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);

//   // first roller
//   autonRoller(color);
//   pros::delay(20);
//   driveForward(1.6, true);
//   pros::delay(300);

//   rotate(90);
//   pros::delay(500);
//   driveForward(2, false);
//   pros::delay(300);

//   autonRoller(color);
//   pros::delay(20);

//   // reverse reverse
//   driveForward(0.8, true);
//   pros::delay(200);
//   drive->getModel()->tank(0.2, -0.2);
//   pros::delay(400);
//   drive->getModel()->tank(0, 0);

//   // start flywheel
//   autonFlywheelState = FlywheelState::FULL_SPEED;
//   setFWState(autonFlywheelState);
//   pros::delay(2900);

//   // firts shoot
//   autonIndexer();
//   pros::delay(2000);

//   // second shoot
//   autonIndexer();
//   pros::delay(20);

//   // flywheel off
//   autonFlywheelState = FlywheelState::OFF;
//   setFWState(autonFlywheelState);

//   // turn
//   drive->getModel()->tank(0.4, -0.4);
//   pros::delay(1000);
//   drive->getModel()->tank(0, 0);

//   // conveyor on
//   conveyor.moveVelocity(600);

//   // zoom
//   driveForward(2, false);
//   driveForward(2, false);
//   driveForward(2, false);
//   driveForward(2, false);
//   driveForward(2, false);
//   driveForward(2, false);
//   pros::delay(3000);

//   // conveyor off
//   conveyor.moveVelocity(0);
//   pros::delay(20);

//   // turn
//   rotate(90);
//   pros::delay(500);

//   // second pair of rollers
//   //  first roller
//   autonRoller(color);
//   pros::delay(20);
//   driveForward(0.8, false);
//   pros::delay(200);

//   rotate(90);
//   pros::delay(500);
//   driveForward(0.8, true);
//   pros::delay(200);

//   autonRoller(color);
//   pros::delay(20);
// }
