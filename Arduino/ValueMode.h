#pragma once

#include <Arduino.h>
#include "Globals.h"

template <typename T>
class ValueMode : public ModeEnc {
public:
  ValueMode(const String & title, const String & unit, T & value, T step) :
    ModeEnc(title),
    m_unit(unit),
    m_value(value),
    m_step(step)
  {
  }

  void onTurn(int dir) {
    m_currentValue += m_step * dir;
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
    String msg;
    msg.reserve(16);
    msg += m_currentValue;
    msg += " " + m_unit;
    displayValue(msg);
  }

protected:
  String m_unit;
  T & m_value;
  T m_currentValue;
  T m_step;
};
