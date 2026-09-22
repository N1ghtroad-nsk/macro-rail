#pragma once

#include <Arduino.h>
#include "Globals.h"
#include "ModeEnc.h"

class PhotoStage {
public:
  virtual bool inProgress() = 0;
  virtual bool isBroken(const __FlashStringHelper * & message) {return false;}
  virtual void cancel() {}
};

class TimeoutStage : public PhotoStage {
public:
  virtual void start(int timeout) {
    long current = millis();
    m_finmillis = current + timeout;
    m_overflow = m_finmillis < current;
  }

  virtual bool inProgress() override {
    long current = millis();
    return m_overflow ? m_finmillis < current : m_finmillis > current;
  }

private:
  long m_finmillis;
  bool m_overflow;
};

class ExposureStage : public TimeoutStage {
public:
  // shot() returns immediately (lines raised), so the timeout is armed
  // right AFTER it: the exposure window is counted from the moment of the press
  void start(int timeout) override {
    g_camera.shot(g_settings.photoSettings.cameraShotDelay);
    TimeoutStage::start(timeout);
  }
};

class MoveStage : public PhotoStage {
public:
  void cancel() override {
    g_stepper.stop();
  }

  void start(float distance) {
    g_stepper.rotateDistance(distance);
  }

  bool inProgress() override {
    return g_stepper.stepsLeft() != 0;
  }

  bool isBroken(const __FlashStringHelper * & message) override {
    if (g_stepper.endstopHit()) {
      message = F("Endstop");
      return true;
    }
    return false;
  }
};

class InitStage : public MoveStage {
public:
  void start(long fromPosition, long toPosition, int timeoutSecs, int frames) {
    BufPrint b;
    b.print(abs(g_stepper.stepsToMm(toPosition - fromPosition)));
    b.print(F("mm,"));
    b.print(abs(int(frames)));
    b.print(F("f"));
    strlcpy(m_displaySettings, b.buf, sizeof(m_displaySettings));

    m_secsLeft = timeoutSecs;

    long current = millis();
    m_relmillis = current + 1000;
    m_overflow = m_relmillis < current;

    g_stepper.setSpeed(120);
    g_stepper.goToPosition(fromPosition);
    display();
  }

  bool inProgress() override {
    long current = millis();
    if (g_stepper.stepsLeft() != 0) {
      m_relmillis = current + 1000;
      m_overflow = m_relmillis < current;
    } else { 
      if (m_overflow ? m_relmillis > current : m_relmillis < current) {
        m_secsLeft--;
        m_relmillis = current + 1000;
        m_overflow = m_relmillis < current;
        display();
      }
    }
    if (m_secsLeft > 0 || g_stepper.stepsLeft() != 0)
      return true;
    g_stepper.setSpeed(10);
    return false;
  }

  void display() {
    BufPrint b;
    b.print(m_displaySettings);
    for (int i = 0; i < m_secsLeft; ++i)
      b.print('.');
    g_display.printValue(b.buf);
  }
private:
  long m_relmillis;
  int m_secsLeft;
  bool m_overflow;
  char m_displaySettings[LCD_LINE_SIZE];
};

class PhotoMode : public ModeEnc {
public:
  PhotoMode();
  void initImpl() override;
  void updateImpl() override;

protected:
  void onClick() override;
  void onTurn(int dir) override;
  void display();
  void nextStage();
  void stopProcess(const __FlashStringHelper * reason);
  void updateMinMax();

private:
  enum Stage {
    stCalm,
    stExposure,
    stCountMove,
    stLoop,
    stDone,
    stInit
  } m_currentStage;

  InitStage initStage;
  TimeoutStage timeoutStage;
  ExposureStage exposureStage;
  MoveStage moveStage;
  
  PhotoStage * m_currentStageWorker;
  int m_framesShot;
  const __FlashStringHelper * m_message = nullptr;

  int m_addition;
  long m_minPosition, m_maxPosition;
  long m_fromPosition;
  float m_frameDepth;
  int m_nFrames;
};
