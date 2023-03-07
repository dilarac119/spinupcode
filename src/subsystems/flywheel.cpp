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

ControllerButton fullFlywheelToggle = ControllerButton(ControllerDigital::R1);
ControllerButton halfFlywheelToggle = ControllerButton(ControllerDigital::R2);
ControllerButton zoomToggle = ControllerButton(ControllerDigital::X);

void fwInit() {
  flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
  pros::Task flywheelControlHandle((controlFlywheelTask));
  pros::Task updateFlywheelHandle((updateFlywheelTask));
}

void controlFlywheelTask(void *) {
  while (true) {
    if (target == 0) {
      flywheel.moveVelocity(0);
      continue;
    }
    float error = target - (flywheel.getActualVelocity() * 5.0f);
    if (error > (target - rpmWindow)) {
      flywheel.moveVoltage(target -
                           (flywheel.getActualVelocity() * 5.0f * error /
                           100));
      
    } else if (error <= -rpmWindow) {
      flywheel.moveVoltage(pros::c::motor_get_voltage(7)-300);
    } else { // Within threshold window -> Use Feedforward and P Controller
      flywheel.moveVoltage((target * 4.8f) + (error * 1.4f));
    }
    pros::delay(20);
  }
}

// void flywheelBangBangControl(void *) {
//   while (true) {
//     float target = static_cast<int>(currentFlywheelState);
//     if (target == 0) {
//       flywheel.moveVelocity(0);
//       pros::delay(10);
//       continue;
//     }
//     float error = target - (flywheel.getActualVelocity());
//     printf("error %f\n", error);
//     if (error > (rpmWindow)) {
//       flywheel.moveVelocity(12000);

//     }
//     // else if (error <= -rpmWindow) {
//     //   flywheel.moveVelocity(0);
//     // }
//     else { // Within threshold window -> Use Feedforward and P Controller
//       flywheel.moveVelocity((target * 5.0f) + (error * 1.5f));
//     }
//     pros::delay(10);
//   }
// }

// void print1(int error) {
//   int i = 0;
//   int its = 0;

//   pros::screen::set_pen(COLOR_BLUE);
//   while (1) {
//     // Will print seconds started since program started.
//     pros::screen::print(pros::E_TEXT_MEDIUM, 3, (error), i++);
//     its++;
//     pros::delay(1000);
//   }
// }

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
      target = 2300;
      break;
    case FlywheelState::FULL_SPEED:
      target = 2500;
      break;
    case FlywheelState::ZOOM:
      target = 4000;
    }
    pros::delay(20);
  }
}

FlywheelState getFWState() { return currentFlywheelState; }

void setFWState(FlywheelState FWstate) { currentFlywheelState = FWstate; }
