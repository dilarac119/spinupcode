#include "flywheel.hpp"
#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/rtos.hpp"

using namespace okapi;



Motor flywheel(flywheelPort, false, AbstractMotor::gearset::blue,
               AbstractMotor::encoderUnits::degrees);

enum class FlywheelState {
  OFF = 0,
  HALF_SPEED = 1,
  FULL_SPEED = 2,
};

FlywheelState currentFlywheelState = FlywheelState::OFF;
FlywheelState previousFlywheelState = FlywheelState::OFF;

static float target = 0;

ControllerButton flywheelToggle = ControllerButton(ControllerDigital::L1);

void init() {
  pros::Task flywheelControlHandle(controlFlywheelTask);
  pros::Task updateFlywheelHandle(updateFlywheelTask);
}

void controlFlywheelTask(void *) {
  while (true) {
    if (target == 0) {
      flywheel.moveVoltage(0);
      continue;
    }
    float error = target - (flywheel.getActualVelocity() * 5.0f);
    if (error > (target - 75.0f)) {
      flywheel.moveVoltage(12000);
    } else if (error <= -75.0f) {
      flywheel.moveVoltage(0);
    } else { // Within threshold window -> Use Feedforward and P Controller
      flywheel.moveVoltage((target * 4.0f) + (error * 1.125f));
    }
    pros::delay(20);
  }
}

void updateFlywheelTask(void *) {
  while (true) {
    if (flywheelToggle.changedToPressed()) {

      switch (currentFlywheelState) {
      case FlywheelState::OFF:
        currentFlywheelState = FlywheelState::HALF_SPEED;
        break;
      case FlywheelState::HALF_SPEED:
        currentFlywheelState = FlywheelState::FULL_SPEED;
        break;
      case FlywheelState::FULL_SPEED:
        currentFlywheelState = FlywheelState::OFF;
        break;
      }
    }

    switch (currentFlywheelState) {
    case FlywheelState::OFF:
      target = 0;
      break;
    case FlywheelState::HALF_SPEED:
      target = 2000;
      break;
    case FlywheelState::FULL_SPEED:
      target = 3000;
      break;
    }
    pros::delay(20);
  }
}

