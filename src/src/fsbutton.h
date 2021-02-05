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

typedef void (*selfOperation)(vrxBtn0* vrxB);
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
    operation action0;
    operation action1;
    operation action2;
    selfOperation selfOp;
    uint8_t residedAct;

} vrxDockBtn;

vrxDockBtn vrxBtn0 = {0,0,0,0,PIN_BUTTON0,incrementVrxMode,NULL,NULL,NULL,0};

vrxDockBtn vrxBtn0 = {0,0,0,0,PIN_BUTTON0,NULL,NULL,NULL,setResidedAct,0};
//vrxDockBtn vrxBtn1;
vrxDockBtn vrxBtn2 = {0,0,0,0,PIN_BUTTON2,decrementVrxMode,NULL,NULL,NULL,0};


void incrementVrxMode(void);
void setResidedAct(vrxDockBtn* vrxB);
void decrementVrxMode(void);
void updateVrxBtn(uint32_t currentTimeUs, vrxDockBtn* vrxB);

#endif