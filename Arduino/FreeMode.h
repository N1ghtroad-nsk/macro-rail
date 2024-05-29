#pragma once

#include <Arduino.h>
#include "Globals.h"
class FreeMode : public ModeEnc {
public:
  FreeMode() :
    ModeEnc("Free stepper")
  {
  }

  virtual void initImpl() {
    g_stepper.disable();
    displayValue("Done!");
  }
};
