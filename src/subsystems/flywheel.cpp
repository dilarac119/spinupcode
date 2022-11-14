#include "main.h"

using namespace okapi;

bool released3 = true;
bool released4 = true;


bool toggle3 = false;
bool toggle4 = false;

Motor flywheel(flywheelPort,false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

int flywheelState;
bool released = true;
bool toggle = false;

int prevError = 0;

void updateFlywheel()
{

  if (controller.getDigital(ControllerDigital::L1) == 0)
  {
      released3 = true;
  }


  if (controller.getDigital(ControllerDigital::L1) == 1 && released3)
  {
      released3 = false;

    toggle4 = false;
    if (!toggle3){
      flywheel.moveVelocity(600);
      toggle3 = true;
    } else if (toggle3){
      flywheel.moveVelocity(0);
      toggle3 = false;
    }
    
  }

   if (controller.getDigital(ControllerDigital::L2) == 0)
  {
      released4 = true;
  }

   if (controller.getDigital(ControllerDigital::L2) == 1 && released4){
 
 
    released4 = false;

    toggle3 = false;
    if (!toggle4){
      flywheel.moveVelocity(400);
      toggle4 = true;
    } else if (toggle4){
      flywheel.moveVelocity(0);
      toggle4 = false;
    }

  }
  
  if (toggle4) {
    double kP = 0.1;
    double ki = 0.0;
    double kd = 0.1;

    int error = 400-flywheel.getActualVelocity();
    int integral = integral + error;
    int derivative = error - prevError;
    int prevError = error;
    int p = error * kP;
    int i = integral * ki;
    int d = derivative * kd;

    flywheel.moveVelocity(400+p+i+d);
  }
}
  
/*  
   if (controller.getDigital(ControllerDigital::R1) == 0)
  {
      released = true;
  }
  if (controller.getDigital(ControllerDigital::L1) == 1 && released)
  {
    released = false;
    if (!secondPress){
      flywheel.moveVelocity(600);
      secondPress = true;
    } else if (secondPress){
      flywheel.moveVelocity(0);
      secondPress = false;
    }
  }
}
  
  else if (controller.getDigital(ControllerDigital::L2) == 1)
  {
    flywheelState = 2;
  }
  switch (flywheelState)
  {
    case 1:
      flywheel.moveVelocity(600);
      break;
    case 2:
      flywheel.moveVelocity(0);
      break;
  }
}
*/