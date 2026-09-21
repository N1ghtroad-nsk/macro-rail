#pragma once

#include <ArduinoStl.h>
#include <Arduino.h>
#include "MenuMode.h"

class MainMenu : public MenuMode {
public:
  MainMenu();
  void finalize() override {};
};
