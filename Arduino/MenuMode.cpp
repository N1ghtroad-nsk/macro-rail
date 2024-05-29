#include "MenuMode.h"
#include "Globals.h"
MenuMode::MenuMode(const String & title) :
  ModeEnc(title)
{
}

MenuMode::~MenuMode() {
  for(auto mode : m_modes) {
    delete mode;
  }
}

void MenuMode::finalize() {
  m_backMode = new BackMode();
  insertMode(m_backMode);
}

void MenuMode::insertMode(Mode * newMode) {
  m_modes.push_back(newMode);
}

void MenuMode::updateImpl() {
  if (m_activeMode) { 
    if(m_activeMode->update())
      return;
    else
      returnSub();
  } else
      ModeEnc::updateImpl();
}

void MenuMode::initImpl() {
  m_currentMode = m_modes.begin();
  m_activeMode = nullptr;
  display();
}

void MenuMode::onTurn(int dir) {
  if (dir > 0) {
    m_currentMode++;
    if (m_currentMode == m_modes.end())
      m_currentMode = m_modes.begin();
  } else {
    if (m_currentMode == (m_modes.begin()))
      m_currentMode = m_modes.end();
    m_currentMode--;
  }
  display();
}

void MenuMode::onClick() {
  enterSub();
}

void MenuMode::enterSub() {
  m_activeMode = *m_currentMode;
  if (m_activeMode == m_backMode) {
    Mode::apply();
    return;
  }
  m_activeMode->init();
}

void MenuMode::returnSub() {
  m_activeMode = nullptr;
  displayTitle();
  display();
}

void MenuMode::display() {
  if (*m_currentMode)
    displayValue((*m_currentMode)->title());
}
