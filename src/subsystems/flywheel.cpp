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
static const float rpmWindow = 80.0f;

ControllerButton fullFlywheelToggle = ControllerButton(ControllerDigital::R1);
ControllerButton halfFlywheelToggle = ControllerButton(ControllerDigital::R2);
ControllerButton zoomToggle = ControllerButton(ControllerDigital::X);

void fwInit() {
  flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
  pros::Task flywheelControlHandle((flywheelBangBangControl));
  pros::Task updateFlywheelHandle((updateFlywheelTask));
}

// void controlFlywheelTask(void *) {
//   while (true) {
//     if (target == 0) {
//       flywheel.moveVoltage(0);
//       continue;
//     }
//     float error = target - (flywheel.getActualVelocity() * 4.0f);
//     if (error > (target - rpmWindow)) {
//       flywheel.moveVoltage(target -
//                            (flywheel.getActualVelocity() * 4.0f * error /
//                            100));
//       // flywheel.moveVoltage(4000);
//     } else if (error <= -rpmWindow) {
//       flywheel.moveVoltage(0);
//     } else { // Within threshold window -> Use Feedforward and P Controller
//       flywheel.moveVoltage((target * 4.0f) + (error * 1.6f));
//     }
//     pros::delay(20);
//   }
// }

void flywheelBangBangControl(void *) {
  while (true) {
    float target = static_cast<int>(currentFlywheelState);
    if (target == 0) {
      flywheel.moveVoltage(0);
      pros::delay(10);
      continue;
    }
    float error = target - (flywheel.getActualVelocity() * 5);
    if (error > (target - rpmWindow)) {
      flywheel.moveVoltage(12000);
    } else if (error <= -rpmWindow) {
      flywheel.moveVoltage(0);
    } else { // Within threshold window -> Use Feedforward and P Controller
      flywheel.moveVoltage((target * 5.0) + (error * 0.85));
    }
    pros::delay(10);
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
        currentFlywheelState = FlywheelState::HALF_SPEED;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }
    if (zoomToggle.changedToPressed()) {
      if (currentFlywheelState != FlywheelState::ZOOM) {
        currentFlywheelState = FlywheelState::ZOOM;
      } else {
        currentFlywheelState = FlywheelState::OFF;
      }
    }

    switch (currentFlywheelState) {
    case FlywheelState::OFF:
      target = 0;
      break;
    case FlywheelState::HALF_SPEED:
      target = 2200;
      break;
    case FlywheelState::FULL_SPEED:
      target = 2400;
      break;
    case FlywheelState::ZOOM:
      target = 3600;
    }
    pros::delay(20);
  }
}

FlywheelState getFWState() { return currentFlywheelState; }

void setFWState(FlywheelState FWstate) { currentFlywheelState = FWstate; }
