/*
   SPI driver based on fs_skyrf_58g-main.c Written by Simon Chambers
   TVOUT by Myles Metzel
   Scanner by Johan Hermen
   Inital 2 Button version by Peter (pete1990)
   Refactored and GUI reworked by Marko Hoepken
   Universal version my Marko Hoepken
   Diversity Receiver Mode and GUI improvements by Shea Ivey
   OLED Version by Shea Ivey
   Seperating display concerns by Shea Ivey

  The MIT License (MIT)

  Copyright (c) 2015 Marko Hoepken

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), bto deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <EEPROM.h>
#include "settings.h"
#include "settings_eeprom.h"
#include "state_home.h"
#include "channels.h"
#include "receiver.h"
#include "receiver_spi.h"
#include "state.h"
#include "ui.h"
#include "voltage.h"
#include "temperature.h"
#include "touchpad.h"
#include "receiver_spi.h"
#include <esp_now.h>
#include <WiFi.h>
#include "ExpressLRS_Protocol.h"

#ifdef SPEED_TEST
    uint32_t n = 0; 
    uint32_t previousTime = millis();
#endif

/* 
    esp-now setup for communicating to https://github.com/AlessandroAU/ExpressLRS
    broadcastAddress is the mac of your receiving esp32/esp8266
*/
uint8_t broadcastAddress[] = {0x84, 0x0D, 0x8E, 0x13, 0x3F, 0x38}; // 84:0D:8E:13:3F:38

void setup()
{

    #ifdef SPEED_TEST
        Serial.begin(115200);
    #endif

    EEPROM.begin(2048);
    SPI.begin();
    
    EepromSettings.setup();
    setupPins();
    StateMachine::setup();
    Ui::setup(); 
    TouchPad::setup(); 

    // Has to be last setup() otherwise channel may not be set.
    // RX possibly not booting quick enough if setup() is called earler.
    // delay() may be needed.
    Receiver::setup(); 

    if (!EepromSettings.isCalibrated) {
        StateMachine::switchState(StateMachine::State::SETTINGS_RSSI); 
        Ui::tvOn();
    } else {
        StateMachine::switchState(StateMachine::State::HOME); 
    }

    /* 
        esp-now setup for communicating to https://github.com/AlessandroAU/ExpressLRS
    */

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        // Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_peer_info_t injectorInfo;
    memcpy(injectorInfo.peer_addr, broadcastAddress, 6);
    injectorInfo.channel = 0;  
    injectorInfo.encrypt = false;

    if (esp_now_add_peer(&injectorInfo) != ESP_OK){
        // Serial.println("Failed to add peer");
        return;
    }
}

void setupPins() {

    // Rx and Tx set as input so that they are high impedance when conencted to goggles.
    pinMode(1, INPUT);
    pinMode(3, INPUT);
    
    pinMode(PIN_SPI_SLAVE_SELECT_RX_A, OUTPUT);
    digitalWrite(PIN_SPI_SLAVE_SELECT_RX_A, HIGH);
    
    pinMode(PIN_SPI_SLAVE_SELECT_RX_B, OUTPUT);
    digitalWrite(PIN_SPI_SLAVE_SELECT_RX_B, HIGH);
    
    pinMode(PIN_TOUCHPAD_SLAVE_SELECT, OUTPUT);
    digitalWrite(PIN_TOUCHPAD_SLAVE_SELECT, HIGH);

    pinMode(PIN_RX_SWITCH, OUTPUT);
    digitalWrite(PIN_RX_SWITCH, LOW);

    pinMode(PIN_TOUCHPAD_DATA_READY, INPUT);

    pinMode(PIN_RSSI_A, INPUT);
    pinMode(PIN_RSSI_B, INPUT);
//    pinMode(PIN_RSSI_C, INPUT);
//    pinMode(PIN_RSSI_D, INPUT);

}

void loop() {

    Receiver::update();

    #ifdef USE_VOLTAGE_MONITORING  
        Voltage::update();
    #endif
 
    TouchPad::update();

    if (Ui::isTvOn) {
      
        Ui::display.begin(0);
        StateMachine::update();
        Ui::update();
        Ui::display.end();
  
        EepromSettings.update();
    }
    
    if (TouchPad::touchData.isActive) {
        Ui::UiTimeOut.reset();
    }
    
    if (Ui::isTvOn &&
        Ui::UiTimeOut.hasTicked() &&
        StateMachine::currentState != StateMachine::State::SETTINGS_RSSI ) 
    {
        Ui::tvOff();  
        EepromSettings.update();
    }
    
    if (!Ui::isTvOn &&
        TouchPad::touchData.buttonPrimary)
    {
        Ui::tvOn();
    }
  
    TouchPad::clearTouchData();  

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

void sendToExLRS(group g, task t, uint8_t data)
{
    uint8_t nowDataOutput[3] = {g, t, data};
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &nowDataOutput, sizeof(nowDataOutput));
}