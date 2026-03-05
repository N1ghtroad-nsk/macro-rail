#include "PhotoSettings.h"
#include "Globals.h"
#include <EEPROM.h>
#define CURRENT_VERSION 3

void PhotoSettings::saveToEEPROM(int addr) {
  EEPROM.put(addr, CURRENT_VERSION);
  EEPROM.put(addr+sizeof(int), *this);
}

void PhotoSettings::loadFromEEPROM(int addr) {
  int version;
  EEPROM.get(addr, version);
  if (version == CURRENT_VERSION)
    EEPROM.get(addr+sizeof(int), *this);
}

void Settings::finalize() {
  nFrames = abs(g_stepper.stepsToMm(toPosition) / photoSettings.frameDepth);
  if (nFrames == 0)
    nFrames = 1;
}
