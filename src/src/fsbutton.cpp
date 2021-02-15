#include "fsbutton.h"
#include "settings_eeprom.h"
#include "settings.h"


uint8_t readFSBtn(void){
    return 0x7 - ((digitalRead(FS_BUTTON2) << 2) | (digitalRead(FS_BUTTON1) << 1) | digitalRead(FS_BUTTON0));
}

void updateFSBtn(void){
    int8_t currentValue = readFSBtn();
    if(fatBtn.previousValue < 0x7 && fatBtn.previousValue > 0x0){
        if(currentValue - fatBtn.previousValue > 0){
            fsButtonIncrease();
            fatBtn.previousValue = currentValue;
        } else if(currentValue - fatBtn.previousValue < 0){
            fsButtonDecrease();
            fatBtn.previousValue = currentValue;
        }
    } else {
        if(fatBtn.previousValue == 0x0){
            fatBtn.previousDirection = 1;
        } else if(fatBtn.previousDirection == 0x7){
            fatBtn.previousDirection = -1;
        }
    }
    
}

void fsButtonIncrease(void){
    if(fatBtn.previousDirection < 0){
        fatBtn.directionChanged = 1;
        fatBtn.previousDirection = 1;
    } else {
        fatBtn.valueChanged = 1;
    }
}


void fsButtonDecrease(void){
    if(fatBtn.previousDirection > 0){
        fatBtn.directionChanged = 1;
        fatBtn.previousDirection = -1;
    } else {
        fatBtn.valueChanged = 1;
    }
}

void clearFSBtnFlags(void){
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
}
uint8_t getFSBtnFlags(void){
    return ((fatBtn.directionChanged << 2) | (fatBtn.valueChanged << 1));
}
void fsBtnInit(void){
    fatBtn.previousDirection = 0;
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
    fatBtn.pin0 = FS_BUTTON0;
    fatBtn.pin1 = FS_BUTTON1;
    fatBtn.pin2 = FS_BUTTON2;
    pinMode(fatBtn.pin0, INPUT);
    pinMode(fatBtn.pin1, INPUT);
    pinMode(fatBtn.pin2, INPUT);
    
    fatBtn.previousValue = readFSBtn();
}