#pragma once

#include "Display.h"

class Display1602 : public IDisplay {
public:
  Display1602(LiquidCrystal_I2C & lcd);
  virtual void init();
  virtual void printTitle(const __FlashStringHelper * title);
  virtual void printValue(const char * value);
  virtual void printValue(const __FlashStringHelper * value);
private:
  void display();
  LiquidCrystal_I2C & m_lcd;

  char m_title[LCD_LINE_SIZE];
  char m_value[LCD_LINE_SIZE];
};
