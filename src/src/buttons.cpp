#include "buttons.h"
#include "settings_eeprom.h"
#include "settings.h"


extern dockBtn fatBtn;
uint8_t readDockBtn(dockBtn btns){
    return 0x7 - ((digitalRead(DOCK_BUTTON2) << 2) | (digitalRead(DOCK_BUTTON1) << 1) | digitalRead(DOCK_BUTTON0));
}

void updateDockBtn(void){
    int8_t currentValue = readDockBtn();
        if(currentValue - fatBtn.previousValue > 0){
            dockButtonIncrease();
            fatBtn.previousValue = currentValue;
            fatBtn.previousDirection = 1;
        } else if(currentValue - fatBtn.previousValue < 0){
            dockButtonDecrease();
            fatBtn.previousValue = currentValue;
            fatBtn.previousDirection = -1;
        }
        if(isDockBtnErr()){
            clearDockBtnFlags();
        }
    
}

void dockButtonIncrease(void){
    if(fatBtn.previousDirection < 0){
        if(fatBtn.previousValue != 0x0){
            fatBtn.directionChanged = 1;
        }
        
    } else {
        fatBtn.valueChanged = 1;
    }
}


void dockButtonDecrease(void){
    if(fatBtn.previousDirection > 0){
        if(fatBtn.previousValue != 0x7){
            fatBtn.directionChanged = 1;
        }
        
    } else {
        fatBtn.valueChanged = 1;
    }
}

void clearDockBtnFlags(void){
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
}
uint8_t getDockBtnFlags(void){
    return ((fatBtn.directionChanged << 2) | (fatBtn.valueChanged << 1));
}
void dockBtnInit(void){
    fatBtn.previousDirection = 0;
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
    fatBtn.pin0 = DOCK_BUTTON0;
    fatBtn.pin1 = DOCK_BUTTON1;
    fatBtn.pin2 = DOCK_BUTTON2;
    pinMode(DOCK_BUTTON0, INPUT);
    pinMode(DOCK_BUTTON1, INPUT);
    pinMode(DOCK_BUTTON2, INPUT);
    
    fatBtn.previousValue = readDockBtn();
}

bool isDockBtnErr(void){
    if(getDockBtnFlags() == 6){
        return 1;
    } else {
        return 0;
    }
}

void updateVrxBtn(uint32_t currentTimeUs)
{
     uint8_t reading = 0x7 - ((digitalRead(DOCK_BUTTON2) << 2) | (digitalRead(DOCK_BUTTON1) << 1) | digitalRead(DOCK_BUTTON0));
        /**
         if(reading){
            VRX_LED0_ON;
        } else {
            VRX_LED0_OFF;
        }
        */
     
       if (reading != lastReading) {
            lastDebounceTime = currentTimeUs;
        }

        lastReading = reading;

        if (
            reading != pressed &&
            (currentTimeUs - lastDebounceTime) >= BUTTON_DEBOUNCE_DELAY
        ) {
            
            //Serial.println("pressed long");
            if(pressed = 0x1){
                    vrxB->action0();
                    vrxB->residedAct = 1;
            }
            pressed = reading;
            
        }   
}