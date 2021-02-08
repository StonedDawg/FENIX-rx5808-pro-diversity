
#ifndef SERVO_CONTROL
#define SERVO_CONTROL
#include <Arduino.h>
#include "settings.h"
#include <ESP32Servo.h>
#include <stdint.h>


typedef struct servoControl
{
    uint8_t servoId;
    uint8_t servoCurrentAngle;
    uint8_t servoNextAngle;
    uint8_t servoPreviousAngle;
    uint8_t servoDirection;
    uint8_t servoAvailableTravel;
    uint8_t servoStatus;
}dockTower;
void initDockTowerServo(void);
void updateDockTower(void);
void ActivateTower(dockTower *tower);
void deactivateTower(dockTower *tower);
void runServo(Servo *servo, uint8_t angle);
void updateTower(dockTower *tower);
void deactivateTower(dockTower *tower);
void ActivateTower(dockTower *tower);

#endif