#pragma once

#include <ArduinoStl.h>
#include <Arduino.h>
#include "ModeEnc.h"
#include "BackMode.h"

class MenuMode : public ModeEnc {
public:
  MenuMode(const String & title);
  virtual ~MenuMode();
  virtual void finalize();
  void insertMode(Mode * newMode);
  void initImpl();
  void updateImpl() override;
  
protected:
  virtual void onTurn(int dir);
  virtual void onClick();

  void enterSub();
  void returnSub();
  void display();

private:
  std::vector<Mode*> m_modes;
  std::vector<Mode*>::iterator m_currentMode;
  Mode* m_activeMode = nullptr;

  BackMode * m_backMode = nullptr;
};
