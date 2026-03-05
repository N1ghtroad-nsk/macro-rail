#pragma once

#include <Arduino.h>
#include <TimerOne.h>

#define DERATTLING_DELAY 100

class Stepper {
public:
  virtual void init() = 0;    
  virtual void enable() = 0;
  virtual void disable() = 0;
  virtual void setSpeed(int rpm) = 0;
  virtual void rotateSteps(long steps) = 0;
  virtual void startRotate(int direction) = 0;
  virtual void rotateDistance(float mm) = 0;  
  virtual void stop() = 0;
  virtual void update() = 0;
  virtual long stepsLeft() = 0;
  virtual bool endstopHit() = 0;

  virtual void resetPosition() = 0;
  virtual long getPosition() = 0;

  virtual double stepsToMm(long steps) = 0;
  virtual long mmToSteps(double mm) = 0;

  virtual void goToPosition(long position) = 0;
};

template <byte ENA, byte STEP, byte DIR, byte ENDSTOP, byte INTERP, int STEPSPERROT, int DISTPERROT>
class StepperT : public Stepper {
public:
  StepperT() {
  }

  void init() {
    pinMode(ENA, OUTPUT);
    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(ENDSTOP, INPUT_PULLUP);
    disable();
  }
    
  void enable() {
    stop();
    digitalWrite(ENA, LOW);
  }

  void disable() {
    stop();
    resetPosition();
    digitalWrite(ENA, HIGH);
  }

  void setSpeed(int rpm) {
    Timer1.setPeriod(60000000 / rpm / STEPSPERROT / INTERP);
  }

  void rotateSteps(long steps) {
    m_steps += steps;
    m_infinite = false;
  }

  void startRotate(int direction) {
    if (m_steps * direction < 0)
      m_steps = 0;
    else
      m_steps = direction;
    m_infinite = true;
  }

  void rotateDistance(float mm) {
    if (m_steps == 0)
      m_steps = mmToSteps(mm);
    m_infinite = false;
  }

  void stop() {
    m_steps = 0;
    m_infinite = false;
  }

  void update() {
    if (m_steps == 0)
      return;
    if (endstop() && m_steps < 0) {
      m_endstopped = true;
      stop();
    }
    if (m_steps != 0) {
      m_endstopped = false;
      digitalWrite(DIR, m_steps > 0 ? HIGH : LOW);
      digitalWrite(STEP, m_stepPin ? HIGH : LOW);
      m_stepPin = !m_stepPin;
      m_position += m_steps > 0 ? 1 : -1;
      if (!m_infinite)
        m_steps += m_steps > 0 ? -1 : 1;
    }
  }

  long stepsLeft() {
    return m_steps;
  };

  bool endstop() {
    const bool hit = digitalRead(ENDSTOP) == LOW;
    if (!hit)
      m_derattlingTime = millis();
    else if (millis() - m_derattlingTime > DERATTLING_DELAY || m_derattlingTime > millis())
      return true;
    return false;     
  }

  bool endstopHit() override {
    return m_endstopped;
  }

  virtual void resetPosition() {
    m_position = 0;
  };

  virtual long getPosition() {
    return m_position;
  };

  virtual double stepsToMm(long steps) {
    return double(steps) * DISTPERROT / STEPSPERROT / INTERP;
  }
  virtual long mmToSteps(double mm) {
    return mm/DISTPERROT*STEPSPERROT*INTERP;
  }

  virtual void goToPosition(long position) {
    stop();
    rotateSteps(position - m_position);
  }

private:
  long m_steps;
  long m_position;
  bool m_stepPin;
  bool m_infinite;
  bool m_endstopped;
  long m_derattlingTime = 0;
};
