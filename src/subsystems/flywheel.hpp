#pragma once
#include "main.h"

using namespace okapi;

extern Motor flywheel;

extern void fwInit();

enum class FlywheelState {
  OFF = 0,
  HALF_SPEED = 1,
  FULL_SPEED = 2,
  ZOOM = 3,
};

extern void updateFlywheelTask(void *);
extern void controlFlywheelTask(void *);
extern void flywheelBangBangControl(void*);

extern FlywheelState getFWState();
extern void setFWState(FlywheelState FWstate);

static FlywheelState currentFlywheelState = FlywheelState::OFF;
