#include "Globals.h"

LiquidCrystal_I2C g_lcd(0x27,16,2);
Display1602 g_display(g_lcd);
EncButton g_encoder(ENC_A, ENC_B, ENC_BTN);
CameraT<CAM_FOCUS, CAM_SHOT> g_camera;
StepperT<STEPP_ENA, STEPP_STEP, STEPP_DIR, STEPP_END, 32, 200, 4> g_stepper;
Settings g_settings;
const int g_photoSettingsAddress = 0;