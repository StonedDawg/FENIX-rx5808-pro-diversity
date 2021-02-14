#ifndef FSBUTTON_H
#define FSBUTTON_H

#include <Arduino.h>

#include "settings.h"
#include <stdint.h>

#define BUTTON_DEBOUNCE_DELAY 300


#define VRX_LED0_TOGGLE digitalWrite(VRX_LED0,!digitalRead(VRX_LED0))
#define  VRX_LED0_OFF   digitalWrite(VRX_LED0,LOW);
#define  VRX_LED0_ON    digitalWrite(VRX_LED0,HIGH);
#define VRX_LED1_TOGGLE digitalWrite(VRX_LED1,!digitalRead(VRX_LED1))
#define  VRX_LED1_OFF   digitalWrite(VRX_LED1,LOW);
#define  VRX_LED1_ON    digitalWrite(VRX_LED1,HIGH);
#define VRX_LED2_TOGGLE digitalWrite(VRX_LED2,!digitalRead(VRX_LED2))
#define  VRX_LED2_OFF   digitalWrite(VRX_LED2,LOW);
#define  VRX_LED2_ON    digitalWrite(VRX_LED2,HIGH);



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

