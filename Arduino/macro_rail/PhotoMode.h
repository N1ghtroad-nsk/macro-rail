#pragma once

#include <Arduino.h>
#include "Globals.h"
#include "ModeEnc.h"

class PhotoStage {
public:
  virtual bool inProgress() = 0;
  virtual bool isBroken(String & message) {return false;}
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
  ~ExposureStage() {
    g_camera.release();
  }

  void start(int timeout) override {
    TimeoutStage::start(timeout);
    long current = millis();
    m_relmillis = current + 10;
    m_overflow = m_relmillis < current;
    g_camera.shot();
  }

  bool inProgress() override {
    long current = millis();
    if (m_overflow ? m_relmillis > current : m_relmillis < current)
      g_camera.release();
    return TimeoutStage::inProgress();
  }

  void cancel() override {
    g_camera.release();
  }

private:
  long m_relmillis;
  bool m_overflow;
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

  bool isBroken(String & message) override {
    if (g_stepper.endstopHit()) {
      message = F("Endstop");
      return true;
    }
  }
};

class InitStage : public MoveStage {
public:
  InitStage () {
    m_displaySettings.reserve(11);
  }

  void start(long fromPosition, long toPosition, int timeoutSecs, int frames) {
    m_displaySettings = "";
    m_displaySettings += abs(g_stepper.stepsToMm(toPosition - fromPosition));
    m_displaySettings += F("mm,");
    m_displaySettings += abs(int(frames));
    m_displaySettings += F("f");

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
    String msg;
    msg.reserve(16);
    msg += m_displaySettings;
    for (int i = 0; i < m_secsLeft; ++i)
      msg += ".";
    g_display.printValue(msg);
  }
private:
  long m_relmillis;
  int m_secsLeft;
  bool m_overflow;
  String m_displaySettings;
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
  void stopProcess(const String & reason);
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
  String m_message;

  int m_addition;
  long m_minPosition, m_maxPosition;
  long m_fromPosition;
  float m_frameDepth;
  int m_nFrames;
};
