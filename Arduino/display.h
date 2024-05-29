#pragma once

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class IDisplay {
public:
  virtual void init() = 0;    
  virtual void printTitle(const String & title) = 0;
  virtual void printValue(const String & value) = 0;
};