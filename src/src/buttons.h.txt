#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#include "settings.h"
#include <stdint.h>

#define BUTTON_DEBOUNCE_DELAY 300





typedef void (*operation)(void);

//typedef void (*operationz)(void);
/**
typedef struct vrxDock {
    uint8_t mode;
} vrxDock;
*/
typedef struct vrxDockBtn {
    uint32_t lastDebounceTime;
    bool lastReading;
    bool pressed;
    uint32_t changedTime;
    uint8_t pin;
    uint8_t residedAct;
    operation action0;
    operation action1;
    operation action2;
//    int seletOperation(vrxDockBtn*);
    //    operationz action3;
} vrxDockBtn;


//int seletOperation(vrxDockBtn *vrxB);
void enterOTA(void);
void noActionBtn(void);
//int noActionBtnz(vrxDockBtn* vrxB);
void incrementVrxMode(void);
int setResidedAct(vrxDockBtn* vrxB);
void decrementVrxMode(void);
void updateVrxBtn(uint32_t currentTimeUs, vrxDockBtn* vrxB);


#endif

