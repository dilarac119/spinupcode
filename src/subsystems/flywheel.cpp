#include "flywheel.hpp"
#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

using namespace okapi;

Motor flywheel(flywheelPort, false, AbstractMotor::gearset::blue,
               AbstractMotor::encoderUnits::degrees);

static float target = 0;
static const float rpmWindow = 30.0f;

ControllerButton midFlywheelToggle = ControllerButton(ControllerDigital::R1);
ControllerButton lowFlywheelToggle = ControllerButton(ControllerDigital::R2);
ControllerButton highFlywheelToggle = ControllerButton(ControllerDigital::X);
ControllerButton reverseToggle = ControllerButton(ControllerDigital::left);

void fwInit() {
  flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
  static int auton = 0;
  pros::Task flywheelControlHandle((controlFlywheelTask));
  pros::Task updateFlywheelHandle((updateFlywheelTask));
}

void controlFlywheelTask(void *) {

  if (auton == 1) {
    bool max = false;

    while (true) {
      if (currentFlywheelState != FlywheelState::OFF) {
        pros::delay(100);
        if (flywheel.getActualVelocity() < (target - 20)) {
          flywheel.moveVoltage(12000);
        } else if (flywheel.getActualVelocity() < (target + 20) &&
                   flywheel.getActualVelocity() > (target - 20)) {
          flywheel.moveVoltage(target * 20);
        } else if (flywheel.getActualVelocity() > (target + 20)) {
          flywheel.moveVoltage(0);
        } else {
          flywheel.moveVelocity(target);
        }

      } else {
        // flywheel.controllerSet(0);
        flywheel.moveVelocity(0);
        // power = "OFF";
      }
    }
  } else {
    bool max = false;
    while (true) {
      if (currentFlywheelState != FlywheelState::OFF) {
        if (currentFlywheelState != FlywheelState::MID_SPEED) {
          if (flywheel.getActualVelocity() < (target - 280)) {
            // flywheel.controllerSet(1);
            flywheel.moveVelocity(600);
            // power = "ON";
          } else {
            // flywheel.controllerSet(0.75);
            flywheel.moveVelocity(target);
            // power = "ON";
          }
        } else {
          if (flywheel.getActualVelocity() < (target - 60)) {
            // flywheel.controllerSet(1);
            flywheel.moveVelocity(600);
            // power = "ON";
          } else {
            // flywheel.controllerSet(0.75);
            max = true;
            flywheel.moveVelocity(target);
            // power = "ON";
          }
        }

      } else {
        // flywheel.controllerSet(0);
        flywheel.moveVelocity(0);
        // power = "OFF";
      }
    }
  }
}


void updateFlywheelTask(void *) {
  while (true) {
    if (lowFlywheelToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::LOW_SPEED) {
        currentFlywheelState = FlywheelState::LOW_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    if (midFlywheelToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::MID_SPEED) {
        currentFlywheelState = FlywheelState::MID_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    if (highFlywheelToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::HIGH_SPEED) {
        currentFlywheelState = FlywheelState::HIGH_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    if (reverseToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::REVERSE) {
        currentFlywheelState = FlywheelState::REVERSE;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    switch (currentFlywheelState) {
    case FlywheelState::OFF:
      target = 0;
      break;
    case FlywheelState::LOW_SPEED:
      target = 420;
      break;
    case FlywheelState::MID_SPEED:
      target = 405;
      break;
    case FlywheelState::HIGH_SPEED:
      target = 450;
      break;
    case FlywheelState::REVERSE:
      target = -500;
      break;
    case FlywheelState::AUTONHIGH:
      target = 485;
      break;
    case FlywheelState::AUTONLOW:
      target = 420;
      break;
    }
    pros::delay(20);
  }
}

FlywheelState getFWState() { return currentFlywheelState; }

void setFWState(FlywheelState FWstate) { currentFlywheelState = FWstate; }