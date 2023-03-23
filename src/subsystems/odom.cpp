#include "drive.hpp"
#include "intake.hpp"
#include "main.h"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/device/rotarysensor/continuousRotarySensor.hpp"
#include "okapi/impl/control/iterative/iterativeControllerFactory.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#include "okapi/impl/device/rotarysensor/integratedEncoder.hpp"
#include "ports.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <iterator>
#include <string>

const double PI = 3.141592653589793238462643383279502884L;
#define toRadian(theta) (PI / 180 * theta)

static const double WHEEL_RADIUS = 1.375; // hi
const double T_l = 2.5;
const double T_r = 3.75;
const double T_c = 1;

IMU imu1(imuPort1, IMUAxes::y);

double posX = 0;
double posY = 0;

double prevLeft = 0;
double prevRight = 0;
// double prevCenter = 0;

int theta = 0;

double thetaPrev = 0;

bool isPressed = false;


okapi::IntegratedEncoder leftEncoder = IntegratedEncoder(leftTopPort, false);
okapi::IntegratedEncoder rightEncoder = IntegratedEncoder(rightTopPort, true);

void setChassisBrakeMode(AbstractMotor::brakeMode mode) {
  drive->getModel()->setBrakeMode(mode);
}

void resetImu(bool print = true) {
  imu1.reset();
  int time = pros::millis();
  int iter = 0;
  while (imu1.isCalibrating()) {
    if (print) {
      printf("IMU Calibrating... %d [ms]\n", iter);
    }
    iter += 100;
    if (iter >= 2500) { // IMU should not take more than 2500 ms to calibrate
      if (print) {
        printf("IMU Calibration Failed!\n");
      }
      break;
    }
    pros::delay(100);
  }
  if (print) {
    printf("IMU Calibrated in %d [ms]\n", iter - time);
  }
}

void imuInnit() {
  setChassisBrakeMode(AbstractMotor::brakeMode::brake);
  resetImu();
}

void IEInnit() {
  //left
  pros::c::motor_set_encoder_units(leftTopPort, pros::E_MOTOR_ENCODER_DEGREES);
 
  //right
  pros::c::motor_set_encoder_units(rightTopPort, pros::E_MOTOR_ENCODER_DEGREES);
  
  leftEncoder.reset();
  rightEncoder.reset();
}


