#ifndef STATUS_LED_H
#define STATUS_LED_H
#include <stdint.h>

#define VRX_LED0_TOGGLE digitalWrite(VRX_LED0,!digitalRead(VRX_LED0))
#define  VRX_LED0_OFF   digitalWrite(VRX_LED0,LOW);
#define  VRX_LED0_ON    digitalWrite(VRX_LED0,HIGH);
#define VRX_LED1_TOGGLE digitalWrite(VRX_LED1,!digitalRead(VRX_LED1))
#define  VRX_LED1_OFF   digitalWrite(VRX_LED1,LOW);
#define  VRX_LED1_ON    digitalWrite(VRX_LED1,HIGH);
#define VRX_LED2_TOGGLE digitalWrite(VRX_LED2,!digitalRead(VRX_LED2))
#define  VRX_LED2_OFF   digitalWrite(VRX_LED2,LOW);
#define  VRX_LED2_ON    digitalWrite(VRX_LED2,HIGH);

void updateVrxLed(uint32_t currentTimeUs);

#endif