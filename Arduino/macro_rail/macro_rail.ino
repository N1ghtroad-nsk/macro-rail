#include <TimerOne.h>
#include <ArduinoSTL.h>

#include "Globals.h"

#include "MenuMode.h"
#include "MainMenu.h"
#include "FastManualMode.h"
#include "FromPositionMode.h"
#include "ToPositionMode.h"
#include "PhotoMode.h"
#include "SaveMode.h"
#include "ValueMode.h"
#include "FreeMode.h"

MainMenu g_mainMenu;

void timerInterrupt() {
  g_stepper.update();
}

void setup() {
  g_lcd.init();
  g_lcd.backlight();
  g_display.init();
  g_stepper.init();

  g_encoder.setEncType(EB_STEP4_LOW);

  g_settings.photoSettings.loadFromEEPROM(g_photoSettingsAddress);

  g_camera.setup();

  MenuMode * settingsMenu = new MenuMode(F("Settings"));

  g_mainMenu.insertMode(new FromPositionMode());
  g_mainMenu.insertMode(new ToPositionMode());
  g_mainMenu.insertMode(new PhotoMode());

  g_mainMenu.insertMode(new FreeMode());

  settingsMenu->insertMode(new ValueMode<float>(F("Frame depth"), F("mm"), g_settings.photoSettings.frameDepth, 0.01));
  settingsMenu->insertMode(new ValueMode<int>(F("Calm time"), F("mSec"), g_settings.photoSettings.calmMsec, 10));
  settingsMenu->insertMode(new ValueMode<int>(F("Exposure time"), F("mSec"), g_settings.photoSettings.exposureMsec, 10));

  settingsMenu->insertMode(new SaveMode());

  settingsMenu->finalize();
  g_mainMenu.insertMode(settingsMenu);
  
  g_mainMenu.init();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerInterrupt);
}

void loop() {
  g_encoder.tick();
  g_mainMenu.update();
}
