
#include "state_settings_internal.h"
#include "settings_eeprom.h"
#include "state.h"
#include "ui.h"
#include "voltage.h"
#ifdef FATSHARK_BUTTON
#include "fsbutton.h"
#else
#include "buttons.h"
#endif

#ifdef FATSHARK_BUTTON
extern fsBtn fatBtn;
#else
extern dockBtn fatBtn;
#endif


void StateMachine::SettingsInternalStateHandler::onEnter() {
    selectedInternalMenuItem = 0;
    menuLevel = 0;
//    Ui::clear();
}

void StateMachine::SettingsInternalStateHandler::onExit() {
}

void StateMachine::SettingsInternalStateHandler::onUpdate() {
//    Ui::needUpdate();
    onUpdateDraw();
        if (getBtnFlags()) {
        //clearBtnFlags();
        this->doTapAction();
        
        }
}

void StateMachine::SettingsInternalStateHandler::onInitialDraw() {
    this->onUpdateDraw();
}
void StateMachine::SettingsInternalStateHandler::onUpdateDraw() {


    if (!menuLevel) {
      
        int8_t index;
    
//        Ui::clear();
//        Ui::setTextSize(1);
        
        Ui::display.setCursor(0, 8);
        index = selectedInternalMenuItem-2;
        if (index < 0) {
          index += menuInternalItems;
        }
        Ui::display.print( settingsInternalMenu[index] );
        
            Ui::display.setCursor(6, 18);
            index = selectedInternalMenuItem-1;
            if (index < 0) {
              index += menuInternalItems;
            }
            Ui::display.print( settingsInternalMenu[index] );
            
                Ui::display.setCursor(12, 28);
                Ui::display.setTextColor(0, 100); // 'inverted' text
                Ui::display.print( settingsInternalMenu[selectedInternalMenuItem] );
                Ui::display.setTextColor(100);
            
            Ui::display.setCursor(6, 38);    
            index = selectedInternalMenuItem+1;
            if (index > menuInternalItems-1) {
              index -= menuInternalItems;
            }
            Ui::display.print( settingsInternalMenu[index] );
        
        Ui::display.setCursor(0, 48);
        index = selectedInternalMenuItem+2;
        if (index > menuInternalItems-1) {
          index -= menuInternalItems;
        }
        Ui::display.print( settingsInternalMenu[index] );
    }
    
    if (menuLevel) {
        Ui::display.fillRect(15, 20, 96, 24, 0);
        Ui::display.rect(15, 20, 96, 24, 2);
        Ui::display.setCursor(29, 28);

        switch(selectedInternalMenuItem) {
          
            case 0:    // Factory Reset
              if (selectedInternalInternalMenuItem == 0) {
                  Ui::display.print(("    No     "));
              } else if (selectedInternalInternalMenuItem == 1) {
                  Ui::display.print(("       No  "));
              } else if (selectedInternalInternalMenuItem == 2) {
                  Ui::display.print(("  No       "));
              } else if (selectedInternalInternalMenuItem == 3) {
                  Ui::display.print(("    Yes     "));
              } else if (selectedInternalInternalMenuItem == 4) {
                  Ui::display.print(("  No       "));
              } else if (selectedInternalInternalMenuItem == 5) {
                  Ui::display.print(("       No  "));
              }
            
            break;
       
            case 1:    // spectatorFreqScanStep
                Ui::display.print(EepromSettings.spectatorFreqScanStep);
            break;
            
            case 2:    // spectatorFWHM
                Ui::display.print(EepromSettings.spectatorFWHM);
            break;
            
            case 3:    // rssiSeekTreshold
                Ui::display.print(EepromSettings.rssiSeekTreshold);
            break;
            
            case 4:    // rssiMinTuneTime
                if (selectedInternalInternalMenuItem == 0) {
                  Ui::display.print("cancel");
              } else if (selectedInternalInternalMenuItem == 1) {
                  Ui::display.print("+++");
              } else if (selectedInternalInternalMenuItem == 2) {
                  Ui::display.print("---");
              } else if (selectedInternalInternalMenuItem == 3) {
                  Ui::display.print("OK");
              }Ui::display.print(EepromSettings.rssiMinTuneTime);
            break;
            
            case 5:    // rssiHysteresis
                if (selectedInternalInternalMenuItem == 0) {
                  Ui::display.print("cancel");
              } else if (selectedInternalInternalMenuItem == 1) {
                  Ui::display.print("+++");
              } else if (selectedInternalInternalMenuItem == 2) {
                  Ui::display.print("---");
              } else if (selectedInternalInternalMenuItem == 3) {
                  Ui::display.print("OK");
              }
              Ui::display.print(EepromSettings.rssiHysteresis);
            break;
            
            case 6:    // rssiHysteresisPeriod
                if (selectedInternalInternalMenuItem == 0) {
                  Ui::display.print("cancel");
              } else if (selectedInternalInternalMenuItem == 1) {
                  Ui::display.print("+++");
              } else if (selectedInternalInternalMenuItem == 2) {
                  Ui::display.print("---");
              } else if (selectedInternalInternalMenuItem == 3) {
                  Ui::display.print("OK");
              }
                Ui::display.print(EepromSettings.rssiHysteresisPeriod);
            break;
            
            case 7:    // Rssi inversion 
                Ui::display.print(EepromSettings.rssiInverted);                
                break;
            case 8:    // back 
                Ui::display.print("back");                
                break;
            
        }

    }
    
        
//    Ui::needDisplay();

}

