#pragma once

#include <LiquidCrystal_I2C.h>
#include "EncoderDef.h"
#include "Stepper.h"
#include "Camera.h"
#include "Display1602.h"
#include "PhotoSettings.h"

#define ENC_A 3
#define ENC_B 2
#define ENC_BTN 4
#define STEPP_ENA 9
#define STEPP_STEP 11
#define STEPP_DIR 10
#define STEPP_END 12
#define CAM_FOCUS 7
#define CAM_SHOT 13

extern LiquidCrystal_I2C g_lcd;;
extern Display1602 g_display;
extern EncButton g_encoder;
extern CameraT<CAM_FOCUS, CAM_SHOT> g_camera;
extern StepperT<STEPP_ENA, STEPP_STEP, STEPP_DIR, STEPP_END, 16, 200, 3> g_stepper;
extern Settings g_settings;
extern const int g_photoSettingsAddress;