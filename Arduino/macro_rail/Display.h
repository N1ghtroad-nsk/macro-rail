#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 16x2: a line is at most 16 chars + '\0'
#define LCD_LINE_SIZE 17

class IDisplay {
public:
  virtual void init() = 0;
  // title/value from flash (F("...")) or RAM (BufPrint buf) — never bare literals!
  virtual void printTitle(const __FlashStringHelper * title) = 0;
  virtual void printValue(const char * value) = 0;
  virtual void printValue(const __FlashStringHelper * value) = 0;
};

// Builds a string into a fixed RAM buffer without dynamic allocation.
// Extends Print, so print(int)/print(float)/print(F("...")) etc. all work.
class BufPrint : public Print {
public:
  char buf[LCD_LINE_SIZE];

  BufPrint() { restart(); }
  void restart() { m_pos = 0; buf[0] = '\0'; }

  virtual size_t write(uint8_t c) {
    if (m_pos < sizeof(buf) - 1) {
      buf[m_pos++] = (char)c;
      buf[m_pos] = '\0';
    }
    return 1;
  }

private:
  size_t m_pos;
};