//void StateMachine::SettingsInternalStateHandler::onButtonChange(
//    Button button,
//    Buttons::PressType pressType
//)  {
//  
//  if (
//      pressType == Buttons::PressType::SHORT &&
//      button == Button::MODE_PRESSED
//     ) {
//          if (showChangeInternalMenuOptions) {
//              showChangeInternalMenuOptions = false;
//              if (factoryReset == 3) {
//                EepromSettings.initDefaults(); 
//              }
//              EepromSettings.save();    
//              delay(250); // wait for eeprom to finish writing
//              nvic_sys_reset();
//          } else {
//              showChangeInternalMenuOptions = true;        
//          }
//        }
//  else if (
//      pressType == Buttons::PressType::LONG &&
//      button == Button::MODE_PRESSED
//     ) {
//        }
//  else if (
//      pressType == Buttons::PressType::SHORT &&
//      button == Button::UP_PRESSED
//     ) {
//          if (!showChangeInternalMenuOptions) {
//              selectedInternalMenuItem--;
//              if (selectedInternalMenuItem < 0)
//                selectedInternalMenuItem += menuInternalItems;
//          }    
//          if (showChangeInternalMenuOptions) {   
//    
//            switch(selectedInternalMenuItem) {
//                
//                case (0):    // Factry Reset 
//                    factoryReset--;
//                    if (factoryReset > 5) {
//                      factoryReset = 5;
//                    }                         
//                break;
//           
//                case 1:    // spectatorFreqScanStep
//                    EepromSettings.spectatorFreqScanStep--;
//                break;
//                
//                case 2:    // spectatorFWHM
//                    EepromSettings.spectatorFWHM--;
//                break;
//                
//                case 3:    // rssiSeekTreshold
//                    EepromSettings.rssiSeekTreshold--;
//                break;
//                
//                case 4:    // rssiMinTuneTime
//                    EepromSettings.rssiMinTuneTime--;
//                break;
//                
//                case 5:    // rssiHysteresis
//                    EepromSettings.rssiHysteresis--;
//                break;
//                
//                case 6:    // rssiHysteresisPeriod
//                    EepromSettings.rssiHysteresisPeriod--;
//                break;
//
//                case (7):    // 
//                break;
//                
//            }  
//          }
//        }
//  else if (
//      pressType == Buttons::PressType::SHORT &&
//      button == Button::DOWN_PRESSED
//     ) {
//          if (!showChangeInternalMenuOptions) {
//              selectedInternalMenuItem++;
//              if (selectedInternalMenuItem > menuInternalItems-1)
//                selectedInternalMenuItem -= menuInternalItems;
//          } 
//          if (showChangeInternalMenuOptions) {   
//    
//            switch(selectedInternalMenuItem) {
//                
//                case (0):    // Factry Reset 
//                    factoryReset++;
//                    if (factoryReset > 5) {
//                      factoryReset = 0;
//                    }     
//                break;
//           
//                case 1:    // spectatorFreqScanStep
//                    EepromSettings.spectatorFreqScanStep++;
//                break;
//                
//                case 2:    // spectatorFWHM
//                    EepromSettings.spectatorFWHM++;
//                break;
//                
//                case 3:    // rssiSeekTreshold
//                    EepromSettings.rssiSeekTreshold++;
//                break;
//                
//                case 4:    // rssiMinTuneTime
//                    EepromSettings.rssiMinTuneTime++;
//                break;
//                
//                case 5:    // rssiHysteresis
//                    EepromSettings.rssiHysteresis++;
//                break;
//                
//                case 6:    // rssiHysteresisPeriod
//                    EepromSettings.rssiHysteresisPeriod++;
//                break;
//
//                case (7):    // 
//                break;
//                
//            }
//          }
//        }
//
//}

