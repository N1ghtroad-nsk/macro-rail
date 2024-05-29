#pragma once

#include <Arduino.h>
#include "Globals.h"

class Mode {
public:
  Mode(const String & title) : m_title(title) {};
  void init() {m_active = true; initImpl(); displayTitle();}
  bool update() {updateImpl(); return m_active;}
  void apply() {onApply(); m_active = false;}
  const String & title() {return m_title;}

  virtual void updateImpl() {}
  virtual void initImpl() {}
protected:
  void displayTitle() {g_display.printTitle(m_title);}
  void displayValue(const String & value) {g_display.printValue(value);}

  virtual void onApply() {};

private:
  String m_title;

  bool m_active;
};
