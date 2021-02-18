#include <Arduino.h>

#include "settings.h"
#include "settings_eeprom.h"
#include "receiver.h"
//#include "receiver_spi.h"
#include "channels.h"
#include "state.h"
#include "ui.h"

#include "timer.h"

#include "servoControl.h"

static void updateRssiLimits();
static void writeSerialData();

extern dockTower tracker1;
extern dockTower tracker2;
namespace Receiver {
    ReceiverId activeReceiver = ReceiverId::A;
    uint8_t activeChannel = EepromSettings.startChannel;
    uint8_t dockMode = 0;
    uint16_t  rssiA = 0;
    uint32_t rssiARaw = 0;
    uint16_t  rssiALast[RECEIVER_LAST_DATA_SIZE] = { 0 };

    uint16_t  rssiB = 0;
    uint32_t rssiBRaw = 0;
    uint16_t  rssiBLast[RECEIVER_LAST_DATA_SIZE] = { 0 };
    
    uint16_t  rssiC = 0;
    uint32_t rssiCRaw = 0;
    uint16_t  rssiCLast[RECEIVER_LAST_DATA_SIZE] = { 0 };
    
    uint16_t  rssiD = 0;
    uint32_t rssiDRaw = 0;
    uint16_t  rssiDLast[RECEIVER_LAST_DATA_SIZE] = { 0 };

    uint16_t rssiBandScanData[CHANNELS_SIZE] = { 0 };

    uint16_t previousSwitchTime = 0;
    uint16_t antennaAOnTime = 0;
    uint16_t antennaBOnTime = 0;
    uint16_t antennaCOnTime = 0;
    uint16_t antennaDOnTime = 0;
    
    ReceiverId diversityTargetReceiver = activeReceiver;
    static Timer diversityHysteresisTimer = Timer(5); // default value and is replce by value stored in eeprom during setup

    static Timer rssiStableTimer = Timer(30); // default value and is replce by value stored in eeprom during setup
    static Timer rssiLogTimer = Timer(RECEIVER_LAST_DELAY);

    bool hasRssiUpdated = false;
    
    uint8_t receiverState;
    void setChannel(uint8_t channel)
    {
        ////ReceiverSpi::setSynthRegisterB(Channels::getSynthRegisterB(channel));

        rssiStableTimer.reset();
        activeChannel = channel;

        hasRssiUpdated = false;
    }
    
    void setChannelByFreq(uint16_t freq)
    {      
        ////ReceiverSpi::setSynthRegisterB(Channels::getSynthRegisterBFreq(freq));

        rssiStableTimer.reset();
//        activeChannel = channel;

        hasRssiUpdated = false;      
    }

    void receiverSelect(int8_t sel){
            if(receiverState){
                digitalWrite(PIN_VRX_SWITCH0, !sel);
                digitalWrite(VRX_LED1,!sel);
                digitalWrite(PIN_VRX_SWITCH1, sel);
                digitalWrite(VRX_LED2,sel);
            }
                
    }
    void receiverOff(void){
                receiverState = 0;
                digitalWrite(PIN_VRX_SWITCH0, LOW);
                digitalWrite(VRX_LED1,LOW);
                digitalWrite(PIN_VRX_SWITCH1,LOW);
                digitalWrite(VRX_LED2,LOW);
                
    }
    void receiverOn(void){
                receiverState = 1;
                digitalWrite(PIN_VRX_SWITCH0, HIGH);
                digitalWrite(VRX_LED1,HIGH);
                digitalWrite(PIN_VRX_SWITCH1,LOW);
                digitalWrite(VRX_LED2,LOW);
                activeReceiver = ReceiverId::A;
                
    }

