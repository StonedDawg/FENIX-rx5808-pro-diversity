/*
 * Setings file by Shea Ivey

The MIT License (MIT)

Copyright (c) 2015 Shea Ivey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
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

#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION_NUMBER 0002

#define MSP_VTX_CONFIG      88 //out message         Get vtx settings - betaflight
#define MSP_SET_VTX_CONFIG  89 //in message          Set vtx settings - betaflight

// === Features ===============================================================

// You can use any of the arduino analog pins to measure the voltage of the
// battery. See additional configuration below.

#define USE_VOLTAGE_MONITORING

// Can enable this to powerdown the audio blocks on the RX58xx if you don't
// need it. Save a tiny bit of power, make your videos less noisy.

// WARNING: Makes RSSI act a little weird.
//#define DISABLE_AUDIO

// === Pins ====================================================================


#define PIN_SPI_SLAVE_SELECT_RX_A       32
#define PIN_SPI_SLAVE_SELECT_RX_B       33

#define PIN_RX_SWITCH1                  27
#define PIN_RX_SWITCH2                  28

#define PIN_BUTTON                      29

#define VRX_LED0                        31
#define VRX_LED1                        30
#define VRX_LED2                        32

#define PIN_RSSI_A                      36
#define PIN_RSSI_B                      39
//#define PIN_RSSI_C                      39 // dummy pin
//#define PIN_RSSI_D                      39 // dummy pin

//#define PIN_TOUCHPAD_SLAVE_SELECT       5
//#define PIN_TOUCHPAD_DATA_READY         34

#define PIN_VBAT                        35

// === Voltage Monitoring ======================================================

// Voltage levels
#define WARNING_VOLTAGE 72 // 3.6V per cell for 2S
#define CRITICAL_VOLTAGE 66 // 3.3V per cell for 2S
#define VBAT_SCALE 26 // 100k 10k resistor potential divider
#define VBAT_OFFSET 0

// Alarm sounds
#define ALARM_EVERY_SEC 20000
#define CRITICAL_BEEP_EVERY_MSEC 400
#define CRITICAL_BEEPS 3
#define WARNING_BEEP_EVERY_MSEC 200
#define WARNING_BEEPS 2

// === RSSI ====================================================================

// RSSI default raw range.
#define RSSI_MIN_VAL 500
#define RSSI_MAX_VAL 2000

// 75% threshold, when channel is printed in spectrum.
#define RSSI_SEEK_FOUND 75

// 80% under max value for RSSI.
//#define RSSI_SEEK_TRESHOLD 50

// Scan loops for setup run.
#define RSSI_SETUP_RUN 1

// === Misc ====================================================================

#define EEPROM_SAVE_TIME 1000

#define BEEPER_CHIRP 25

//#define SPEED_TEST

#define DELAY_50_HZ (1000 / 50)
#define DELAY_33_HZ (1000 / 33)
#define DELAY_10_HZ (1000 / 10)
#define DELAY_5_HZ (1000 / 5)

#define BUTTON_DEBOUNCE_DELAY 300


#define VRX_LED0_TOGGLE digitalWrite(VRX_LED0,!digitalRead(VRX_LED0))
#define  VRX_LED0_OFF   digitalWrite(VRX_LED0,LOW);
#define  VRX_LED0_ON    digitalWrite(VRX_LED0,HIGH);
#define VRX_LED1_TOGGLE digitalWrite(VRX_LED1,!digitalRead(VRX_LED1))
#define  VRX_LED1_OFF   digitalWrite(VRX_LED1,LOW);
#define  VRX_LED1_ON    digitalWrite(VRX_LED1,HIGH);
#define VRX_LED2_TOGGLE digitalWrite(VRX_LED2,!digitalRead(VRX_LED2))
#define  VRX_LED2_OFF   digitalWrite(VRX_LED2,LOW);
#define  VRX_LED2_ON    digitalWrite(VRX_LED2,HIGH);
#endif // file_defined
