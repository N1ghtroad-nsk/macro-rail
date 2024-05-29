#pragma once

#include <Arduino.h>
#include "Globals.h"
#include "FastManualMode.h"

class ToPositionMode : public FastManualMode {
public:
  ToPositionMode() : FastManualMode(F("To position")) {};
  
protected:
  void onApply() override {g_settings.toPosition = g_stepper.getPosition();};
};