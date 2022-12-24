#include "auton.hpp"
#include "flywheel.hpp"
#include "indexer.hpp"
#include "intake.hpp"
#include "main.h"
#include "okapi/api/chassis/controller/odomChassisController.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "pros/motors.hpp"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include <cstdio>



// void flywheelShoot(int count, float target){
//   int cnt = 0;
//   flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
//   static const float rpmWindow = 75.0f;
//   while (cnt < count) {
//     if (target == 0) {
//       flywheel.moveVoltage(0);
//       continue;
//     }
//     float error = target - (flywheel.getActualVelocity() * 5.0f);
//     if (error > (target - rpmWindow)) {
//       flywheel.moveVoltage(12000);
//     } else if (error <= -rpmWindow) {
//       flywheel.moveVoltage(0);
//     } else { // Within threshold window -> Use Feedforward and P Controller
//       flywheel.moveVoltage((target * 4.0f) + (error * 1.125f));
//     }
//     pros::delay(20);

//   }

//  cnt = 0;
// }

// void flywheelShoot(int count) {
//   int cnt = 0;
//   int prevError = 0;
//   while (cnt < count) {
//     double kP = 0.03;
//     double ki = 0.0;
//     double kd = 0.1;

//     int error = 450 - flywheel.getActualVelocity();
//     int integral = integral + error;
//     int derivative = error - prevError;
//     int prevError = error;
//     int p = error * kP;
//     int i = integral * ki;
//     int d = derivative * kd;

//     flywheel.moveVelocity(500 + p + i + d);

//     if (error - 25 <= 0 || error + 25 >= 0) {
//       indexer.set_value(false);
//       pros::delay(700);
//       indexer.set_value(true);
//       cnt++;
//     }

//     pros::delay(10);
//   }

//   cnt = 0;
// }


void autonFlywheel(float rpm) {
  flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
  static const float rpmWindow = 75.0f;
  float target = rpm;
  while (true) {
    if (target == 0) {
      flywheel.moveVoltage(0);
      continue;
    }
    float error = target - (flywheel.getActualVelocity() * 5.0f);
    if (error > (target - rpmWindow)) {
      flywheel.moveVoltage(12000);
    } else if (error <= -rpmWindow) {
      flywheel.moveVoltage(0);
    } else { // Within threshold window -> Use Feedforward and P Controller
      flywheel.moveVoltage((target * 4.0f) + (error * 1.125f));
      indexer.set_value(true);
      pros::delay(700);
      indexer.set_value(false);
    }
    pros::delay(20);
  }
}

void autonIndexer() {
    indexer.set_value(true);
    pros::delay(500);
    indexer.set_value(false);
}

bool isRed(double hue) {
  bool red = false;
  if (hue >= 0 && hue <= 30) {
    red = true;
  }
  return red;
}

void rollerRed() {
  if (!isRed(opticalSensor.get_hue())) {
    conveyor.moveVelocity(600);
    pros::delay(400);
    conveyor.moveVelocity(0);
    pros::delay(100);
    if (isRed(opticalSensor.get_hue())) {
      conveyor.moveVelocity(-600);
      pros::delay(200);
      conveyor.moveVelocity(0);
      pros::delay(100);
    }
    conveyor.moveVelocity(600);
    pros::delay(100);
    conveyor.moveVelocity(0);
    pros::delay(20);
  } else {
    conveyor.moveVelocity(-600);
    pros::delay(300);
    conveyor.moveVelocity(0);
    pros::delay(100);
    if (!isRed(opticalSensor.get_hue())) {
      conveyor.moveVelocity(600);
      pros::delay(200);
      conveyor.moveVelocity(0);
      pros::delay(100);
    }
    conveyor.moveVelocity(-600);
    pros::delay(100);
    conveyor.moveVelocity(0);
    pros::delay(20);
  }
  pros::delay(20);
}

void rollerBlue() {
  if (!isRed(opticalSensor.get_hue())) {
    conveyor.moveVelocity(-600);
    pros::delay(400);
    conveyor.moveVelocity(0);
    pros::delay(100);
    if (isRed(opticalSensor.get_hue())) {
      conveyor.moveVelocity(600);
      pros::delay(200);
      conveyor.moveVelocity(0);
      pros::delay(100);
    }
    conveyor.moveVelocity(-600);
    pros::delay(100);
    conveyor.moveVelocity(0);
    pros::delay(20);
  } else {
    conveyor.moveVelocity(600);
    pros::delay(400);
    conveyor.moveVelocity(0);
    pros::delay(100);
    if (!isRed(opticalSensor.get_hue())) {
      conveyor.moveVelocity(-600);
      pros::delay(200);
      conveyor.moveVelocity(0);
      pros::delay(100);
    }
    conveyor.moveVelocity(600);
    pros::delay(100);
    conveyor.moveVelocity(0);
    pros::delay(20);
  }
  pros::delay(20);
}

void rollUntilColor(int color) {
  // red = 1, blue = 2

  if (color == 1) {
    rollerRed();
  } else if (color == 2) {
    rollerBlue();
  }
}




void autonDirect(int color) {
  
  drive->getModel()->tank(.2, .2);
  rollUntilColor(color);
  pros::delay(1000);
  drive->getModel()->tank(0, 0);
  pros::delay(20);
  driveForward(0.75, true);
  rotate(45);


  //-------------------------- works^
}
  //gyroPID(90, true, 400);
  

  // pros::delay(20);
  // driveForward(0.75, true);
  // pros::delay(20);
  // rotate(90);
  // pros::delay(20);
  
  //driveForward(3, true);
 // pros::delay(20);

  // autonFlywheel();


// void autonIndirect() {

//   driveForward(1, false);
//   pros::delay(20);
//   rotate(90);
//   pros::delay(20);
//   driveForward(1, false);
//   pros::delay(20);
//   // roller();
//   pros::delay(20);
//   driveForward(1, true);
//   pros::delay(20);
//   rotate(-90);
//   pros::delay(20);
//   driveForward(3, true);
//   pros::delay(20);
//   autonFlywheel();
// }
