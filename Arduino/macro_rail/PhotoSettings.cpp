#include "PhotoSettings.h"
#include "Globals.h"
#include <EEPROM.h>
#define CURRENT_VERSION 5

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
  // frame count is intervals + 1: shooting at both start and end positions inclusive;
  float intervals = 0;
  if (photoSettings.frameDepth != 0)
    intervals = abs(g_stepper.stepsToMm(toPosition) / photoSettings.frameDepth);
  nFrames = int(intervals + 0.001) + 1;
}