void gyroPID(float degree, bool CW, int ms) {
  float taredRotation = imu1.get();
  int timer = 0;
  float prevError = 0;
  float integral = 0;
  while (timer < ms) {
    float sensorVal = imu1.get() - taredRotation;
    float error = degree - sensorVal;
    if (error < .1){break;}
    float derivative = error - prevError;
    prevError = error;
    integral += error;
    float power = (0.012 * error) + (0.00 * integral) + (0.01 * derivative);
    if (CW) {
      drive->getModel()->tank(power, -1.0f * power);
    } else {
      drive->getModel()->tank(-1.0f * power, power);
    }
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}


void turnPIDOdomCC(float leftTarget, float rightTarget, int ms, float maxV) {
  leftEncoder.reset();
  rightEncoder.reset();
  float currentLeftTravel = 0; // [in]
  float currentRightTravel = 0; // [in]
  float leftTargetTravel = currentLeftTravel + leftTarget;    // [in]
  float rightTargetTravel = currentRightTravel + rightTarget; // [in]
  float prevErrorL = 0;
  float prevErrorR = 0;
  float integralL = 0;
  float integralR = 0;
  int timer = 0;
  while (timer < ms ) { // Within time limit, increment PID loop``
    // Compute PID values from current wheel travel measurements
    currentLeftTravel = (-1* leftEncoder.get() * (4 * M_PI)) /840.0f;
    currentRightTravel = (rightEncoder.get() * (4 * M_PI) /840.0f);
    // currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;
    // currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f;
    float errorL = leftTargetTravel - currentLeftTravel;
    float errorR = rightTargetTravel - currentRightTravel;
    if (errorL < .1 || errorR < .1){break;}
    integralL += errorL;
    integralR += errorR;
    float derivativeL = errorL - prevErrorL;
    float derivativeR = errorR - prevErrorR;
    prevErrorL = errorL;
    prevErrorR = errorR;
    // Calculate power using PID
    float powerL = (0.1 * errorL) + (0 * integralL) + (0.01 * derivativeL);
    float powerR = (0.1 * errorR) + (0 * integralR) + (0.01 * derivativeR);
    drive->getModel()->tank(-1*powerL * maxV, powerR * maxV);
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}

void turnPIDOdomCW(float leftTarget, float rightTarget, int ms, float maxV) {
  leftEncoder.reset();
  rightEncoder.reset();
  float currentLeftTravel = 0; // [in]
  float currentRightTravel = 0; // [in]
  float leftTargetTravel = currentLeftTravel + leftTarget;    // [in]
  float rightTargetTravel = currentRightTravel + rightTarget; // [in]
  float prevErrorL = 0;
  float prevErrorR = 0;
  float integralL = 0;
  float integralR = 0;
  int timer = 0;
  while (timer < ms ) { // Within time limit, increment PID loop``
    // Compute PID values from current wheel travel measurements
    currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) /840.0f;
    currentRightTravel = (-1*rightEncoder.get() * (4 * M_PI) /840.0f);
    // currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;
    // currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f;
    float errorL = leftTargetTravel - currentLeftTravel;
    float errorR = rightTargetTravel - currentRightTravel;
    if (errorL < .1 || errorR < .1){break;}
    integralL += errorL;
    integralR += errorR;
    float derivativeL = errorL - prevErrorL;
    float derivativeR = errorR - prevErrorR;
    prevErrorL = errorL;
    prevErrorR = errorR;
    // Calculate power using PID
    float powerL = (0.1 * errorL) + (0 * integralL) + (0.01 * derivativeL);
    float powerR = (0.1 * errorR) + (0 * integralR) + (0.01 * derivativeR);
    drive->getModel()->tank(powerL * maxV, -1*powerR * maxV);
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}

void movePIDOdom(float leftTarget, float rightTarget, int ms, float maxV) {
  leftEncoder.reset();
  rightEncoder.reset();
  float currentLeftTravel = 0; // [in]
  float currentRightTravel = 0; // [in]
  float leftTargetTravel = currentLeftTravel + leftTarget;    // [in]
  float rightTargetTravel = currentRightTravel + rightTarget; // [in]
  float prevErrorL = 0;
  float prevErrorR = 0;
  float integralL = 0;
  float integralR = 0;
  int timer = 0;
  while (timer < ms ) { // Within time limit, increment PID loop``
    // Compute PID values from current wheel travel measurements
    currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) /840.0f;
    currentRightTravel = (rightEncoder.get() * (4 * M_PI) /840.0f);
    // currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;
    // currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f;
    float errorL = leftTargetTravel - currentLeftTravel;
    float errorR = rightTargetTravel - currentRightTravel;
    if (errorL < .1 || errorR < .1){break;}
    integralL += errorL;
    integralR += errorR;
    float derivativeL = errorL - prevErrorL;
    float derivativeR = errorR - prevErrorR;
    prevErrorL = errorL;
    prevErrorR = errorR;
    // Calculate power using PID
    float powerL = (0.1 * errorL) + (0 * integralL) + (0.01 * derivativeL);
    float powerR = (0.1 * errorR) + (0 * integralR) + (0.01 * derivativeR);
    drive->getModel()->tank(powerL * maxV, powerR * maxV);
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}

void movePIDOdomR(float leftTarget, float rightTarget, int ms, float maxV) {
  leftEncoder.reset();
  rightEncoder.reset();
  float currentLeftTravel = 0; // [in]
  float currentRightTravel = 0; // [in]
  float leftTargetTravel = currentLeftTravel + leftTarget;    // [in]
  float rightTargetTravel = currentRightTravel + rightTarget; // [in]
  float prevErrorL = 0;
  float prevErrorR = 0;
  float integralL = 0;
  float integralR = 0;
  int timer = 0;
  while (timer < ms ) { // Within time limit, increment PID loop``
    // Compute PID values from current wheel travel measurements
    currentLeftTravel = (-1*leftEncoder.get() * (4 * M_PI)) /840.0f;
    currentRightTravel = (-1*rightEncoder.get() * (4 * M_PI) /840.0f);
    // currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;
    // currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f;
    float errorL = leftTargetTravel - currentLeftTravel;
    float errorR = rightTargetTravel - currentRightTravel;
    if (errorL < .1 || errorR < .1){break;}
    integralL += errorL;
    integralR += errorR;
    float derivativeL = errorL - prevErrorL;
    float derivativeR = errorR - prevErrorR;
    prevErrorL = errorL;
    prevErrorR = errorR;
    // Calculate power using PID
    float powerL = (0.1 * errorL) + (0 * integralL) + (0.01 * derivativeL);
    float powerR = (0.1 * errorR) + (0 * integralR) + (0.01 * derivativeR);
    drive->getModel()->tank(powerL * maxV * -1, powerR * maxV * -1);
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}

// void movePIDOdom(float leftTarget, float rightTarget, float leftMultiplier, float rightMultiplier, int ms, float maxV) {
//   leftEncoder.reset();
//   rightEncoder.reset();
//   float currentLeftTravel = 0; // [in]
//   float currentRightTravel = 0; // [in]
//   int timer = 0;

//   while (timer < ms ) { // Within time limit, increment PID loop
//     // Compute PID values from current wheel travel measurements
//     currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) /840.0f;
//     currentRightTravel = (rightEncoder.get() * (4 * M_PI) /840.0f);

//     float errorL = (leftTarget-currentLeftTravel) * leftMultiplier;
//     float errorR = (rightTarget-currentRightTravel) * rightMultiplier;

//     float powerL = 1;
//     float powerR = 1;

//     if (errorL <= 10) {
//         powerL = errorL * 0.1;
//     }
//     if (errorR <= 10) {
//         powerR = errorR * 0.1;
//     }

//     if (errorL < 0 && errorR < 0) {
//         break;
//     }

//     drive->getModel()->tank(powerL * maxV * leftMultiplier, powerR * maxV * rightMultiplier);
//     timer += 10;
//     pros::delay(10);
//   }
//   drive->stop();
// }


// void updateOdom() {
//     // get rotations number of rotations to find distane each wheel travels
//     double deltaLeft = (leftEncoder.get() - prevLeft)*PI/360 *
//     2*WHEEL_RADIUS; double deltaRight = (rightEncoder.get() -
//     prevRight)*PI/360 * 2*WHEEL_RADIUS; double deltaCenter =
//     (centerEncoder.get() - prevCenter)*PI/360 * 2*WHEEL_RADIUS;

//     // double theta = toRadian(90 - imu.get());
//     // double deltaTheta = theta - thetaPrev;
//     double deltaTheta = (drltaLeft - deltaRight)/(T_r*2);
//     theta += deltaTheta*180/pi;
//     theta %= 360;

//     // radius to center of bot
//     double radiusY = deltaRight/deltaTheta + T_r;
//     double radiusX = deltaCenter/deltaTheta + T_c;

//     // chaning from polar to cartesian

//     if (deltaTheta == 0) {
//         posX = deltaCenter; // radius*cos(deltaTheta);
//         posY = deltaRight; // radius*sin(deltaTheta);
//     }
//     else {
//         posX += radiusX*sin(deltaTheta);
//         posY += radiusY*sin(deltaTheta);
//     }

//     if (controller.getDigital(ControllerDigital::up) == 1) {
//         pros::lcd::set_text(4, std::to_string(posX));
//         pros::lcd::set_text(5, std::to_string(posY));
//         pros::lcd::set_text(6, std::to_string(theta));
//     }

//     double prevLeft = leftEncoder.get();
//     double prevRight = rightEncoder.get();
//     double prevBack = centerEncoder.get();
//     // thetaPrev = theta;

// }
