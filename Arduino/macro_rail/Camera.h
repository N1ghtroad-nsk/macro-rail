#pragma once

#include <Arduino.h>

class Camera {
  public :
  virtual void setup() = 0;
  // raises the lines and returns immediately; they are lowered on their own
  // in update() after delayMsec. If called too early (lines still raised, or
  // resting in low for less than delayMsec) it waits, driving update() internally
  virtual void shot(unsigned int delayMsec) = 0;
  // polled from the main loop()
  virtual void update() = 0;
};

template <byte FOCUS, byte SHUTTER>
class CameraT : public Camera {
public:
  void setup() {
    pinMode(FOCUS, OUTPUT);
    pinMode(SHUTTER, OUTPUT);
    digitalWrite(FOCUS, LOW);
    digitalWrite(SHUTTER, LOW);
  }

  void shot(unsigned int delayMsec) {
    m_delay = delayMsec;
    // between presses the lines must rest in low for at least delayMsec
    while (m_pressed || millis() - m_lowMillis < (unsigned long)delayMsec)
      update();
    digitalWrite(FOCUS, HIGH);
    digitalWrite(SHUTTER, HIGH);
    m_highMillis = millis();
    m_pressed = true;
  }

  void update() override {
    if (m_pressed && millis() - m_highMillis >= (unsigned long)m_delay) {
      digitalWrite(SHUTTER, LOW);
      digitalWrite(FOCUS, LOW);
      m_pressed = false;
      m_lowMillis = millis();
    }
  }

private:
  unsigned long m_highMillis = 0;
  unsigned long m_lowMillis = 0;
  unsigned int m_delay = 0;
  bool m_pressed = false;
};
