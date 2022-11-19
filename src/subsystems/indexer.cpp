#include "main.h"
#include "okapi/impl/device/button/controllerButton.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "ports.hpp"

using namespace okapi;

pros::ADIDigitalOut indexer(indexerPort);

ControllerButton indexerButton = ControllerButton(ControllerDigital::A);

void updateIndexer() {
  if (indexerButton.changedToPressed()) {
    indexer.set_value(false);
    pros::delay(1000);
    indexer.set_value(true);
  }
}