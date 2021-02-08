# 1 "C:\\Users\\StephenNonMS.INSPIRON5437\\AppData\\Local\\Temp\\tmpoqydylcb"
#include <Arduino.h>
# 1 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
# 35 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
#include <EEPROM.h>
#include "settings.h"
#include "settings_eeprom.h"
#include "state_home.h"
#include "channels.h"
#include "receiver.h"

#include "state.h"
#include "ui.h"
#include "voltage.h"
#include "temperature.h"


#include <esp_now.h>
#include <WiFi.h>
#include "ExpressLRS_Protocol.h"
#include "WebUpdater.h"
#include "fsbutton.h"
#include "servoControl.h"

#ifdef SPEED_TEST
    uint32_t n = 0;
    uint32_t previousTime = millis();
#endif
# 72 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xDA, 0x56, 0xCA,
                              0x50, 0x02, 0x91, 0xDA, 0x37, 0x84};

bool updatingOTA = false;
uint32_t previousLEDTime = 0;

vrxDockBtn vrxBtn0;
vrxDockBtn vrxBtn1;
vrxDockBtn vrxBtn2;

dockTower tracker1;

dockTower tracker2;
void setup();
void setupPins();
void loop();
uint8_t crc8_dvb_s2(uint8_t crc, unsigned char a);
void sendToExLRS(uint16_t function, uint16_t payloadSize, const uint8_t *payload);
void updateVrxLed(uint32_t currentTimeUs);
#line 87 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
void setup()
{

vrxBtn0.residedAct = 0;
vrxBtn0.pressed = 0;
vrxBtn0.pin = PIN_BUTTON0;
vrxBtn0.lastReading = 0;
vrxBtn0.lastDebounceTime = 0;
vrxBtn0.changedTime = 0;
vrxBtn0.action0 = incrementVrxMode;
vrxBtn0.action1 = noActionBtn;
vrxBtn0.action2 = noActionBtn;

vrxBtn1.residedAct = 0;
vrxBtn1.pressed = 0;
vrxBtn1.pin = PIN_BUTTON1;
vrxBtn1.lastReading = 0;
vrxBtn1.lastDebounceTime = 0;
vrxBtn1.changedTime = 0;
vrxBtn1.action0 = noActionBtn;
vrxBtn1.action1 = noActionBtn;
vrxBtn1.action2 = noActionBtn;

vrxBtn2.residedAct = 0;
vrxBtn2.pressed = 0;
vrxBtn2.pin = PIN_BUTTON2;
vrxBtn2.lastReading = 0;
vrxBtn2.lastDebounceTime = 0;
vrxBtn2.changedTime = 0;
vrxBtn2.action0 = incrementVrxMode;
vrxBtn2.action1 = noActionBtn;
vrxBtn2.action2 = noActionBtn;


tracker1.servoId = 1;
tracker1.servoCurrentAngle = 0;
tracker1.servoNextAngle = 0;
tracker1.servoPreviousAngle = 0;
tracker1.servoDirection = 0;
tracker1.servoAvailableTravel = 0;
tracker1.servoStatus = 0;

tracker2.servoId = 2;
tracker2.servoCurrentAngle = 0;
tracker2.servoNextAngle = 0;
tracker2.servoPreviousAngle = 0;
tracker2.servoDirection = 0;
tracker2.servoAvailableTravel = 0;
tracker2.servoStatus = 0;
    #ifdef SPEED_TEST
        Serial.begin(115200);
    #endif



    EEPROM.begin(2048);


    EepromSettings.setup();
    setupPins();
    StateMachine::setup();
    Ui::setup();


    initDockTowerServo();



    Receiver::setup();

    if (!EepromSettings.isCalibrated) {
        StateMachine::switchState(StateMachine::State::SETTINGS_RSSI);
        Ui::tvOn();
    } else {
        StateMachine::switchState(StateMachine::State::HOME);
    }


    if (EepromSettings.otaUpdateRequested)
    {
        BeginWebUpdate();
        EepromSettings.otaUpdateRequested = false;
        EepromSettings.save();
        updatingOTA = true;
    } else



    {
        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {

            return;
        }


        esp_now_peer_info_t injectorInfo;
        for (int i = 0; i < sizeof(broadcastAddress) / 6; i++)
        {
            memcpy(injectorInfo.peer_addr, broadcastAddress + (6 * i), 6);
            injectorInfo.channel = 0;
            injectorInfo.encrypt = false;

            if (esp_now_add_peer(&injectorInfo) != ESP_OK){

                return;
            }
        }
    }
}

