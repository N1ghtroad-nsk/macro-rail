#pragma once

#include "Display1602.h"

Display1602::Display1602(LiquidCrystal_I2C & lcd) :
  m_lcd(lcd)
{
  m_title.reserve(16);
  m_value.reserve(16);
}

void Display1602::init() {
  m_lcd.clear();
}

void Display1602::printTitle(const String & title) {
  m_title = title;
  display();
}

void Display1602::printValue(const String & value) {
  m_value = value;
  display();
}

void Display1602::display() {
  m_lcd.clear();
  m_lcd.setCursor(0, 0);
  m_lcd.print(m_title);
  m_lcd.setCursor(0, 1);
  m_lcd.print(m_value);
}
