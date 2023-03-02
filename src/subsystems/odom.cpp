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
// IMU imu2(imuPort2, IMUAxes::z);n

double posX = 0;
double posY = 0;

double prevLeft = 0;
double prevRight = 0;
// double prevCenter = 0;

int theta = 0;

double thetaPrev = 0;

bool isPressed = false;

// okapi::ADIEncoder leftEncoder = ADIEncoder(encoderLPort1, encoderLPort2,
// false); okapi::ADIEncoder rightEncoder = ADIEncoder(encoderRPort1,
// encoderRPort2, true); okapi::ADIEncoder centerEncoder =
//     ADIEncoder(encoderCPort1, encoderCPort2, false);

okapi::IntegratedEncoder leftEncoder = IntegratedEncoder(leftBottomPort, true);
okapi::IntegratedEncoder rightEncoder =
    IntegratedEncoder(rightBottomPort, true);

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
  leftEncoder.reset();
  rightEncoder.reset();
}


void movePID(float leftTarget, float rightTarget, int ms, float maxV) {
  float degreesL = ((-leftTarget * 360.0f) / (M_PI * 4)) * 18;
  float degreesR = ((-rightTarget * 360.0f) / (M_PI * 4)) * 18;
  IterativePosPIDController drivePIDL =
      IterativeControllerFactory::posPID(0.2, 0.0, 0.00);
  IterativePosPIDController drivePIDR =
      IterativeControllerFactory::posPID(0.2, 0.0, 0.00);
  drive->getModel()->resetSensors();
  int timer = 0;
  float errorL;
  float errorR;
  float powerL;
  float powerR;
  while (timer < ms) { // Within time limit, increment PID loop
    errorL = degreesL + drive->getModel()->getSensorVals()[0];
    errorR = degreesR + drive->getModel()->getSensorVals()[1];
    powerL = drivePIDL.step(errorL);
    powerR = drivePIDR.step(errorR);
    drive->getModel()->tank(powerL * maxV, powerR * maxV);
    timer += 10;
    pros::delay(10);
  }
  drive->stop();
}

void gyroPID(float degree, bool CW, int ms) {
  float taredRotation = imu1.get();
  int timer = 0;
  float prevError = 0;
  float integral = 0;
  while (timer < ms) {
    float sensorVal = imu1.get() - taredRotation;
    float error = degree - sensorVal;
    float derivative = error - prevError;
    prevError = error;
    integral += error;
    float power = (0.2 * error) + (0.00 * integral) + (0.0 * derivative);
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
void movePIDOdom(float leftTarget, float rightTarget, int ms, float maxV) {
    float currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;   // [in]
    float currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f; // [in]
    float leftTargetTravel = currentLeftTravel + leftTarget;                                               // [in]
    float rightTargetTravel = currentRightTravel + rightTarget;                                            // [in]
    float prevErrorL = 0;
    float prevErrorR = 0;
    float integralL = 0;
    float integralR = 0;
    int timer = 0;
    while (timer < ms) { // Within time limit, increment PID loop
        // Compute PID values from current wheel travel measurements
        currentLeftTravel = (leftEncoder.get() * (4 * M_PI)) / 36000.0f;
        currentRightTravel = (rightEncoder.get() * (4 * M_PI)) / 36000.0f;
        float errorL = leftTargetTravel - currentLeftTravel;
        float errorR = rightTargetTravel - currentRightTravel;
        integralL += errorL;
        integralR += errorR;
        float derivativeL = errorL - prevErrorL;
        float derivativeR = errorR - prevErrorR;
        prevErrorL = errorL;
        prevErrorR = errorR;
        // Calculate power using PID
        float powerL = (0.5 * errorL) + (0 * integralL) + (0.0007 * derivativeL);
        float powerR = (0.5 * errorR) + (0 * integralR) + (0.0007 * derivativeR);
        drive->getModel()->tank(powerL * maxV, powerR * maxV);
        timer += 10;
        pros::delay(10);
    }
    drive->stop();
}

// double imuVal(double imu1, double imu2) {

//   double avg = (imu1 + imu2) / 2;
//   return avg;
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

// angle in degrees
void rotate(double targetAngle) {

  okapi::IterativePosPIDController rotatePID =
      okapi::IterativeControllerFactory::posPID(0.015, 0, 0.0001);

  rotatePID.setTarget(targetAngle);

  // double curAngle = drive->getState().theta.convert(okapi::degree);
  double curAngle = imu1.controllerGet();

  while (abs(targetAngle - curAngle) >= 3) {
    // curAngle = drive->getState().theta.convert(okapi::degree);
    curAngle = imu1.controllerGet();

    double vel = rotatePID.step(curAngle);

    drive->getModel()->tank(vel, -vel);

    pros::delay(20);
  }

  rotatePID.reset();

  // leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  // leftTop.setBrakeMode(AbstractMotor::brakeMode::hold);
  // leftBottom.setBrakeMode(AbstractMotor::brakeMode::hold);

  // rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
  // rightTop.setBrakeMode(AbstractMotor::brakeMode::hold);
  // rightBottom.setBrakeMode(AbstractMotor::brakeMode::hold);

  drive->getModel()->tank(0, 0);

  // pros::delay(1000);

  // leftFront.setBrakeMode(AbstractMotor::brakeMode::coast);
  // leftTop.setBrakeMode(AbstractMotor::brakeMode::coast);
  // leftBottom.setBrakeMode(AbstractMotor::brakeMode::coast);

  // rightFront.setBrakeMode(AbstractMotor::brakeMode::coast);
  // rightTop.setBrakeMode(AbstractMotor::brakeMode::coast);
  // rightBottom.setBrakeMode(AbstractMotor::brakeMode::coast);
}

// distance in feet
void driveForward(double distance, bool backwards) {

  okapi::IterativePosPIDController drivePID =
      okapi::IterativeControllerFactory::posPID(1.4, 0, 0.0007);

  const double target = distance;

  drivePID.setTarget(target);

  double orgPosX = drive->getState().x.convert(okapi::foot);
  double orgPosY = drive->getState().y.convert(okapi::foot);

  double distTravelled = 0;

  while (abs(target - distTravelled) >= 0.2) {
    double dx = drive->getState().x.convert(okapi::foot) - orgPosX;
    double dy = drive->getState().y.convert(okapi::foot) - orgPosY;

    distTravelled = sqrt(dx * dx + dy * dy);

    double vel = drivePID.step(distTravelled);

    if (backwards)
      vel = -vel;

    drive->getModel()->tank(vel*0.7, vel);

    pros::delay(10);
  }

  drivePID.reset();
  drive->getModel()->tank(0, 0);
}
