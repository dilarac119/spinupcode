#include "main.h"
#include "okapi/impl/device/rotarysensor/IMU.hpp"

okapi::Controller controller;
okapi::Rate rate;

//pros::Vision visionSensor(visionSensorPort);

 okapi::IMU imu = IMU(imuPort, IMUAxes::y);
