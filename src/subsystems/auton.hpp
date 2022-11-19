#pragma once
#include "main.h"
#include "okapi/impl/device/opticalSensor.hpp"
#include "pros/motors.h"
#include "pros/optical.hpp"
#include "ports.hpp"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
//#include "okapi/impl/device/rotarysensor/potentiometer.hpp"


using namespace okapi;

extern Motor rightFront;
extern Motor rightTop;
extern Motor rightBottom;

extern Motor leftFront;
extern Motor leftTop;
extern Motor leftBottom;

extern std::shared_ptr<OdomChassisController> drive;

static pros::Optical opticalSensor = pros::Optical(opticalPort);

// void updateAuton();
//void setFlywheelState(FlywheelState state);
void gyroPID(float degree, bool CW, int ms = 1000);
void autonDirect(int color);
//void autonIndirect();