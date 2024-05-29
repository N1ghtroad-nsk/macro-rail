#pragma once

struct PhotoSettings {
  int exposureMsec = 200;
  int calmMsec = 800;
  int initSec = 5;
  float frameDepth = 0.1f;

  void saveToEEPROM(int addr);

  void loadFromEEPROM(int addr);
};

struct Settings {
  long toPosition = 1;
  int nFrames = 0;

  PhotoSettings photoSettings;

  void finalize();
};