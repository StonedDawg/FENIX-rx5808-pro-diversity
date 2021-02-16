#ifndef STATE_SETTINGS_INTERNAL_H
#define STATE_SETTINGS_INTERNAL_H


#include "state.h"


namespace StateMachine {
    class SettingsInternalStateHandler : public StateMachine::StateHandler {
        private:
                        
            int8_t selectedInternalMenuItem = 0;
            int8_t selectedInternalInternalMenuItem = 0;
            int8_t menuLevel = 0;
            uint8_t menuInternalItems = 9; // Number of items in settingsInternalMenu[]
            char* settingsInternalMenu[]={ 
                "Factory Reset",
                "spectatorFreqScanStep",
                "spectatorFWHM",
                "rssiSeekTreshold",
                "rssiMinTuneTime",
                "rssiHysteresis",
                "rssiHysteresisPeriod",
                "rssiInversion",
                "back",
                };
                void doTapAction();
        public:
            void onEnter();
            void onExit();
            void onUpdate();

            void onInitialDraw();
            void onUpdateDraw();
    };
}


#endif
