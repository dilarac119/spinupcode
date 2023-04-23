#pragma once
#include "main.h"

using namespace okapi;

extern pros::ADIDigitalOut expansion;
extern pros::ADIDigitalIn expansionButton;
extern pros::ADIDigitalOut blooper;
extern pros::ADIDigitalIn blooperButton;

void expansionInit();
void updateExpansion();
void blooperInit();
void updateBlooper();