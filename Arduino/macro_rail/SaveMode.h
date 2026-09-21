#pragma once

#include <Arduino.h>
#include "ModeEnc.h"
#include "Globals.h"

class SaveMode : public ModeEnc {
public:
  SaveMode() :
    ModeEnc("Save settings")
  {
  }

  void initImpl() {
    g_settings.photoSettings.saveToEEPROM(g_photoSettingsAddress);
    displayValue("Done!");
  }
};