    void setActiveReceiver(ReceiverId receiver) {
        
        switch (EepromSettings.dockMode) {
            case ANTENNA_A:
                receiver = ReceiverId::A;
                receiverSelect(0);
                
                break;

            case ANTENNA_B:
                receiver = ReceiverId::B;
                receiverSelect(1);
                break;
            #ifdef QUADVERSITY
            case ANTENNA_C:
                receiver = ReceiverId::C;
                break;

            case ANTENNA_D:
                receiver = ReceiverId::D;
                break;
            #endif

            default:
                if (receiver == ReceiverId::A) {
                    receiverSelect(0);
                    deactivateTower(&tracker1);
                    ActivateTower(&tracker2);
                    invertTowerDirection(&tracker1,&tracker2);
                }
                if (receiver == ReceiverId::B){
                    receiverSelect(1);
                    deactivateTower(&tracker2);
                    ActivateTower(&tracker1);
                    invertTowerDirection(&tracker2,&tracker1);
                }   
                break;
            #ifdef QUADVERSITY
            case QUADVERSITY:
//                #ifdef DUAL_DIVERSITY
//                    if (receiver == ReceiverId::A || receiver == ReceiverId::B) {
//                    receiverSelect(0);
//                }
//                if (receiver == ReceiverId::C || receiver == ReceiverId::D){
//                    receiverSelect(1);
//                  
//                }
//                #else
//                digitalWrite(PIN_LED_A, receiver == ReceiverId::A);
//                digitalWrite(PIN_LED_B, receiver == ReceiverId::B);
//                digitalWrite(PIN_LED_C, receiver == ReceiverId::C);
//                digitalWrite(PIN_LED_D, receiver == ReceiverId::D);
//                #endif
                break;
                #endif
        }

        activeReceiver = receiver;
    }

    bool isRssiStable() {
        return rssiStableTimer.hasTicked();
    }

    void updateRssi() {
  
        uint8_t RSSI_READS = 3; //15;
        
        rssiARaw = 0;
        for (uint8_t i = 0; i < RSSI_READS; i++) {                       
            rssiARaw += analogRead(PIN_RSSI_A);
        }
        rssiARaw /= RSSI_READS;
        
        rssiBRaw = 0;
        for (uint8_t i = 0; i < RSSI_READS; i++) { 
            rssiBRaw += analogRead(PIN_RSSI_B);
        }
        rssiBRaw /= RSSI_READS;

//        if (EepromSettings.quadversity) {
//            rssiCRaw = 0;
//            for (uint8_t i = 0; i < RSSI_READS; i++) {                       
//                rssiCRaw += analogRead(PIN_RSSI_C);
//            }
//            rssiCRaw /= RSSI_READS;
//            
//            rssiDRaw = 0;
//            for (uint8_t i = 0; i < RSSI_READS; i++) {                       
//                rssiDRaw += analogRead(PIN_RSSI_D);
//            }
//            rssiDRaw /= RSSI_READS;
//        }

        if (StateMachine::currentState != StateMachine::State::SETTINGS_RSSI) {
          
            rssiA = constrain(
                map(
                    rssiARaw,
                    EepromSettings.rssiAMin,
                    EepromSettings.rssiAMax,
                    0,
                    1000
                ),
                0,
                1000
            );
            
            rssiB = constrain(
                map(
                    rssiBRaw,
                    EepromSettings.rssiBMin,
                    EepromSettings.rssiBMax,
                    0,
                    1000
                ),
                0,
                1000
            );
            
//            rssiC = constrain(
//                map(
//                    rssiCRaw,
//                    EepromSettings.rssiCMin,
//                    EepromSettings.rssiCMax,
//                    0,
//                    1000
//                ),
//                0,
//                1000
//            );
//            
//            rssiD = constrain(
//                map(
//                    rssiDRaw,
//                    EepromSettings.rssiDMin,
//                    EepromSettings.rssiDMax,
//                    0,
//                    1000
//                ),
//                0,
//                1000
//            );
          
        }

        if (rssiLogTimer.hasTicked()) {
            for (uint8_t i = 0; i < RECEIVER_LAST_DATA_SIZE - 1; i++) {
                rssiALast[i] = rssiALast[i + 1];
                rssiBLast[i] = rssiBLast[i + 1];                
                #ifdef QUADVERSITY
                if (EepromSettings.quadversity) {
                    rssiCLast[i] = rssiCLast[i + 1];
                    rssiDLast[i] = rssiDLast[i + 1];
                }
                #endif
            }

            rssiALast[RECEIVER_LAST_DATA_SIZE - 1] = rssiA;
            rssiBLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiB;
            #ifdef QUADVERSITY
            if (EepromSettings.quadversity) {
                rssiCLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiC;
                rssiDLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiD;
            }
            #endif

            rssiLogTimer.reset();
        hasRssiUpdated = true;
        updateDockTower();
        }

    }

