#include "main.h"
#include "okapi/api/util/mathUtil.hpp"
#include "pros/vision.hpp"

using namespace okapi;

Motor conveyor(conveyorPort, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

bool released1 = true;
bool released2 = true;
bool releasedA = true;


bool toggle1 = false;
bool toggle2 = false;
bool toggleA = false;


void rollerRed()
{

pros::Vision vision_sensor (8);


  if ( vision_sensor.get_object_count() > 0){
    while((vision_sensor.get_object_count()) > 0){
      conveyor.moveVelocity(600);
    }
    pros::delay(20);
    while(vision_sensor.get_object_count()  < 1){
     conveyor.moveVelocity(600);
    }
    
     conveyor.moveVelocity(0);
  } else if (vision_sensor.get_object_count() < 1){
    while(vision_sensor.get_object_count()  < 1){
     conveyor.moveVelocity(600);
    }
  }
}

void rollerBlue()
{

  
}

void updateConveyor()
{

  if (controller.getDigital(ControllerDigital::R1) == 0)
  {
      released1 = true;
  }


  if (controller.getDigital(ControllerDigital::R1) == 1 && released1)
  {
      released1 = false;

    toggle2 = false;
    if (!toggle1){
      conveyor.moveVelocity(600);
      toggle1 = true;
    } else if (toggle1){
      conveyor.moveVelocity(0);
      toggle1 = false;
    }
    
  }

   if (controller.getDigital(ControllerDigital::R2) == 0)
  {
      released2 = true;
  }

   if (controller.getDigital(ControllerDigital::R2) == 1 && released2){
 
 
    released2 = false;

    toggle1 = false;
    if (!toggle2){
      conveyor.moveVelocity(-600);
      toggle2 = true;
    } else if (toggle2){
      conveyor.moveVelocity(0);
      toggle2 = false;
    }

  }
  

  if (controller.getDigital(ControllerDigital::B) == 1 )
  {
    rollerRed();
  }


}


/*
  else if (controller.getDigital(ControllerDigital::R2) == 1)
  {
    conveyor.moveVelocity(-600);
  }
  else
  {
    conveyor.moveVelocity(0);
  }
}

*/


