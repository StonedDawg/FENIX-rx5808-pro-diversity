#include <stdint.h>
#include "state_menu.h"
#include "ui.h"
//#include "touchpad.h"
#include "CompositeGraphics.h"
#include "settings_eeprom.h"
#include "icons.h"
#include "settings.h"
/**
extern vrxDockBtn vrxBtn0;
extern vrxDockBtn vrxBtn1;
extern vrxDockBtn vrxBtn2;
*/
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

Image<CompositeGraphics> iconHome(home::xres, home::yres, home::pixels);
Image<CompositeGraphics> iconExLRS(exlrs::xres, exlrs::yres, exlrs::pixels);
Image<CompositeGraphics> iconCalibrate(calibrate::xres, calibrate::yres, calibrate::pixels);
Image<CompositeGraphics> iconUpdate(update::xres, update::yres, update::pixels);
Image<CompositeGraphics> iconBookmark(bookmark::xres, bookmark::yres, bookmark::pixels);

void StateMachine::MenuStateHandler::onEnter() {    
   //selectedMenu=0;
   
}

void StateMachine::MenuStateHandler::onUpdate() {
    onUpdateDraw();
    if (getBtnFlags()) {
      //clearBtnFlags();
      this->doTapAction();
      
    }
}

void StateMachine::MenuStateHandler::doTapAction() {
   if(getBtnFlags() == 2){
      clearBtnFlags();
      if(selectedMenu < 7){
      selectedMenu++;
   } else {
      selectedMenu = 0;
   }
   } else if (getBtnFlags() == 4){
      clearBtnFlags();
      if (selectedMenu == 0)
      {
         StateMachine::switchState(StateMachine::State::HOME); 
      }
      else if ( // ExpressLRS Settings
      selectedMenu == 1)
      {
         StateMachine::switchState(StateMachine::State::HOME); 
         //StateMachine::switchState(StateMachine::State::EXPRESSLRS); 
      }
      else if ( // Internal Settings
      selectedMenu == 2
      )
      {
         StateMachine::switchState(StateMachine::State::SETTINGS_INTERNAL); 
         
      }
      else if ( // menu 4
      selectedMenu == 3
      )
      {
         
         
            EepromSettings.noSwitchOnLow = !EepromSettings.noSwitchOnLow;
         
         
      }
      else if ( // RSSI
      selectedMenu == 4
      )
      {
         StateMachine::switchState(StateMachine::State::HOME); 
         
      }
      else if ( // RSSI INVERSION
      selectedMenu == 5
      )
      {
         
         
            EepromSettings.rssiInverted = !EepromSettings.rssiInverted;
         
         
      }
      else if ( // Calibration
      selectedMenu == 6
      )
      {
        StateMachine::switchState(StateMachine::State::SETTINGS_RSSI); 
        
      }
      else if ( // WiFi OTA Update
      selectedMenu == 7
      )
      {
         EepromSettings.otaUpdateRequested = true;
         EepromSettings.save();
         ESP.restart();
      }

   }
     
}

void StateMachine::MenuStateHandler::onInitialDraw() {
    onUpdateDraw();
}

void StateMachine::MenuStateHandler::onUpdateDraw() {    

    iconHome.draw(Ui::display, 47, 57);         // Home
    iconExLRS.draw(Ui::display, 47+60, 57);      // ExpressLRS
    iconBookmark.draw(Ui::display, 47+120, 57);
    iconBookmark.draw(Ui::display, 47+180, 57);
    iconBookmark.draw(Ui::display, 47, 117);
    iconBookmark.draw(Ui::display, 47+60, 117);
    iconCalibrate.draw(Ui::display, 47+120, 117);
    iconUpdate.draw(Ui::display, 47+180, 117);    // Calibration

   Ui::display.setTextColor(100);

   if ( // Home
   selectedMenu == 0
   )
   {
      Ui::display.rect(47-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 120, 200);
      Ui::display.print("Home Screen");
   }
   else if ( // ExpressLRS Settings
   selectedMenu == 1
   )
   {
      Ui::display.rect(47+60-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 90, 200);
      Ui::display.print("ExpressLRS Settings");
   }
   else if ( // Internal Settings
   selectedMenu == 2
   )
   {
      Ui::display.rect(47+120-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 140, 200);
      Ui::display.print("Internal Settings");
   }
   else if ( // item 4
   selectedMenu == 3
   )
   {
      Ui::display.rect(47+180-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 120, 193);
      if(EepromSettings.noSwitchOnLow){
      Ui::display.print("no Diversity on LOW");
      } else{
      Ui::display.print("Normal Diversity");
      } 
      Ui::display.setCursor( 100, 207);
      
        uint16_t rssiALowThresholdValue = (((EepromSettings.rssiAMax - EepromSettings.rssiAMin)*EepromSettings.rssiLowThreshold)/100);
        uint16_t rssiBLowThresholdValue = (((EepromSettings.rssiBMax - EepromSettings.rssiBMin)*EepromSettings.rssiLowThreshold)/100);
      Ui::display.print(rssiALowThresholdValue);
      Ui::display.print("/");
      Ui::display.print(rssiBLowThresholdValue);
   }
   else if ( //RSSI
   selectedMenu == 4
   )
   {
      Ui::display.rect(47-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 100, 193);
      Ui::display.print("RSSIAmin");
      Ui::display.print("/RSSIAmax");
      Ui::display.print(EepromSettings.rssiAMin);
      Ui::display.print("/");          
      Ui::display.print(EepromSettings.rssiAMax);
      Ui::display.setCursor( 100, 207);
      Ui::display.print("RSSIBmin");
      Ui::display.print("/RSSIBmax");
      Ui::display.print(EepromSettings.rssiBMin);
      Ui::display.print("/");          
      Ui::display.print(EepromSettings.rssiBMax);
   }
   else if ( // RSSI INVERSION
   selectedMenu == 5
   )
   {
      Ui::display.rect(47+60-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 120, 200);
      if(EepromSettings.rssiInverted){
      Ui::display.print("RSSI INVERT");
      } else{
      Ui::display.print("RSSI NORMAL");
      }
   }
   else if ( // Receiver Calibration
   selectedMenu == 6
   )
   {
      Ui::display.rect(47+120-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 90, 200);
      Ui::display.print("Enhance Calibration");
   }
   else if ( // WiFi OTA Update
   selectedMenu == 7
   )
   {
      Ui::display.rect(47+180-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 120, 193);
      Ui::display.print("version ");
      Ui::display.print(FIRMWARE_VERSION);
      Ui::display.setCursor( 40, 207);
      Ui::display.print("SSID:Dawgversity  IP:192.168.4.1");
   }

}