#pragma once
#include "main.h"

using namespace okapi;

extern pros::ADIDigitalOut expansion;
extern pros::ADIDigitalIn expansionButton;

void expansionInit();
void updateExpansion();