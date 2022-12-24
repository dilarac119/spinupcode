#pragma once
#include "main.h"

using namespace okapi;

extern pros::ADIDigitalOut indexer;
extern pros::ADIDigitalIn indexerButton;

void indexerInit();
void updateIndexer();