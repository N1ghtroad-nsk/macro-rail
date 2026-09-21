#pragma once

struct PhotoSettings {
  unsigned int exposureMsec = 200;
  unsigned int calmMsec = 800;
  unsigned int initSec = 5;
  unsigned int cameraShotDelay = 200;
  float frameDepth = 0.1f;
  bool useMirrorPreUp = true;

  void saveToEEPROM(int addr);

  void loadFromEEPROM(int addr);
};

struct Settings {
  long toPosition = 1;
  int nFrames = 0;

  PhotoSettings photoSettings;

  void finalize();
};