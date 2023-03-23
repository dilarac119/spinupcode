
#include "auton.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "intake.hpp"
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

    // // auton indirect

    // // move to roller
    // // activate flywheel
    // autonFlywheelMAX();
    // pros::delay(20);

    // movePIDOdom(24, 24, 50000, .9);
    // pros::delay(20);
    // turnPIDOdomCW(11.2, 11.2, 1000, 1);
    // pros::delay(20);

    // // creep
    // movePIDOdom(10, 10, 1000, 0.3);
    // pros::delay(30);

    // // roller
    // drive->getModel()->tank(0.2, 0.2);
    // conveyor.moveVoltage(-12000);
    // pros::delay(150);
    // conveyor.moveVoltage(0);
    // pros::delay(2000);
    // drive->getModel()->tank(0, 0);
    // pros::delay(200);

    // // move bakv
    // movePIDOdomR(5, 5, 500, 0.3);
    // pros::delay(20);
    // turnPIDOdomCW(4.8, 4.8, 800, 0.3);

    // // shoot

    // autonIndexer();
    // pros::delay(20);
    // pros::delay(3000);
    // autonIndexer();
    // pros::delay(10);
    // autonFlywheelOFF();

    // //---------------

    // // auton direct

  //   autonFlywheelMAX();

  //   // roller
  //   drive->getModel()->tank(0.2, 0.2);
  //   conveyor.moveVoltage(-12000);
  //   pros::delay(150);
  //   conveyor.moveVoltage(0);
  //   drive->getModel()->tank(0, 0);

  //  // move bakv
  //   movePIDOdomR(5, 5, 500, 0.3);
  //   pros::delay(20);
  //   turnPIDOdomCC(5.1, 5.1, 800, 0.3);

  //   // shoot
  //   pros::delay(4000);
  //   autonIndexer();
  //   pros::delay(20);
  //   pros::delay(3000);
  //   autonIndexer();
  //   pros::delay(10);
  //   autonFlywheelOFF();

    //-------------------------------

    // // skills

    // // roller 1
    // drive->getModel()->tank(0.2, 0.2);
    // conveyor.moveVoltage(-12000);
    // pros::delay(300);
    // conveyor.moveVoltage(0);
    // drive->getModel()->tank(0, 0);
    // pros::delay(20);

    // // back up
    // movePIDOdomR(24, 24, 1500, 0.3);
    // pros::delay(500);

    // // activate flywheel
    // autonFlywheelLOW();
    // pros::delay(200);

    // //turn to second roller
    // turnPIDOdomCW(10.8, 10.8, 1000, .75);
    // pros::delay(500);
    // // move forward
    // movePIDOdom(22, 22, 3000, 1);
    // pros::delay(300);

    // // second roller
    // drive->getModel()->tank(0.2, 0.2);
    // conveyor.moveVoltage(-12000);
    // pros::delay(300);
    // conveyor.moveVoltage(0);
    // drive->getModel()->tank(0, 0);
    // pros::delay(20);
    // movePIDOdomR(5, 5, 1500, 0.7);
    // pros::delay(20);

    // // turn
    // turnPIDOdomCC(10.35, 10.35, 1000, .8);
    // pros::delay(20);
    // movePIDOdomR(49, 46, 4000, 0.9);

    // // shoot
    // pros::delay(1000);
    // autonIndexer();
    // pros::delay(20);
    // autonFlywheelHALF();
    // pros::delay(1400);
    // autonIndexer();
    // pros::delay(10);
    // autonIndexer();
    // pros::delay(20);

    // // reverse
    // movePIDOdom(36, 36, 3000, 0.5);
    // pros::delay(20);

    // // turn diagonal
    // turnPIDOdomCC(15.8, 15.8, 1000, .9);
    // pros::delay(20);

    // //activate conveyor
    // conveyor.moveVoltage(20000);
    // pros::delay(200);
    // movePIDOdom(61, 61, 6000, 0.7);
    // pros::delay(2000);

    // //change flywheel speed
    // autonFlywheelFULL();

    // //turn to basket
    // turnPIDOdomCW(11.35, 11.35, 1200, .7);
    // pros::delay(280);

    // //shoot 2
    // autonIndexer();
    // pros::delay(900);
    // autonIndexer();
    // pros::delay(1000);
    // autonIndexer();
    // pros::delay(800);

    // //turn back
    // //kinda diagonal
    // turnPIDOdomCW(8, 8, 900, .7);
    // pros::delay(20);
    // movePIDOdomR(30, 30, 3000, 0.9);
    // pros::delay(500);
    // //paralel to roller
    // turnPIDOdomCW(9, 9, 900, .9);
    // pros::delay(200);
    // movePIDOdomR(42, 42, 3000, 0.9);
    // pros::delay(200);

    // // line up turn to roller 3
    // turnPIDOdomCW(10, 10, 1000, .9);
    // pros::delay(200);
    // movePIDOdom(20, 20, 1000, .2);
    // pros::delay(100);

    // // roller 3
    // drive->getModel()->tank(0.2, 0.2);
    // conveyor.moveVoltage(-12000);
    // pros::delay(300);
    // conveyor.moveVoltage(0);
    // drive->getModel()->tank(0, 0);
    // pros::delay(20);

    // // go back to 4
    // movePIDOdomR(21, 21, 1500, 0.7);
    // pros::delay(20);
    // // turn to roller 4
    // turnPIDOdomCW(11, 11, 1000, .9);
    // pros::delay(200);
    // movePIDOdom(21, 21, 2000, .3);
    // pros::delay(100);

    // // roller 4
    // drive->getModel()->tank(0.2, 0.2);
    // conveyor.moveVoltage(-12000);
    // pros::delay(300);
    // conveyor.moveVoltage(0);
    // autonFlywheelOFF();
    // drive->getModel()->tank(0, 0);
    // pros::delay(20);

    // // allign for expanison
    // movePIDOdomR(4, 4, 1500, 0.7);
    // pros::delay(20);
    // turnPIDOdomCC(6, 6, 1000, .9);
    // pros::delay(200);

    // //expansion
    // expansion.set_value(true);
    // pros::delay(50);
    // expansion.set_value(false);
  }
}