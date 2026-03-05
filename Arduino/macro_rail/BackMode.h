#pragma once

#include <Arduino.h>
#include "Mode.h"

class BackMode : public Mode {
public:
  BackMode() : Mode(F("Back")){}
private:
};