#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

using namespace okapi;

pros::ADIDigitalOut expansion = pros::ADIDigitalOut(expansionPort);
ControllerButton expansionButton = ControllerButton(ControllerDigital::B);    
pros::ADIDigitalOut blooper = pros::ADIDigitalOut(blooperPort);
ControllerButton blooperButton = ControllerButton(ControllerDigital::down);    



void expansionInit() { expansion.set_value(false); }
void blooperInit() { blooper.set_value(false); }

int Estate = 0;
int Bstate = 0;



void updateExpansion() {
  if (expansionButton.changedToPressed()) {
    if(Estate == 0){
     expansion.set_value(true);
     Estate++;
    } else{
     expansion.set_value(false);
     Estate--;
    }
  }
}

void updateBlooper() {
  if (blooperButton.changedToPressed()) {
    if(Bstate == 0){
     blooper.set_value(true);
     Bstate++;
    } else{
     blooper.set_value(false);
     Bstate--;
    }
  }
}