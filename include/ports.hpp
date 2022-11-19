#include "main.h"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/potentiometer.hpp"
#include <chrono>

//drivetrain ports
using namespace okapi;

const int rightFrontPort = 5;
const int rightTopPort = 4;
const int rightBottomPort = 13;

const int leftFrontPort = 11;
const int leftTopPort = 19;
const int leftBottomPort = 18;

//conveyor port
const int conveyorPort = 9;

//flywheel port
const int flywheelPort = 7;

// indexer port
const char indexerPort = 'H';

const std::uint8_t visionSensorPort = 'G'; // 8

// sensor ports

const std::uint8_t encoderLPort1 = 'A';
const std::uint8_t encoderLPort2 = 'B';
const std::uint8_t encoderRPort1 = 'C';
const std::uint8_t encoderRPort2 = 'D';
const std::uint8_t encoderCPort1 = 'E';
const std::uint8_t encoderCPort2 = 'F';

//const int inertialPort = 20;
//const int inertial2Port = 5;
const std::uint8_t imuPort = 20;
