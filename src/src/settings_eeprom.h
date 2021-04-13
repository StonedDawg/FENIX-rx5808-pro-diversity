#ifndef SETTINGS_EEPROM_H
#define SETTINGS_EEPROM_H


#include <stdint.h>

#include "settings.h"
#include "state.h"
#include "receiver.h"

enum{
        DIVERSITY = 0,
        ANTENNA_A = 1,
        ANTENNA_B = 2,
        ANTENNA_C = 3,
        ANTENNA_D = 4,
        QUADVERSITY = 5,
        NONE = 6
    };
struct EepromSettings {
    uint8_t versionNumber;
    
    uint8_t isCalibrated;
    uint8_t otaUpdateRequested;

    uint8_t dockMode = 0;
    
    uint8_t startChannel;
    int lastKnownMenuItem;
    StateMachine::State lastKnownState;

    uint8_t beepEnabled;
    
    uint16_t rssiAMin;
    uint16_t rssiAMax;
    uint16_t rssiBMin;
    uint16_t rssiBMax;
    uint16_t rssiCMin;
    uint16_t rssiCMax;
    uint16_t rssiDMin;
    uint16_t rssiDMax;

    uint8_t vbatScale;
    uint8_t vbatWarning;
    uint8_t vbatCritical;
    
    uint8_t favouriteChannels[8];
    int spectatorChannels[8];

    uint8_t quadversity;
    uint8_t buttonBeep;

    // Internal settings
    uint8_t spectatorFreqScanStep;
    uint8_t spectatorFWHM;
    uint8_t rssiSeekTreshold;
    uint16_t rssiMinTuneTime;
    uint8_t rssiHysteresis;
    uint16_t rssiHysteresisPeriod;
    uint8_t rssiInverted;
    uint8_t noSwitchOnLow;
    void setup();
    void update();

    void load();
    void save();
    void markDirty();

    void initDefaults();
};


const struct {
    uint8_t versionNumber = VERSION_NUMBER;
    
    uint8_t isCalibrated = false;
    uint8_t otaUpdateRequested = false;    

    uint8_t dockMode = 0;

    uint8_t startChannel = 27;
    int lastKnownMenuItem = 0;
    StateMachine::State lastKnownState = StateMachine::State::HOME;

    uint8_t beepEnabled = true;
    
    uint16_t rssiAMin = UINT16_MAX;
    uint16_t rssiAMax = 0;
    uint16_t rssiBMin = UINT16_MAX;
    uint16_t rssiBMax = 0;
    uint16_t rssiCMin = UINT16_MAX;
    uint16_t rssiCMax = 0;
    uint16_t rssiDMin = UINT16_MAX;
    uint16_t rssiDMax = 0;

    uint8_t vbatScale = VBAT_SCALE;
    uint8_t vbatWarning = WARNING_VOLTAGE;
    uint8_t vbatCritical = CRITICAL_VOLTAGE;
    
    uint8_t favouriteChannels[8] = {32, 33, 34, 35, 36, 37, 38, 39}; // Race band
    int spectatorChannels[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    
    uint8_t quadversity = false;
    uint8_t buttonBeep = true;

    // Internal settings
    uint8_t spectatorFreqScanStep = 5;
    uint8_t spectatorFWHM = 20;
    uint16_t rssiSeekTreshold = 500;
    uint16_t rssiMinTuneTime = 30;
    uint8_t rssiHysteresis = 100;
    uint16_t rssiHysteresisPeriod = 5;
    uint8_t rssiInverted = 1;
    uint8_t noSwitchOnLow = 1;

} EepromDefaults;


extern EepromSettings EepromSettings;


#endif
