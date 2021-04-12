#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#include "settings.h"
#include <stdint.h>

#define BUTTON_DEBOUNCE_DELAY 300

typedef struct dockBtn {
    int8_t previousValue;
    int8_t previousDirection;
    uint8_t directionChanged;
    uint8_t valueChanged;
    uint8_t pin0;
    uint8_t pin1;
    uint8_t pin2;
} dockBtn;

//extern fsBtn fatBtn;
uint8_t readFSBtn(void);
void updateFSBtn(void);
void clearFSBtnFlags(void);
uint8_t getFSBtnFlags(void);
void fsButtonIncrease(void);
void fsButtonDecrease(void);
void fsBtnInit(void);
bool isFSBtnErr(void);

#endif

