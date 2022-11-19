#include "main.h"
#include "okapi/api/util/mathUtil.hpp"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/vision.hpp"

using namespace okapi;

Motor conveyor(conveyorPort, false, AbstractMotor::gearset::green,
               AbstractMotor::encoderUnits::degrees);

enum class IntakeState {
  STOPPED = 0,
  INTAKING = 1,
  OUTTAKING = 2,
};

IntakeState currentIntakeState = IntakeState::STOPPED;
IntakeState previousIntakeState = IntakeState::STOPPED;

ControllerButton intakeToggle = ControllerButton(ControllerDigital::R1);
ControllerButton outakeButton = ControllerButton(ControllerDigital::B);

void updateConveyor() {
  if (outakeButton.changedToPressed()) {
    previousIntakeState = currentIntakeState;
    currentIntakeState = IntakeState::OUTTAKING;
  } else if (outakeButton.changedToReleased()) {
    currentIntakeState = previousIntakeState;
  }

  if (intakeToggle.changedToPressed()) {
    if (currentIntakeState != IntakeState::INTAKING) {
      currentIntakeState = IntakeState::INTAKING;
    } else {
      currentIntakeState = IntakeState::STOPPED;
    }
  }

  switch (currentIntakeState) {
  case IntakeState::STOPPED:
    conveyor.moveVoltage(0);
    break;
  case IntakeState::INTAKING:
    conveyor.moveVoltage(12000);
    break;
  case IntakeState::OUTTAKING:
    conveyor.moveVoltage(-12000);
    break;
  }
}
