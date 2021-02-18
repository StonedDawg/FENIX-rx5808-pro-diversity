#include "statusled.h"
#include "Arduino.h"
#include "settings_eeprom.h"
#include "settings.h"

void updateVrxLed(uint32_t currentTimeUs)
{
    static uint32_t vrxLedTime = 0;
    if(EepromSettings.dockMode == 0){
        if((int32_t)(currentTimeUs - vrxLedTime) < 0){
            return;
        }
        vrxLedTime = currentTimeUs + DELAY_5_HZ;
        VRX_LED0_TOGGLE;
    } else if(EepromSettings.dockMode == 1){
        VRX_LED0_OFF;
    } else {
        VRX_LED0_ON;
    }
}