void setupPins() {


    pinMode(PIN_BUTTON0, INPUT);
    pinMode(PIN_BUTTON1, INPUT);
    pinMode(PIN_BUTTON2, INPUT);
# 216 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
    pinMode(PIN_VRX_SWITCH1, OUTPUT);
    pinMode(PIN_VRX_SWITCH1, OUTPUT);
    pinMode(VRX_LED0, OUTPUT);
    pinMode(VRX_LED1, OUTPUT);
    pinMode(VRX_LED2, OUTPUT);
    digitalWrite(PIN_VRX_SWITCH1, HIGH);
    digitalWrite(PIN_VRX_SWITCH1, LOW);


    pinMode(PIN_RSSI_A, INPUT);
    pinMode(PIN_RSSI_B, INPUT);



    analogSetPinAttenuation(PIN_VBAT, ADC_2_5db);

}

void loop() {

    if (updatingOTA)
    {
        HandleWebUpdate();
        if (millis() > previousLEDTime+100)
        {

            previousLEDTime = millis();
        }
    } else
    {
        Receiver::update();
        updateVrxLed(millis());
        updateVrxBtn(millis(),&vrxBtn0);
        updateVrxBtn(millis(),&vrxBtn1);
        updateVrxBtn(millis(),&vrxBtn2);



        if (Ui::isTvOn) {

        #ifdef USE_VOLTAGE_MONITORING
            Voltage::update();
        #endif

            Ui::display.begin(0);
            StateMachine::update();
            Ui::update();
            Ui::display.end();

            EepromSettings.update();
        }





        if (Ui::isTvOn &&
            Ui::UiTimeOut.hasTicked() &&
            StateMachine::currentState != StateMachine::State::SETTINGS_RSSI )
        {
            Ui::tvOff();
            EepromSettings.update();
        }
# 290 "D:/git/diversityVRX/FENIX-rx5808-pro-diversity/src/src/rx5808-pro-diversity.ino"
        #ifdef SPEED_TEST
            n++;
            uint32_t nowTime = millis();
            if (nowTime > previousTime + 1000) {
                Serial.print(n);
                Serial.println(" Hz");
                previousTime = nowTime;
                n = 0;
            }
        #endif
    }
}

uint8_t crc8_dvb_s2(uint8_t crc, unsigned char a)
{
    crc ^= a;
    for (int ii = 0; ii < 8; ++ii) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0xD5;
        } else {
            crc = crc << 1;
        }
    }
    return crc;
}

void sendToExLRS(uint16_t function, uint16_t payloadSize, const uint8_t *payload)
{
    uint8_t nowDataOutput[9 + payloadSize] = {0};

    nowDataOutput[0] = '$';
    nowDataOutput[1] = 'X';
    nowDataOutput[2] = '<';
    nowDataOutput[3] = '0';
    nowDataOutput[4] = function & 0xff;
    nowDataOutput[5] = (function >> 8) & 0xff;
    nowDataOutput[6] = payloadSize & 0xff;
    nowDataOutput[7] = (payloadSize >> 8) & 0xff;

    for (int i = 0; i < payloadSize; i++)
    {
        nowDataOutput[8 + i] = payload[i];
    }

    uint8_t ck2 = 0;
    for(int i = 3; i < payloadSize+8; i++)
    {
        ck2=crc8_dvb_s2(ck2, nowDataOutput[i]);
    }
    nowDataOutput[payloadSize+8] = ck2;

    for (int i = 0; i < sizeof(broadcastAddress) / 6; i++)
    {
        uint8_t tempBroadcastAddress[6];
        memcpy(tempBroadcastAddress, broadcastAddress + (6 * i), 6);
        esp_now_send(tempBroadcastAddress, (uint8_t *) &nowDataOutput, sizeof(nowDataOutput));
    }
}



void updateVrxLed(uint32_t currentTimeUs)
{
    static uint32_t vrxLedTime = 0;
    if(EepromSettings.diversityMode == 0){
        if((int32_t)(currentTimeUs - vrxLedTime) < 0){
            return;
        }
        vrxLedTime = currentTimeUs + DELAY_5_HZ;
        VRX_LED0_TOGGLE;
    } else if(EepromSettings.diversityMode == 1){
        VRX_LED0_OFF;
    } else {
        VRX_LED0_ON;
    }
}