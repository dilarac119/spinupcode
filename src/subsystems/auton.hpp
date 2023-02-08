#pragma once
#include "main.h"
#include "okapi/impl/device/opticalSensor.hpp"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#include "ports.hpp"
#include "pros/optical.hpp"
//#include "okapi/impl/device/rotarysensor/potentiometer.hpp"

using namespace okapi;

extern Motor rightFront;
extern Motor rightTop;
extern Motor rightBottom;

extern Motor leftFront;
extern Motor leftTop;
extern Motor leftBottom;

extern std::shared_ptr<OdomChassisController> drive;

//static pros::Optical opticalSensor = pros::Optical(opticalPort);

void gyroPID(float degree, bool CW, int ms = 1000);
void movePIDOdom(float leftTarget, float rightTarget, int ms, float maxV);

// roller stuff

bool isRed(double hue);
void rollerRed();
void rollerBlue();
void rollUntilColor(int color);
void autonRoller(int color);





extern void autonFlywheel();
extern void runFW();

void autonIndexer();
void skills(int color);

void driveForward(double distance, bool backwards = false);
void rotate(double angle);

void print1();


void autonDirect(int color);
void autonIndirect(int color);
