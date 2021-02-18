#include "fsbutton.h"
#include "settings_eeprom.h"
#include "settings.h"


extern fsBtn fatBtn;
uint8_t readFSBtn(void){
    return 0x7 - ((digitalRead(FS_BUTTON2) << 2) | (digitalRead(FS_BUTTON1) << 1) | digitalRead(FS_BUTTON0));
}

void updateFSBtn(void){
    int8_t currentValue = readFSBtn();
        if(currentValue - fatBtn.previousValue > 0){
            fsButtonIncrease();
            fatBtn.previousValue = currentValue;
            fatBtn.previousDirection = 1;
        } else if(currentValue - fatBtn.previousValue < 0){
            fsButtonDecrease();
            fatBtn.previousValue = currentValue;
            fatBtn.previousDirection = -1;
        }
        if(isFSBtnErr()){
            clearFSBtnFlags();
        }
    
}

void fsButtonIncrease(void){
    if(fatBtn.previousDirection < 0){
        if(fatBtn.previousValue != 0x0){
            fatBtn.directionChanged = 1;
        }
        
    } else {
        fatBtn.valueChanged = 1;
    }
}


void fsButtonDecrease(void){
    if(fatBtn.previousDirection > 0){
        if(fatBtn.previousValue != 0x7){
            fatBtn.directionChanged = 1;
        }
        
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
    pinMode(FS_BUTTON0, INPUT);
    pinMode(FS_BUTTON1, INPUT);
    pinMode(FS_BUTTON2, INPUT);
    
    fatBtn.previousValue = readFSBtn();
}

bool isFSBtnErr(void){
    if(getFSBtnFlags() == 6){
        return 1;
    } else {
        return 0;
    }
}