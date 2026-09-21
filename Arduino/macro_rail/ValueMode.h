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
    if (dir > 0) {
      m_currentValue += m_step;
    } else if (m_currentValue >= m_step) {
      m_currentValue -= m_step;
    } else {
      // block wrap-around through zero (important for unsigned T)
      m_currentValue = 0;
    }
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
