#pragma once

#include <Arduino.h>
#include "Mode.h"

class ModeEnc : public Mode {
public:
  ModeEnc(const String & title);
  virtual void updateImpl() override;

protected:
  virtual void onTurn(int dir) {}
  virtual void onPressTurn(int dir) {onTurn(dir);}
  virtual void onClick() {apply();}

private:
};
