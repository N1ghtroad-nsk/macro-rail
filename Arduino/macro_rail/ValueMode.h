#pragma once

#include <Arduino.h>
#include "Globals.h"

template <typename T>
class ValueMode : public ModeEnc {
public:
  ValueMode(const __FlashStringHelper * title, const __FlashStringHelper * unit, T & value, T step) :
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
    BufPrint b;
    b.print(m_currentValue);
    b.print(' ');
    b.print(m_unit);
    displayValue(b.buf);
  }

protected:
  const __FlashStringHelper * m_unit;
  T & m_value;
  T m_currentValue;
  T m_step;
};
