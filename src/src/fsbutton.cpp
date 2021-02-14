#include "fsbutton.h"
#include "settings_eeprom.h"



void incrementVrxMode(void){
    if(EepromSettings.diversityMode < 2){
        EepromSettings.diversityMode++;
        
    } else {
        EepromSettings.diversityMode = 0;
    }
}

void noActionBtn(void){
    return;
}
int noActionBtnz(vrxDockBtn* vrxB){
    return 0;
}
void decrementVrxMode(void){
    if(EepromSettings.diversityMode>0){
        EepromSettings.diversityMode--;
    } else {
        EepromSettings.diversityMode = 2;
    }
}

int setResidedAct(vrxDockBtn* vrxB){
    vrxB->residedAct = 1;
}

void enterOTA(void){
    EepromSettings.otaUpdateRequested = true;
}
void updateVrxBtn(uint32_t currentTimeUs, vrxDockBtn* vrxB)
{
     bool reading = !digitalRead(vrxB->pin);
        /**
         if(reading){
            VRX_LED0_ON;
        } else {
            VRX_LED0_OFF;
        }
        */
     
       if (reading != vrxB->lastReading) {
            vrxB->lastDebounceTime = currentTimeUs;
            
            Serial.println("bounce");
        }

        vrxB->lastReading = reading;

        if (
            reading != vrxB->pressed &&
            (currentTimeUs - vrxB->lastDebounceTime) >= BUTTON_DEBOUNCE_DELAY
        ) {
            vrxB->pressed = reading;
            
            Serial.println("pressed");
            uint32_t prevChangeTime = vrxB->changedTime;
            vrxB->changedTime = currentTimeUs;

            if (!vrxB->pressed) {
                uint32_t duration = vrxB->changedTime - prevChangeTime;
                
            Serial.println("released");
                if (duration < 1500){
                    vrxB->action0();
                    vrxB->residedAct = 1;
                }
                else if (duration < 3000){
                    
                    vrxB->action1();
                    //decrementVrxMode();
                    //VRX_LED0_TOGGLE;
                }
            }
        }
        
        if (vrxB->pressed) {
            uint32_t duration = currentTimeUs - vrxB->changedTime;
            
            if (duration >= 3000){
                
                    vrxB->action2();
            VRX_LED1_ON;
            }
        }
        
            
}