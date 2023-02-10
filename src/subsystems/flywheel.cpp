#include "flywheel.hpp"
#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/rtos.hpp"

using namespace okapi;

Motor flywheel(flywheelPort, false, AbstractMotor::gearset::blue,
               AbstractMotor::encoderUnits::degrees);

static float target = 0;
static const float rpmWindow = 75.0f;

ControllerButton fullFlywheelToggle = ControllerButton(ControllerDigital::R1);
ControllerButton halfFlywheelToggle = ControllerButton(ControllerDigital::R2);

void fwInit() {
  flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
  pros::Task flywheelControlHandle(controlFlywheelTask);
  pros::Task updateFlywheelHandle((updateFlywheelTask));
}

void controlFlywheelTask(void *) {
  while (true) {
    if (target == 0) {
      flywheel.moveVoltage(0);
      continue;
    }
    float error = target - (flywheel.getActualVelocity() * 5.0f);
    if (error > (target - rpmWindow)) {
      /// error = 2500 - 2000
      // 500 > 2500 -75
      // 
      // if (error <= -rpmWindow*8){
      //   flywheel.moveVoltage(flywheel.getActualVelocity()*0.2f);
      // }
      // target - v*e/100
      // 2500 - 2000*.8
      // 1500
      flywheel.moveVoltage(target - (flywheel.getActualVelocity() * 5.0f*error/100));
      //flywheel.moveVoltage(4000);
    } else if (error <= -rpmWindow) {
      flywheel.moveVoltage(0);
    } else { // Within threshold window -> Use Feedforward and P Controller
      flywheel.moveVoltage((target * 4.0f) + (error * 1.5f));
    }
    pros::delay(20);
  }
}

void updateFlywheelTask(void *) {
  while (true) {
    if (fullFlywheelToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::FULL_SPEED) {
        currentFlywheelState = FlywheelState::FULL_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }
    if (halfFlywheelToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::HALF_SPEED) {
        if (currentFlywheelState == FlywheelState::OFF){
          flywheel.moveVoltage(4000);
          pros::delay(400);
        }
        currentFlywheelState = FlywheelState::HALF_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    switch (currentFlywheelState) {
    case FlywheelState::OFF:
      target = 0;
      break;
    case FlywheelState::HALF_SPEED:
      target = 2800;
      break;
    case FlywheelState::FULL_SPEED:
      target = 3000;
      break;
    }
    pros::delay(20);
  }
}

FlywheelState getFWState() { return currentFlywheelState; }

void setFWState(FlywheelState FWstate) { currentFlywheelState = FWstate; }
