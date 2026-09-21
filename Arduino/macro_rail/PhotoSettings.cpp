#include "PhotoSettings.h"
#include "Globals.h"
#include <EEPROM.h>
#define CURRENT_VERSION 4

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
  // кадров на одно больше, чем интервалов: снимаем и в начальной, и в конечной точке включительно;
  // epsilon защищает от погрешности float (напр. 9.99997 вместо 10.0)
  float intervals = 0;
  if (photoSettings.frameDepth != 0)
    intervals = abs(g_stepper.stepsToMm(toPosition) / photoSettings.frameDepth);
  nFrames = int(intervals + 0.001) + 1;
}
