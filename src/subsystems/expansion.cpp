#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

using namespace okapi;

pros::ADIDigitalOut expansion = pros::ADIDigitalOut(expansionPort);

ControllerButton expansionButton = ControllerButton(ControllerDigital::up);

void expansionInit() { expansion.set_value(false); }



void updateExpansion() {
  if (expansionButton.changedToPressed()) {
    expansion.set_value(true);
    pros::delay(2000);
    expansion.set_value(false);
  }
}