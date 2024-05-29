#pragma once

#include <Arduino.h>
#include "Globals.h"
#include "FastManualMode.h"

class FromPositionMode : public FastManualMode {
public:
  FromPositionMode() : FastManualMode(F("From position")) {};
  
protected:
  void onApply() override {
    g_settings.toPosition += g_stepper.getPosition();
    g_stepper.resetPosition();
  };
};