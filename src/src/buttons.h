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
uint8_t readDockBtn(void);
void updateDockBtn(void);
void clearDockBtnFlags(void);
uint8_t getDockBtnFlags(void);
void dockButtonIncrease(void);
void dockButtonDecrease(void);
void dockBtnInit(void);
bool dockFSBtnErr(void);

#endif

