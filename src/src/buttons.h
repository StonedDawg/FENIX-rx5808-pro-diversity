#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#include "settings.h"
#include <stdint.h>

#define BUTTON_DEBOUNCE_DELAY 300

typedef struct dockBtn {
    uint32_t lastDebounceTime;
    bool lastReading;
    bool pressed;
    uint32_t changedTime;
    uint8_t directionChanged;
    uint8_t valueChanged;
    uint8_t pin0;
    uint8_t pin1;
    uint8_t pin2;
} dockBtn;

//extern fsBtn fatBtn;
uint8_t readFSBtn(void);
void updateDockBtn(uint32_t currentTimeUs);
void clearBtnFlags(void);
uint8_t getBtnFlags(void);
void fsButtonIncrease(void);
void fsButtonDecrease(void);
void dockBtnInit(void);
bool isFSBtnErr(void);

#endif

