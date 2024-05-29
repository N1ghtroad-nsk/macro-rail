#pragma once

#include "Display.h"

class Display1602 : public IDisplay {
public:
  Display1602(LiquidCrystal_I2C & lcd);
  virtual void init();
  virtual void printTitle(const String & title);
  virtual void printValue(const String & value);
private:
  void display();
  LiquidCrystal_I2C & m_lcd;

  String m_title;
  String m_value;
};