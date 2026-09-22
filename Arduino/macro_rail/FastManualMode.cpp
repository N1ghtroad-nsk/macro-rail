#include "FastManualMode.h"
#include "Globals.h"

FastManualMode::FastManualMode(const __FlashStringHelper * title) :
  ModeEnc(title)
{

}

void FastManualMode::initImpl() {
  m_gear = 0;
  g_stepper.enable();
  display();
}

void FastManualMode::updateImpl() {
  ModeEnc::updateImpl();
  if (g_stepper.endstopHit())
    changeGear(0);
}

void FastManualMode::onTurn(int dir) {
  changeGear(m_gear + dir);
}

void FastManualMode::onClick() {
    changeGear(0);
    apply();
}

void FastManualMode::display() {
  BufPrint b;
  b.print(m_rpms[abs(m_gear)]);
  b.print(F(" rpm"));
  displayValue(b.buf);
}

void FastManualMode::changeGear(int gear) {
  if (gear == m_gear)
    return;
  m_gear = gear;
  if (m_gear == 0) {
      g_stepper.stop();
    } else {
      if (abs(m_gear) > m_maxGear)
        m_gear = m_gear >= 0 ? m_maxGear : -m_maxGear;
      g_stepper.setSpeed(m_rpms[abs(m_gear)]);
      g_stepper.startRotate(m_gear);
    }
    display();
}
