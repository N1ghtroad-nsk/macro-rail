#pragma once

#include <Arduino.h>

class Camera {
  public :
  virtual void setup() = 0;
  virtual void focus() = 0;
  virtual void shot() = 0;
  virtual void release() = 0;
};

template <byte FOCUS, byte SHUTTER>
class CameraT : public Camera {
public:
  void setup() {
    pinMode(FOCUS, OUTPUT);
    pinMode(SHUTTER, OUTPUT);
  }

  void focus() {
    digitalWrite(FOCUS, HIGH);
  }

  void shot() {
    digitalWrite(FOCUS, HIGH);
    digitalWrite(SHUTTER, HIGH);
  }

  void release() {
    digitalWrite(FOCUS, LOW);
    digitalWrite(SHUTTER, LOW);
  }

private:

};
