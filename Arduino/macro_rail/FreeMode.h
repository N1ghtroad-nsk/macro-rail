#pragma once

#include <Arduino.h>
#include "Globals.h"
class FreeMode : public ModeEnc {
public:
  FreeMode() :
    ModeEnc(F("Free stepper"))
  {
  }

  virtual void initImpl() {
    g_stepper.disable();
    displayValue(F("Done!"));
  }
};
