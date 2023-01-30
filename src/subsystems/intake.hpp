#pragma once

#include "main.h"
//#include "pros/vision.hpp"

using namespace okapi;

extern Motor conveyor;

// extern pros::Vision vision1;

enum class IntakeState {
  STOPPED = 0,
  INTAKING = 1,
  OUTTAKING = 2,
};


extern IntakeState getIntakeState();
extern void setIntakeState(IntakeState IState);

static IntakeState currentIntakeState = IntakeState::STOPPED;
static IntakeState previousIntakeState = IntakeState::STOPPED;

void intakeInit();
void updateConveyor();