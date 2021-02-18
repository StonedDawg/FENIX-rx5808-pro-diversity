

#include "servoControl.h"
#include "timer.h"
#include "settings.h"
#define SERVO_MAX_ANGLE 360/2
extern dockTower tracker1;
extern dockTower tracker2;
ESP32PWM pwm1;
Servo servo1;
Servo servo2;
static Timer angleDelay = Timer(ANGLE_DELAY);

void initDockTowerServo(void){
    ESP32PWM::allocateTimer(0);
    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);
    
	servo1.attach(SERVO1_PIN, SERVO_MIN_US, SERVO_MAX_US);
	servo2.attach(SERVO2_PIN, SERVO_MIN_US, SERVO_MAX_US);
	pwm1.attachPin(PWM_PIN, 10000);//10khz
}
void updateDockTower(void){
    updateTower(&tracker1);
    updateTower(&tracker2);
}

void ActivateTower(dockTower *tower){
    tower->servoStatus = 1;
}

void deactivateTower(dockTower *tower){
    tower->servoStatus = 0;
}
void updateTower(dockTower *tower){
    if(tower->servoDirection == 0){
        tower->servoAvailableTravel = tower->servoCurrentAngle;
    } else if(tower->servoDirection == 1){
        tower->servoAvailableTravel = SERVO_MAX_ANGLE - tower->servoCurrentAngle;
    } else {
        tower->servoAvailableTravel = 0;
    }
    if(tower->servoStatus == 1 && angleDelay.hasTicked()){
        if(tower->servoAvailableTravel){
            if(tower->servoDirection ){
                tower->servoNextAngle = tower->servoCurrentAngle +5 ;
            } else {
                tower->servoNextAngle = tower->servoCurrentAngle-5;
            }
        } else {
            tower->servoDirection = !tower->servoDirection;
        }
    }
    if(tower->servoId == 1 && angleDelay.hasTicked()){
    runServo(&servo1,tower->servoNextAngle);
    tower->servoCurrentAngle = tower->servoNextAngle;
    }else{
    runServo(&servo2,tower->servoNextAngle);
    tower->servoCurrentAngle = tower->servoNextAngle;
    }
}
void runServo(Servo *servo, uint8_t angle){
    servo->write(angle);
}

void invertTowerDirection(dockTower *towera, dockTower *towerb){
towerb->servoDirection = !towera->servoDirection;
}