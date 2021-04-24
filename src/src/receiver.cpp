#include <Arduino.h>

#include "settings.h"
#include "settings_eeprom.h"
#include "receiver.h"
//#include "receiver_spi.h"
#include "channels.h"
#include "state.h"
#include "ui.h"
#include "LowPassFilter.h"

#include "timer.h"

static void updateRssiLimits();
static void writeSerialData();


namespace Receiver {
    ReceiverId activeReceiver = ReceiverId::A;
    uint8_t activeChannel = EepromSettings.startChannel;
    uint8_t dockMode = EepromDefaults.dockMode;
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

    uint16_t rssiBLowThreshold;
    uint16_t rssiALowThreshold;

    uint16_t previousSwitchTime = 0;
    uint16_t antennaAOnTime = 0;
    uint16_t antennaBOnTime = 0;
    uint16_t antennaCOnTime = 0;
    uint16_t antennaDOnTime = 0;

    int16_t rssiDiff = 0;
    uint16_t rssiDiffAbs = 0;
    
    LPF rssiALPF(3);
    LPF rssiBLPF(3);
    
    ReceiverId diversityTargetReceiver = activeReceiver;
    static Timer diversityHysteresisTimer = Timer(5); // default value and is replce by value stored in eeprom during setup

    static Timer rssiStableTimer = Timer(30); // default value and is replce by value stored in eeprom during setup
    static Timer rssiLogTimer = Timer(RECEIVER_LAST_DELAY);

    bool hasRssiUpdated = false;
    bool stopSwitch = false;
    
    uint8_t receiverState = 0;

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
                if(!stopSwitch){
                    if (receiver == ReceiverId::A) {
                        receiverSelect(0);
                    }
                    if (receiver == ReceiverId::B){
                        receiverSelect(1);                  
                    }
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
        rssiARaw = rssiALPF.SmoothDataINT;      
            rssiALPF.update(analogRead(PIN_RSSI_A));
        
        rssiBRaw = rssiBLPF.SmoothDataINT;
            rssiBLPF.update(analogRead(PIN_RSSI_B));
        

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
        }

    }

    void switchDiversity() {
        ReceiverId nextReceiver = activeReceiver;
        
            ReceiverId currentBestReceiver = activeReceiver;
            
            if (rssiDiff > 0) {
                currentBestReceiver = ReceiverId::A;
            } else if (rssiDiff < 0) {
                currentBestReceiver = ReceiverId::B;
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
        pinMode(PIN_VRX_SWITCH0, OUTPUT);
        pinMode(PIN_VRX_SWITCH1, OUTPUT);
        pinMode(OSD_SWITCH, OUTPUT);
        digitalWrite(OSD_SWITCH, LOW);
        digitalWrite(PIN_VRX_SWITCH0, LOW);
        digitalWrite(PIN_VRX_SWITCH1, LOW); 
        pinMode(PIN_RSSI_A, INPUT);
        pinMode(PIN_RSSI_B, INPUT);
        gpio_pulldown_dis(GPIO_NUM_36);
        gpio_pulldown_dis(GPIO_NUM_39);
        gpio_pullup_dis(GPIO_NUM_36);
        gpio_pullup_dis(GPIO_NUM_39);
        gpio_set_pull_mode(GPIO_NUM_36,GPIO_FLOATING);
        gpio_set_pull_mode(GPIO_NUM_39,GPIO_FLOATING);
    //    pinMode(PIN_RSSI_C, INPUT);
    //    pinMode(PIN_RSSI_D, INPUT);
        
        analogSetPinAttenuation(PIN_RSSI_A, ADC_2_5db);
        analogSetPinAttenuation(PIN_RSSI_B, ADC_2_5db);
        analogSetPinAttenuation(PIN_VBAT, ADC_2_5db);
           
    }

    void update() {
        
        updateRssi();
        if (rssiStableTimer.hasTicked()) {

            updateAntenaOnTime();
            
            
            
            switchDiversity();
            

            
        } else {    
        uint16_t rssiALowThresholdValue = (((EepromSettings.rssiAMax - EepromSettings.rssiAMin)*EepromSettings.rssiLowThreshold)/100);
        uint16_t rssiBLowThresholdValue = (((EepromSettings.rssiBMax - EepromSettings.rssiBMin)*EepromSettings.rssiLowThreshold)/100);

        if(EepromSettings.rssiInverted){
            rssiALowThreshold = (((EepromSettings.rssiAMax - rssiALowThresholdValue)));
            rssiBLowThreshold = (((EepromSettings.rssiBMax - rssiBLowThresholdValue)));
        } else {
            rssiALowThreshold = (((EepromSettings.rssiAMin + rssiALowThresholdValue)));
            rssiBLowThreshold = (((EepromSettings.rssiBMin + rssiBLowThresholdValue)));
        
        }
//          if (!EepromSettings.quadversity) {
            
            if(EepromSettings.rssiInverted){
                rssiDiff = rssiB - rssiA;
                if(EepromSettings.noSwitchOnLow){
                if(rssiA > rssiALowThreshold && rssiB > rssiBLowThreshold){
                    //noswitch
                        stopSwitch = 1;
                    } else {
                        stopSwitch = 0;
                    }
                } else {
                    stopSwitch = 0;
                }
            } else {
                rssiDiff = rssiA - rssiB;
                if(EepromSettings.noSwitchOnLow){
                if(rssiA<rssiALowThreshold && rssiB<rssiBLowThreshold){
                    //noswitch   
                        stopSwitch = 1;
                    } else {
                        stopSwitch = 0;
                    }
                } else {
                    stopSwitch = 0;
                }    
            }
            rssiDiffAbs = abs(rssiDiff);
        }
        
    }
}
