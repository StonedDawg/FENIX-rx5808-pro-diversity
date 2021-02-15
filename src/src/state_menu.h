#ifndef STATE_MENU_H
#define STATE_MENU_H


#include "state.h"


namespace StateMachine {
    class MenuStateHandler : public StateMachine::StateHandler {
        private:
            uint16_t selectedMenu = 0;
            void doTapAction();

        public:
            void onEnter();
            void onUpdate();

            void onInitialDraw();
            void onUpdateDraw();

    };
}


#endif
