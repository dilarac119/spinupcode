#include "main.h"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/potentiometer.hpp"
#include <chrono>

// drivetrain ports
using namespace okapi;

const int rightFrontPort = 14;
const int rightTopPort = 18;
const int rightBottomPort = 19;

const int leftFrontPort = 10;
const int leftTopPort = 7;
const int leftBottomPort = 15;

// conveyor port
const int conveyorPort = 1;

// flywheel port
const int flywheelPort = 20;

// indexer port
const uint8_t indexerPort = 'A';

const std::uint8_t visionSensorPort = 'G'; // 8

// sensor ports

const std::uint8_t encoderLPort1 = 'A';
const std::uint8_t encoderLPort2 = 'B';
const std::uint8_t encoderRPort1 = 'C';
const std::uint8_t encoderRPort2 = 'D';
const std::uint8_t encoderCPort1 = 'E';
const std::uint8_t encoderCPort2 = 'F';

// const int inertialPort = 20;
// const int inertial2Port = 5;
const std::uint8_t imuPort = 20;
