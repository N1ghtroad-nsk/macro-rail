#pragma once

#include <Arduino.h>
#include "ModeEnc.h"

class FastManualMode : public ModeEnc {
public:
  FastManualMode(const String & title);
  void initImpl();
  void updateImpl() override;

protected:
  void onTurn(int dir);
  void onClick();

  void display();
  void changeGear(int gear);

private:
  const int m_rpms[6] = {0, 15, 30, 60, 120, 240};
  const int m_maxGear = 5;
  int m_gear = 0;
};