    void switchDiversity() {
        ReceiverId nextReceiver = activeReceiver;

//          if (!EepromSettings.quadversity) {
            int8_t rssiDiff;
            if(EepromSettings.rssiInverted){
                rssiDiff = (int8_t) rssiB - (int8_t) rssiA;
            } else {
                rssiDiff = (int8_t) rssiA - (int8_t) rssiB;    
            }
            uint8_t rssiDiffAbs = abs(rssiDiff);
            ReceiverId currentBestReceiver = activeReceiver;

            if (rssiDiff > 0) {
                currentBestReceiver = ReceiverId::A;
            } else if (rssiDiff < 0) {
                currentBestReceiver = ReceiverId::B;
            } else {
                currentBestReceiver = activeReceiver;
            }

            if (rssiDiffAbs >= EepromSettings.rssiHysteresis) {
                if (currentBestReceiver == diversityTargetReceiver) {
                    if (diversityHysteresisTimer.hasTicked()) {
                        nextReceiver = diversityTargetReceiver;
                    }
                } else {
                    diversityTargetReceiver = currentBestReceiver;
                    diversityHysteresisTimer.reset();
                }
            } else {
                diversityHysteresisTimer.reset();
            }            
//          }
//          
//          if (EepromSettings.quadversity) {
//            int8_t rssiMax = max(max(rssiA, rssiB), max(rssiC, rssiD));
//            uint8_t rssiDiff = 0;
//            uint8_t rssiDiffAbs = 0;
//            ReceiverId currentBestReceiver = activeReceiver;
//
//
//            // Find which Rx has the highest RSSI.
//            if (rssiA == rssiMax) {
//              currentBestReceiver = ReceiverId::A;
//            } else if (rssiB == rssiMax) {
//              currentBestReceiver = ReceiverId::B;
//            } else if (rssiC == rssiMax) {
//              currentBestReceiver = ReceiverId::C;
//            } else if (rssiD == rssiMax) {
//              currentBestReceiver = ReceiverId::D;
//            }
//
//            // Difference against currently active Rx.
//            if (ReceiverId::A == activeReceiver) {
//              rssiDiff = rssiMax - rssiA;
//            } else if (ReceiverId::B == activeReceiver) {
//              rssiDiff = rssiMax - rssiB;
//            } else if (ReceiverId::C == activeReceiver) {
//              rssiDiff = rssiMax - rssiC;
//            } else if (ReceiverId::D == activeReceiver) {
//              rssiDiff = rssiMax - rssiD;
//            }
//                
//            rssiDiffAbs = abs(rssiDiff);
//
//            if (rssiDiffAbs >= EepromSettings.rssiHysteresis) {
//                if (currentBestReceiver == diversityTargetReceiver) {
//                    if (diversityHysteresisTimer.hasTicked()) {
//                        nextReceiver = diversityTargetReceiver;
//                    }
//                } else {
//                    diversityTargetReceiver = currentBestReceiver;
//                    diversityHysteresisTimer.reset();
//                }
//            } else {
//                diversityHysteresisTimer.reset();
//            }            
//          }
        
          setActiveReceiver(nextReceiver);

    }

    void updateAntenaOnTime() {
        if (ReceiverId::A == activeReceiver) {
        antennaAOnTime += (millis() / 1000) - previousSwitchTime;
        }
        if (ReceiverId::B == activeReceiver) {
        antennaBOnTime += (millis() / 1000) - previousSwitchTime;
        }
        #ifdef QUADVERSITY
        if (EepromSettings.quadversity) {
            if (ReceiverId::C == activeReceiver) {
                antennaCOnTime += (millis() / 1000) - previousSwitchTime;
            }
            if (ReceiverId::D == activeReceiver) {
                antennaDOnTime += (millis() / 1000) - previousSwitchTime;
            }
        }
        #endif  
        previousSwitchTime = millis() / 1000;
    }

    void setup() {
        #ifdef DISABLE_AUDIO
            //ReceiverSpi::setPowerDownRegister(0b00010000110111110011);
        #endif
        setChannel(EepromSettings.startChannel);
        setActiveReceiver(ReceiverId::A);
        diversityHysteresisTimer = Timer(EepromSettings.rssiHysteresisPeriod);    
        rssiStableTimer = Timer(EepromSettings.rssiMinTuneTime);    
    }

    void update() {

        if (rssiStableTimer.hasTicked()) {

            updateAntenaOnTime();
            
            updateRssi();

            switchDiversity();

            
        }
        
    }
}
