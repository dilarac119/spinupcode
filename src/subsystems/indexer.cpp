#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

using namespace okapi;

pros::ADIDigitalOut indexer = pros::ADIDigitalOut(indexerPort);

ControllerButton indexerButton = ControllerButton(ControllerDigital::A);

void indexerInit() { indexer.set_value(false); }



void updateIndexer() {
  if (indexerButton.changedToPressed()) {
    indexer.set_value(true);
    pros::delay(100);
    indexer.set_value(false);
  }
}