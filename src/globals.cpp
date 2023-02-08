

#include "main.h"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "ports.hpp"

okapi::Controller controller;
okapi::Rate rate;

pros::Vision visionSensor(visionSensorPort);

//okapi::IMU imu = IMU(imuPort1, IMUAxes::y);