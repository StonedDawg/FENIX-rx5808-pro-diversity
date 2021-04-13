
#ifndef FATSHARK_BUTTON

#include "buttons.h"
#include "settings_eeprom.h"
#include "settings.h"

extern dockBtn fatBtn;
uint8_t readFSBtn(void){
    return 0x7 - ((digitalRead(DOCK_BUTTON2) << 2) | (digitalRead(DOCK_BUTTON1) << 1) | digitalRead(DOCK_BUTTON0));
}



void updateDockBtn(uint32_t currentTimeUs){

     uint8_t reading = 0x7 - ((digitalRead(DOCK_BUTTON2) << 2) | (digitalRead(DOCK_BUTTON1) << 1) | digitalRead(DOCK_BUTTON0));
        /**
         if(reading){
            VRX_LED0_ON;
        } else {
            VRX_LED0_OFF;
        }
        */
     

        if (
            reading != fatBtn.pressed &&
            (currentTimeUs - fatBtn.lastDebounceTime) >= BUTTON_DEBOUNCE_DELAY
        ) {
            
            //Serial.println("pressed long");
            switch(fatBtn.pressed){
                case 0x1:
                
                fatBtn.valueChanged = 1;
                break;
                case 0x2:
                fatBtn.directionChanged = 1;
                
                break;
                case 0x3:
                break;
            }
            fatBtn.pressed = reading;
            
        }  
        
       if (reading != fatBtn.lastReading) {
            fatBtn.lastDebounceTime = currentTimeUs;
        }

        fatBtn.lastReading = reading;     
}

void clearBtnFlags(void){
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
}
uint8_t getBtnFlags(void){
    return ((fatBtn.directionChanged << 2) | (fatBtn.valueChanged << 1));
}
void dockBtnInit(void){
    fatBtn.directionChanged = 0;
    fatBtn.valueChanged = 0;
    fatBtn.pin0 = DOCK_BUTTON0;
    fatBtn.pin1 = DOCK_BUTTON1;
    fatBtn.pin2 = DOCK_BUTTON2;
    pinMode(DOCK_BUTTON0, INPUT);
    pinMode(DOCK_BUTTON1, INPUT);
    pinMode(DOCK_BUTTON2, INPUT);
    
    }

bool isFSBtnErr(void){
    if(getBtnFlags() == 6){
        return 1;
    } else {
        return 0;
    }
}
#endif