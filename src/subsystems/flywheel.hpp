#pragma once
#include "main.h"

using namespace okapi;

extern Motor flywheel;

extern void init();

extern void updateFlywheelTask(void *);
extern void controlFlywheelTask(void *);