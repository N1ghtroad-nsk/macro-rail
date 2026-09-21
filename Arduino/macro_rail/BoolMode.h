#pragma once

#include <Arduino.h>
#include "Globals.h"

class BoolMode : public ModeEnc {
public:
  BoolMode(const __FlashStringHelper * title, bool & value) :
    ModeEnc(title),
    m_value(value)
  {
  }

  void onTurn(int dir) {
    m_currentValue = dir > 0;
    display();
  }

  virtual void initImpl() {
    m_currentValue = m_value;
    display();
  }

  void onClick() {
    m_value = m_currentValue;
    ModeEnc::onClick();
  }

protected:
  virtual void display() {
    displayValue(m_currentValue ? F("On") : F("Off"));
  }

protected:
  bool & m_value;
  bool m_currentValue;
};
