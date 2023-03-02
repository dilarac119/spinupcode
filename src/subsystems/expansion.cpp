#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

using namespace okapi;

pros::ADIDigitalOut expansion = pros::ADIDigitalOut(expansionPort);

ControllerButton expansionButton = ControllerButton(ControllerDigital::B);



void expansionInit() { expansion.set_value(false); }
int state = 0;



void updateExpansion() {
  if (expansionButton.changedToPressed()) {
    if(state == 0){
     expansion.set_value(true);
     state++;
    } else{
     expansion.set_value(false);
     state--;
    }
  }
}