void StateMachine::SettingsInternalStateHandler::doTapAction() {
   if(getBtnFlags() == 2){
      clearBtnFlags();
      if(menuLevel == 0){
        if(selectedInternalMenuItem < (menuInternalItems-1)){
            selectedInternalMenuItem++;
        } else {
            selectedInternalMenuItem = 0;
        }
      } else if (menuLevel == 1){
          switch(selectedInternalMenuItem){
              case 0:   //factory reset
                if(selectedInternalInternalMenuItem < 5){
                    selectedInternalInternalMenuItem++;
                } else {
                    selectedInternalInternalMenuItem = 0;
                }
              break;
              
              case 4:   //rssi min tune time
                if(selectedInternalInternalMenuItem < 3){
                    selectedInternalInternalMenuItem++;
                } else {
                    selectedInternalInternalMenuItem = 0;
                }
              break;
              case 5:   // rssiHysteresis
                if(selectedInternalInternalMenuItem < 3){
                    selectedInternalInternalMenuItem++;
                } else {
                    selectedInternalInternalMenuItem = 0;
                }
              break;
              case 6:   // rssiHysteresisPeriod
                if(selectedInternalInternalMenuItem < 3){
                    selectedInternalInternalMenuItem++;
                } else {
                    selectedInternalInternalMenuItem = 0;
                }
              break;
          }
      }
   } else if (getBtnFlags() == 4){
      clearBtnFlags();
    switch(selectedInternalMenuItem){
        case 0:
            if(!menuLevel){
                menuLevel++;
            } else{
                switch(selectedInternalInternalMenuItem){
                    case 3:
                                     
                        EepromSettings.initDefaults();
                        ESP.restart();
                    
                    break;
                    default:
                        menuLevel--;
                        selectedInternalInternalMenuItem = 0;
                    break;
                }
            }
        break;
        case 4: //rssimintunetime
            if(!menuLevel){
                menuLevel++;
            } else{
                
                switch(selectedInternalInternalMenuItem){
                    default: //cancel
                        menuLevel--;
                        selectedInternalInternalMenuItem = 0;
                    break;
                    case 1: //add
                        EepromSettings.rssiMinTuneTime++;
                    break;
                    case 2: //sub
                        EepromSettings.rssiMinTuneTime--;
                    break;
                    case 3: //ok
                        EepromSettings.save();
                        EepromSettings.markDirty();
                        selectedInternalInternalMenuItem = 0;
                        menuLevel--;
                    break;
                }
            }
        break;
        case 5: //rssihysteresis
            if(!menuLevel){
                menuLevel++;
            } else{
                
                switch(selectedInternalInternalMenuItem){
                    default:   //cancel
                        menuLevel--;
                        selectedInternalInternalMenuItem = 0;
                    break;
                    case 1: //add
                        EepromSettings.rssiHysteresis++;
                    break;
                    case 2: //sub
                        EepromSettings.rssiHysteresis--;
                    break;
                    case 3: //ok
                        EepromSettings.save();
                        EepromSettings.markDirty();
                        selectedInternalInternalMenuItem = 0;
                        menuLevel--;
                    break;
                }
            }
        break;
        case 6: //rssihysteresisperiod
            if(!menuLevel){
                menuLevel++;
            } else{
                
                switch(selectedInternalInternalMenuItem){
                    default://cancel
                        menuLevel--;
                        selectedInternalInternalMenuItem = 0;
                    break;
                    case 1: //add
                        EepromSettings.rssiHysteresisPeriod++;
                    break;
                    case 2: //sub
                        EepromSettings.rssiHysteresisPeriod--;
                    break;
                    case 3: //ok
                        EepromSettings.save();
                        EepromSettings.markDirty();
                        selectedInternalInternalMenuItem = 0;
                        menuLevel--;
                    break;
                }
            }
        break;

        case 7:
           if(EepromSettings.rssiInverted){
               EepromSettings.rssiInverted = !EepromSettings.rssiInverted;
            }
        break;
        case 8:
            menuLevel = 0;
            selectedInternalMenuItem = 0;
            StateMachine::switchState(StateMachine::State::MENU);
        break;        

    }
   }



}
