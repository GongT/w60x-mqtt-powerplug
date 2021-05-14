#pragma once

#define AUTO_GOTO_CONFIG 0
#define DISABLE_REBOOT 1

#define LONG_PRESS_HOLD_MS 1000
#define DBLCLICK_MAX_DELAY_MS 300

#include <rt-thread-w60x/pins.h>

// MY VER2
#define PIN_RED PIN_NUM_B15
#define PIN_RELAY PIN_NUM_B16
#define PIN_GREEN PIN_NUM_B17
#define PIN_BEEP PIN_NUM_B18
#define PIN_KEY PIN_NUM_B08
#define PWM_CH_RED 4
#define PWM_CH_GREEN 2
#define PWM_CH_BEEP 1
#define KEY_DOWN_IS PIN_LOW
#define BEEP_INVERT 0
// END MY VER2

// TB01-TEST
// #define PIN_BEEP PIN_B14
// #define PIN_RED PIN_B15
// #define PIN_GREEN PIN_B16
// #define PIN_RELAY PIN_B17
// #define PIN_KEY PIN_A0
// END TB01-TEST
