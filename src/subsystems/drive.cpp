
#include "auton.hpp"
#include "main.h"
#include "odom.hpp"
#include "okapi/impl/device/rotarysensor/potentiometer.hpp"

using namespace okapi;

Motor rightFront(rightFrontPort, false, AbstractMotor::gearset::blue,
                 AbstractMotor::encoderUnits::degrees);
Motor rightTop(rightTopPort, true, AbstractMotor::gearset::blue,
               AbstractMotor::encoderUnits::degrees);
Motor rightBottom(rightBottomPort, false, AbstractMotor::gearset::blue,
                  AbstractMotor::encoderUnits::degrees);

Motor leftFront(leftFrontPort, true, AbstractMotor::gearset::blue,
                AbstractMotor::encoderUnits::degrees);
Motor leftTop(leftTopPort, false, AbstractMotor::gearset::blue,
              AbstractMotor::encoderUnits::degrees);
Motor leftBottom(leftBottomPort, true, AbstractMotor::gearset::blue,
                 AbstractMotor::encoderUnits::degrees);

std::shared_ptr<OdomChassisController> drive =
    ChassisControllerBuilder()
        .withMotors({leftFront, leftTop, leftBottom},
                    {rightFront, rightTop, rightBottom})
        .withDimensions(AbstractMotor::gearset::blue,
                        {{4_in, 13.7_in}, imev5BlueTPR})
        .withSensors(
            // ADIEncoder{encoderLPort1, encoderLPort2}, // Left encoder
            // ADIEncoder{encoderRPort1, encoderRPort2},  // Right encoder
            // ADIEncoder{encoderCPort1, encoderCPort2, true}  // Center encoder
            // reversed
            leftFront.getEncoder(), rightFront.getEncoder())
        // Specify the tracking wheels diam (2.75 in), track (7 in), and TPR
        // (360)
        .withOdometry({{2.75_in, 7.5_in, 1_in, 2.75_in}, quadEncoderTPR})
        .buildOdometry();

static Controller controller = Controller();

void updateDrive() {
  drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
                          controller.getAnalog(ControllerAnalog::rightY));

  if (controller.getDigital(ControllerDigital::left) == 1) {
    leftFront.setBrakeMode(AbstractMotor::brakeMode::hold);
    leftTop.setBrakeMode(AbstractMotor::brakeMode::hold);
    leftBottom.setBrakeMode(AbstractMotor::brakeMode::hold);

    rightFront.setBrakeMode(AbstractMotor::brakeMode::hold);
    rightTop.setBrakeMode(AbstractMotor::brakeMode::hold);
    rightBottom.setBrakeMode(AbstractMotor::brakeMode::hold);
  } else if (controller.getDigital(ControllerDigital::right) == 1) {
    leftFront.setBrakeMode(AbstractMotor::brakeMode::coast);
    leftTop.setBrakeMode(AbstractMotor::brakeMode::coast);
    leftBottom.setBrakeMode(AbstractMotor::brakeMode::coast);

    rightFront.setBrakeMode(AbstractMotor::brakeMode::coast);
    rightTop.setBrakeMode(AbstractMotor::brakeMode::coast);
    rightBottom.setBrakeMode(AbstractMotor::brakeMode::coast);
  }

  if (controller.getDigital(ControllerDigital::Y) == 1) {
    // red = 1, blue = 2

    autonDirect(1);
  
    // roll 1

    driveForward(1.65, true);
    rotate(-90);
    driveForward(2, false);

    autonDirect(1);

    // roll 2

    // driveForward(2, true);
    // rotate(130);
    // driveForward(10.25, false);
    // rotate(90);
    // driveForward(2, false);

    // // // roll 3
    // driveForward(1.65, true);
    // rotate(179);
    // driveForward(2, false);

    // rotate(-65);

    // int cnt = 0;

    // int prevError = 0;
    // while (cnt < 1) {
    //     double kP = 0.03;
    //     double ki = 0.0;
    //     double kd = 0.1;

    //     int error = 450-flywheel.getActualVelocity();
    //     int integral = integral + error;
    //     int derivative = error - prevError;
    //     int prevError = error;
    //     int p = error * kP;
    //     int i = integral * ki;
    //     int d = derivative * kd;

    //     flywheel.moveVelocity(500+p+i+d);

    //     if (error - 25 <= 0 || error + 25 >= 0) {
    //         indexer.set_value(false);
    //         pros::delay(700);
    //         indexer.set_value(true);
    //         cnt++;
    //     }

    //     pros::delay(10);
    // }

    // cnt = 0;
    // pros::delay(2000);
    // rotate(180);
  }
}