#include "auton.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "indexer.hpp"
#include "intake.hpp"
#include "main.h"
#include "odom.hpp"
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
  pros::delay(100);
  indexer.set_value(false);
}

bool isRed(double hue) {
  bool red = false;
  if (hue >= 0 && hue <= 10) {
    red = true;
  }
  return red;
}

void rollerRed() {
  while (isRed(opticalSensor.getHue())) {
    conveyor.moveVoltage(-10000);
  }
  conveyor.moveVoltage(0);
}

void rollerBlue() {
  while (!isRed(opticalSensor.getHue())) {
    conveyor.moveVoltage(-10000);
  }
  conveyor.moveVoltage(0);
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
  pros::delay(10000);
  drive->getModel()->tank(0, 0);
}

void autonFlywheelREVERSE() {
  FlywheelState autonFlywheelState = FlywheelState::REVERSE;
  setFWState(autonFlywheelState);
}


void autonFlywheelAUTONLOW() {
  FlywheelState autonFlywheelState = FlywheelState::AUTONLOW;
  setFWState(autonFlywheelState);
}

void autonFlywheelAUTONHIGH() {
  FlywheelState autonFlywheelState = FlywheelState::AUTONHIGH;
  setFWState(autonFlywheelState);
}

void autonFlywheelOFF() {
  FlywheelState autonFlywheelState = FlywheelState::OFF;
  setFWState(autonFlywheelState);
}

void autonDirect() {

  // auton direct

  autonFlywheelAUTONHIGH();

  // roller
  drive->getModel()->tank(0.2, 0.2);
  conveyor.moveVoltage(-12000);
  pros::delay(150);
  conveyor.moveVoltage(0);
  pros::delay(20);
  drive->getModel()->tank(0, 0);
  pros::delay(200);

  // // move bakv
  // movePIDOdomR(5, 5, 500, 0.3);
  // pros::delay(20);
  // turnPIDOdomCC(5, 5, 500, 0.3);
  // pros::delay(20);

  // // shoot

  // autonIndexer();
  // pros::delay(20);
  // pros::delay(1800);
  // autonIndexer();
  // pros::delay(10);
  // autonFlywheelOFF();
}

void autonIndirect() {
  // auton indirect

  // move to roller
  // activate flywheel
  autonFlywheelAUTONHIGH();
  pros::delay(20);

  movePIDOdom(24, 24, 50000, .9);
  pros::delay(20);
  turnPIDOdomCW(11.2, 11.2, 1000, 1);
  pros::delay(20);

  // creep
  movePIDOdom(10, 10, 1000, 0.3);
  pros::delay(30);

  // roller
  drive->getModel()->tank(0.2, 0.2);
  conveyor.moveVoltage(-12000);
  pros::delay(150);
  conveyor.moveVoltage(0);
  pros::delay(20);
  drive->getModel()->tank(0, 0);
  pros::delay(200);

  // move bakv
  movePIDOdomR(5, 5, 500, 0.3);
  pros::delay(20);
  turnPIDOdomCW(4.8, 4.8, 800, 0.3);

  // shoot

  autonIndexer();
  pros::delay(20);
  pros::delay(3000);
  autonIndexer();
  pros::delay(10);
  autonFlywheelOFF();
}

 void skills(int color) {

//   // skills

//   // roller 1
//   drive->getModel()->tank(0.2, 0.2);
//   conveyor.moveVoltage(-12000);
//   pros::delay(300);
//   conveyor.moveVoltage(0);
//   drive->getModel()->tank(0, 0);
//   pros::delay(20);

//   // back up
//   movePIDOdomR(24, 24, 1500, 0.3);
//   pros::delay(500);

//   // activate flywheel
//   autonFlywheelAUTONLOW();
//   pros::delay(200);

//   // turn to second roller
//   turnPIDOdomCW(11, 11, 1000, .75);
//   pros::delay(500);
//   // move forward
//   movePIDOdom(22, 22, 3000, 1);
//   pros::delay(300);

//   // second roller
//   drive->getModel()->tank(0.2, 0.2);
//   conveyor.moveVoltage(-12000);
//   pros::delay(300);
//   conveyor.moveVoltage(0);
//   drive->getModel()->tank(0, 0);
//   pros::delay(20);
//   movePIDOdomR(5, 5, 1500, 0.7);
//   pros::delay(20);

//   // turn
//   turnPIDOdomCC(10.35, 10.35, 1000, .8);
//   pros::delay(20);
//   movePIDOdomR(49, 46, 4000, 0.9);

//   // shoot
//   pros::delay(1000);
//   autonIndexer();
//   pros::delay(20);
//   autonFlywheeAUTONHIGH();
//   pros::delay(1400);
//   autonIndexer();
//   pros::delay(10);
//   autonIndexer();
//   pros::delay(20);

//   // reverse
//   movePIDOdom(36, 36, 3000, 0.5);
//   pros::delay(20);

//   // turn diagonal
//   turnPIDOdomCC(15.8, 15.8, 1000, .9);
//   pros::delay(20);

//   // activate conveyor
//   conveyor.moveVoltage(20000);
//   pros::delay(200);
//   movePIDOdom(61, 61, 6000, 0.7);
//   pros::delay(2000);

//   // change flywheel speed
//   autonFlywheelMID();

//   // turn to basket
//   turnPIDOdomCW(11.35, 11.35, 1200, .7);
//   pros::delay(280);

//   // shoot 2
//   autonIndexer();
//   pros::delay(900);
//   autonIndexer();
//   pros::delay(1300);
//   autonIndexer();
//   pros::delay(800);
//   // turn back
//   // kinda diagonal
//   turnPIDOdomCW(8, 8, 900, .7);
//   pros::delay(20);
//   movePIDOdomR(30, 30, 3000, 0.9);
//   pros::delay(500);
//   // paralel to roller
//   turnPIDOdomCW(11, 11, 900, .9);
//   pros::delay(200);
//   movePIDOdomR(42, 42, 3000, 0.9);
//   pros::delay(200);

//   // line up turn to roller 3
//   turnPIDOdomCW(10, 10, 1000, .9);
//   pros::delay(200);
//   movePIDOdom(20, 20, 1000, .2);
//   pros::delay(100);

//   // roller 3
//   drive->getModel()->tank(0.2, 0.2);
//   conveyor.moveVoltage(-12000);
//   pros::delay(300);
//   conveyor.moveVoltage(0);
//   drive->getModel()->tank(0, 0);
//   pros::delay(20);

//   // go back to 4
//   movePIDOdomR(21, 21, 1500, 0.7);
//   pros::delay(20);
//   // turn to roller 4
//   turnPIDOdomCW(11, 11, 1000, .9);
//   pros::delay(200);
//   movePIDOdom(21, 21, 2000, .3);
//   pros::delay(100);

//   // roller 4
//   drive->getModel()->tank(0.2, 0.2);
//   conveyor.moveVoltage(-12000);
//   pros::delay(300);
//   conveyor.moveVoltage(0);
//   autonFlywheelOFF();
//   drive->getModel()->tank(0, 0);
//   pros::delay(20);

//   // allign for expanison
//   movePIDOdomR(4, 4, 1500, 0.7);
//   pros::delay(20);
//   turnPIDOdomCC(6, 6, 1000, .9);
//   pros::delay(200);

//   // expansion
//   expansion.set_value(true);
//   pros::delay(50);
//   expansion.set_value(false);
}
