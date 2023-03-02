#include "main.h"
#include "intake.hpp"
#include "okapi/api/util/mathUtil.hpp"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/vision.hpp"

using namespace okapi;

Motor conveyor(conveyorPort, false, AbstractMotor::gearset::blue,
               AbstractMotor::encoderUnits::degrees);



ControllerButton intakeButton = ControllerButton(ControllerDigital::L1);
ControllerButton outakeButton = ControllerButton(ControllerDigital::L2);
ControllerButton halfInButton = ControllerButton(ControllerDigital::up);

void intakeInit() { conveyor.setBrakeMode(AbstractMotor::brakeMode::coast); }

void updateConveyor() {

  // if (outakeButton.changedToPressed()) {
  //   previousIntakeState = currentIntakeState;
  //   currentIntakeState = IntakeState::OUTTAKING;
  // } else if (outakeButton.changedToReleased()) {
  //   currentIntakeState = previousIntakeState;
  // }

  if (intakeButton.changedToPressed()) {
    if (currentIntakeState == IntakeState::INTAKING) {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::STOPPED;
    } else {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::INTAKING;
    }
  } 
  if (halfInButton.changedToPressed()) {
    if (currentIntakeState == IntakeState::HALF) {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::STOPPED;
    } else {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::HALF;
    }
  }
  if (outakeButton.changedToPressed()) {
    if (currentIntakeState == IntakeState::OUTTAKING) {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::STOPPED;
    } else {
      previousIntakeState = currentIntakeState;
      currentIntakeState = IntakeState::OUTTAKING;
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
  case IntakeState::HALF:
    conveyor.moveVoltage(7000);
    
  }
}

IntakeState getIntakeState() { return currentIntakeState; }

void setIntakeState(IntakeState Istate) { currentIntakeState = Istate; }
