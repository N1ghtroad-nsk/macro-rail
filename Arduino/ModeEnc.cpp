#include "ModeEnc.h"
#include "Globals.h"

ModeEnc::ModeEnc(const String & title) :
  Mode(title)
{
}

void ModeEnc::updateImpl() {
  if (g_encoder.turn())
  {
    if (g_encoder.pressing())
      onPressTurn(g_encoder.dir());
    else
      onTurn(g_encoder.dir());
  }
  if (g_encoder.click())
    onClick();
}
