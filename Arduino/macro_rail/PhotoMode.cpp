#include "PhotoMode.h"

PhotoMode::PhotoMode() 
  : ModeEnc(F("Start"))
{}

void PhotoMode::initImpl() {
  g_settings.finalize();
  
  m_addition = 0;
  m_minPosition = 0;
  m_maxPosition = 0;
  m_fromPosition = 0; 
  m_frameDepth = g_settings.toPosition > 0 ? g_settings.photoSettings.frameDepth : -g_settings.photoSettings.frameDepth;
  m_nFrames = g_settings.nFrames;

  m_currentStage = Stage::stDone;
  m_framesShot = 0;
  m_message = nullptr;
  
  g_stepper.enable();
  display();
  nextStage();
}

void PhotoMode::updateImpl() {
  ModeEnc::updateImpl();
  if (m_currentStage == stDone)
    return;

  if (m_currentStageWorker->inProgress())
    return;

  const __FlashStringHelper * errorMsg = nullptr;
  if (m_currentStageWorker->isBroken(errorMsg))
    stopProcess(errorMsg);
  else
    nextStage();
}

void PhotoMode::nextStage() {
  m_currentStage = (m_currentStage) + 1;
  switch(m_currentStage) {
    case stInit:
      m_currentStage = stCountMove;
      m_currentStageWorker = &initStage;
      initStage.start(m_fromPosition, m_fromPosition + g_stepper.mmToSteps(m_frameDepth*m_nFrames), g_settings.photoSettings.initSec, m_nFrames);
      break;
    case stLoop:
      m_currentStage = stCalm;
    case stCalm:
      m_currentStageWorker = &timeoutStage;
      // mirror pre-up: shot() raises the lines and returns at once,
      // Camera::update() from loop() releases them; settling window counts from the press
      if (g_settings.photoSettings.useMirrorPreUp)
        g_camera.shot(g_settings.photoSettings.cameraShotDelay);
      timeoutStage.start(g_settings.photoSettings.calmMsec);
      break;
    case stExposure:
      updateMinMax();
      m_currentStageWorker = &exposureStage;
      exposureStage.start(g_settings.photoSettings.exposureMsec);
      break;
    case stCountMove:
      m_framesShot++;
      if (m_framesShot >= m_nFrames) {
        stopProcess(F("Done!"));
      } else {
        m_currentStageWorker = &moveStage;
        moveStage.start(m_frameDepth);
      }
      display();
      break;
    default:
      break;
  }
}

void PhotoMode::stopProcess(const __FlashStringHelper * reason) {
  m_currentStageWorker->cancel();
  m_currentStage = stDone;
  if (reason != nullptr)
    m_message = reason;
  display();
}

void PhotoMode::updateMinMax() {
  if (m_frameDepth > 0)
    m_maxPosition = g_stepper.getPosition();
  else
    m_minPosition = g_stepper.getPosition();
}

void PhotoMode::onClick() {
  if (m_currentStage == stDone) {
    if (m_addition == 0)
      ModeEnc::onClick();
    else {
      if (m_addition > 0) {
        m_frameDepth = abs(m_frameDepth);
        m_nFrames = abs(m_addition); 
        m_fromPosition =  m_maxPosition + g_stepper.mmToSteps(m_frameDepth);
      } else {
        m_frameDepth = -abs(m_frameDepth);
        m_nFrames = abs(m_addition); 
        m_fromPosition =  m_minPosition + g_stepper.mmToSteps(m_frameDepth);
      }

      m_currentStage = Stage::stDone;
      m_framesShot = 0;
      m_message = nullptr;
      m_addition = 0;
      display();
      nextStage();
    }
  } else
    stopProcess(F("Stop"));
}

void PhotoMode::onTurn(int dir) {
  if (m_currentStage == stDone) {
    m_addition -= dir;
    display();
  }
}

void PhotoMode::display() {
  BufPrint b;
  if (m_addition == 0) {
    b.print(m_framesShot);
    b.print(F(" of "));
    b.print(m_nFrames);

    if (m_message != nullptr) {
      b.print(F(": "));
      b.print(m_message);
    }
  } else {
    b.print(F("extra "));
    b.print(m_addition);
  }

  displayValue(b.buf);
}
