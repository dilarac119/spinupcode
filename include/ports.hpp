#include "main.h"
#include "okapi/impl/device/rotarysensor/IMU.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#include <chrono>

// drivetrain ports
using namespace okapi;

// big boy d-train ports
const std::uint8_t rightFrontPort = 20;
const std::uint8_t rightTopPort = 11;
const std::uint8_t rightBottomPort = 18;

const std::uint8_t leftFrontPort = 2;
const std::uint8_t leftTopPort = 3;
const std::uint8_t leftBottomPort = 13;

// conveyor/intake sucky sucky port
const std::uint8_t conveyorPort = 21;

// flywheel shooty port
const std::uint8_t flywheelPort = 7;

// sensors later
const std::uint8_t encoderLPort1 = 'H';
const std::uint8_t encoderLPort2 = 'G';
const std::uint8_t encoderRPort1 = 'A';
const std::uint8_t encoderRPort2 = 'B';
const std::uint8_t encoderCPort1 = 'C';
const std::uint8_t encoderCPort2 = 'D';
const std::uint8_t visionSensorPort = 'G'; 

const std::uint8_t imuPort1 = 9;
const std::uint8_t imuPort2 = 17;

// pnuematics if we use
const std::uint8_t indexerPort = 'F';
const std::uint8_t expansionPort = 'E';


const std::uint8_t opticalPort = 4;
