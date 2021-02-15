#include <stdint.h>
#include "state_menu.h"
#include "ui.h"
//#include "touchpad.h"
#include "CompositeGraphics.h"
#include "settings_eeprom.h"
#include "fsbutton.h"
#include "icons.h"
/**
extern vrxDockBtn vrxBtn0;
extern vrxDockBtn vrxBtn1;
extern vrxDockBtn vrxBtn2;
*/
Image<CompositeGraphics> iconHome(home::xres, home::yres, home::pixels);
Image<CompositeGraphics> iconExLRS(exlrs::xres, exlrs::yres, exlrs::pixels);
Image<CompositeGraphics> iconCalibrate(calibrate::xres, calibrate::yres, calibrate::pixels);
Image<CompositeGraphics> iconUpdate(update::xres, update::yres, update::pixels);
Image<CompositeGraphics> iconBookmark(bookmark::xres, bookmark::yres, bookmark::pixels);

void StateMachine::MenuStateHandler::onEnter() {    
   selectedMenu=0;
}

void StateMachine::MenuStateHandler::onUpdate() {
    onUpdateDraw();
    if (getFSBtnFlags()) {
      //clearFSBtnFlags();
      this->doTapAction();
    }
}

void StateMachine::MenuStateHandler::doTapAction() {
   if(getFSBtnFlags() == 2){
      clearFSBtnFlags();
      selectedMenu++;
   } else if (getFSBtnFlags() == 4){
      clearFSBtnFlags();
      if (selectedMenu == 0)
      {
         StateMachine::switchState(StateMachine::State::HOME); 
      }
      else if ( // ExpressLRS Settings
      selectedMenu == 1)
      {
         //StateMachine::switchState(StateMachine::State::EXPRESSLRS); 
      }
      else if ( // item 3
      selectedMenu == 2
      )
      {
         
      }
      else if ( // item 4
      selectedMenu == 3
      )
      {
         
      }
      else if ( // item 5
      selectedMenu == 4
      )
      {
         
      }
      else if ( // item 6
      selectedMenu == 5
      )
      {
         
      }
      else if ( // Calibration
      selectedMenu == 6
      )
      {
         EepromSettings.initDefaults();
         ESP.restart();
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
   else if ( // item 3
   selectedMenu == 2
   )
   {
      Ui::display.rect(47+120-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 140, 200);
      Ui::display.print("Menu 3");
   }
   else if ( // item 4
   selectedMenu == 3
   )
   {
      Ui::display.rect(47+180-5, 57-6, 60, 60, 100);
      Ui::display.setCursor( 140, 200);
      Ui::display.print("Menu 4");
   }
   else if ( // item 5
   selectedMenu == 4
   )
   {
      Ui::display.rect(47-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 140, 200);
      Ui::display.print("Menu 5");
   }
   else if ( // item 6
   selectedMenu == 5
   )
   {
      Ui::display.rect(47+60-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 140, 200);
      Ui::display.print("Menu 6");
   }
   else if ( // Receiver Calibration
   selectedMenu == 6
   )
   {
      Ui::display.rect(47+120-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 90, 200);
      Ui::display.print("Receiver Calibration");
   }
   else if ( // WiFi OTA Update
   selectedMenu == 7
   )
   {
      Ui::display.rect(47+180-5, 117-5, 60, 60, 100);
      Ui::display.setCursor( 120, 193);
      Ui::display.print("WiFi Update");
      Ui::display.setCursor( 70, 207);
      Ui::display.print("SSID:Dawgversity  IP:192.168.4.1");
   }

}