#include "Display1602.h"

Display1602::Display1602(LiquidCrystal_I2C & lcd) :
  m_lcd(lcd)
{
  m_title[0] = '\0';
  m_value[0] = '\0';
}

void Display1602::init() {
  m_lcd.clear();
}

void Display1602::printTitle(const __FlashStringHelper * title) {
  strlcpy_P(m_title, (const char *)title, sizeof(m_title));
  display();
}

void Display1602::printValue(const char * value) {
  strlcpy(m_value, value, sizeof(m_value));
  display();
}

void Display1602::printValue(const __FlashStringHelper * value) {
  strlcpy_P(m_value, (const char *)value, sizeof(m_value));
  display();
}

void Display1602::display() {
  m_lcd.clear();
  m_lcd.setCursor(0, 0);
  m_lcd.print(m_title);
  m_lcd.setCursor(0, 1);
  m_lcd.print(m_value);
}
