#pragma once

#define EB_NO_FOR
#define EB_NO_BUFFER
#define EB_NO_CALLBACK
#define EB_NO_COUNTER

#define EB_DEB_TIME 50      // debounce timeout (button)
#define EB_CLICK_TIME 500   // click wait timeout (button)
#define EB_HOLD_TIME 600    // hold timeout (button)
#define EB_STEP_TIME 200    // pulsed hold timeout (button)
#define EB_FAST_TIME 30     // fast rotation timeout (encoder)
#include <EncButton.h>
