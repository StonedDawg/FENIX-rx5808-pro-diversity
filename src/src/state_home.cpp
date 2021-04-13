#include <Arduino.h>
#include <WString.h>

#include "settings_eeprom.h"

#include "state_home.h"

#include "receiver.h"
//#include "receiver_spi.h"
#include "channels.h"
#include "state.h"
#include "ui.h"
#include "temperature.h"
#include "voltage.h"
//#include "touchpad.h"
#include "ExpressLRS_Protocol.h"
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

extern void sendToExLRS(uint16_t function, uint16_t payloadSize, const uint8_t *payload);

// For scalling graphics accross screen
#ifdef CHANNELS_72
    uint8_t CHANNELS_SIZE_DIVIDER = 4;
#endif
#ifdef CHANNELS_48
    uint8_t CHANNELS_SIZE_DIVIDER = 6;
#endif

using StateMachine::HomeStateHandler;

void HomeStateHandler::onEnter() {
  
    displayActiveChannel = Receiver::activeChannel;
    
}

void HomeStateHandler::onUpdate() {

}

void HomeStateHandler::onInitialDraw() {
    this->onUpdateDraw();
}

void HomeStateHandler::onUpdateDraw() {
    
    if (getBtnFlags()) {
        //clearBtnFlags();
      this->doTapAction();
      Ui::UiTimeOut.reset();
      
    }
    if(!(menuLevel && !internalSelectedMenu)){
      Ui::UiTimeOut.reset();
    }
    
    if (isInBandScanRegion()) {
        bandScanUpdate();
        wasInBandScanRegion = true;
    } else {
        if (wasInBandScanRegion) {
            Receiver::setChannel(displayActiveChannel);
        }
        displayActiveChannel = Receiver::activeChannel;
        wasInBandScanRegion = false;
    }
    
    // Mode
    Ui::display.setTextColor(100);
    Ui::display.setCursor( 8, 0);  
    if (EepromSettings.dockMode == ANTENNA_A) {
        Ui::display.print("Antenna A");
    }   
    if (EepromSettings.dockMode == ANTENNA_B) {
        Ui::display.print("Antenna B");
    }   
    if (EepromSettings.dockMode == ANTENNA_C) {
        Ui::display.print("Antenna C");
    }   
    if (EepromSettings.dockMode == ANTENNA_D) {
        Ui::display.print("Antenna D");
    }   
    if (EepromSettings.dockMode == DIVERSITY) {
        Ui::display.print("Diversity");
    }
    if (EepromSettings.dockMode == QUADVERSITY) {
        Ui::display.print("Quadversity");
    }
    
        Ui::display.print("      ");
        if(menuLevel == 0){    
        Ui::display.setTextColor(0, 100);
        }
        if(internalSelectedMenu == 0){
           Ui::display.print("Mode");
        }else if(internalSelectedMenu == 1){
           Ui::display.print("Analyze");
           Ui::display.print(bandScanSelectedChannel);
           
        } else if(internalSelectedMenu == 2){
           Ui::display.print("scanning");
        } else if(internalSelectedMenu == 3){
           Ui::display.print("menu");
        }
        Ui::display.setTextColor(100);
    #ifdef USE_VOLTAGE_MONITORING
        // Voltage
        if (Voltage::voltage > 9) {
            Ui::display.setCursor( 173, 0);
        } else {
            Ui::display.setCursor( 181, 0);
        }
        Ui::display.print(Voltage::voltage);
        Ui::display.print(".");
        Ui::display.print(Voltage::voltageDec);
        Ui::display.print("V ");
    #else
        Ui::display.setCursor( 221, 0);
    #endif

    // Temperature // Doesnt currently work within ESP32 Arduino.
    Ui::display.print(Temperature::getTemperature());
    Ui::display.print("C ");

    // On Time
    uint8_t hours = millis() / 1000 / 60 / 60;
    uint8_t mins  = millis() / 1000 / 60 - hours * 60 * 60;
    uint8_t secs  = millis() / 1000 - hours * 60 * 60 - mins * 60;
    Ui::display.print(hours);
    Ui::display.print(":"); 
    if(mins < 10) {
        Ui::display.print("0");       
    }
    Ui::display.print(mins);
    Ui::display.print(":"); 
    if(secs < 10) {
        Ui::display.print("0");       
    }
    Ui::display.print(secs);

    // Menu Icon
    Ui::display.line( 315, 1, 322, 1, 100);
    Ui::display.line( 315, 4, 322, 4, 100);
    Ui::display.line( 315, 7, 322, 7, 100);

    // Horixontal line
    Ui::display.line( 0, 9, SCREEN_WIDTH, 9, 100);
    
    // Display Band and Channel
    Ui::display.setCursor( 2, 15);
    Ui::display.printLarge(Channels::getName(displayActiveChannel), 8, 12);
           
    // Display Frequency
    Ui::display.setCursor( 0, 105);
    Ui::display.printLarge(Channels::getFrequency(displayActiveChannel), 4, 3);
    
//    // Channel labels
//    if (!EepromSettings.quadversity) {
    Ui::display.setCursor( 130, 12 + 28*0 + 3);
    Ui::display.printLarge("A", 2, 2);
    Ui::display.setCursor( 130, 12 + 28*2 + 3);
    Ui::display.printLarge("B", 2, 2);//      
//    }
//    if (EepromSettings.quadversity) {
//        Ui::drawBigCharacter( 190, 23, 
//                              'A', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28, 
//                              'B', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28*2, 
//                              'C', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28*3, 
//                              'D', 
//                              3, 2);      
//    }
    
//    // Channel selected square
//    if (!EepromSettings.quadversity) {
      if (Receiver::activeReceiver == Receiver::ReceiverId::A) {
          Ui::display.rect(128, 10 + 28*0 + 3, 19, 19, 100);
      }
      if (Receiver::activeReceiver == Receiver::ReceiverId::B) {
          Ui::display.rect(128, 10 + 28*2 + 3, 18, 18, 100);
      }
//    }
//    if (EepromSettings.quadversity) {
//      if (Receiver::activeReceiver == Receiver::ReceiverId::A) {
//          Ui::drawRoundRect(185, 20, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::B) {
//          Ui::drawRoundRect(185, 20 + 28, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::C) {
//          Ui::drawRoundRect(185, 20 + 28*2, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::D) {
//          Ui::drawRoundRect(185, 20 + 28*3, 34, 24, 2, WHITE);
//      }
//    }
//
//    // On percentage
//    if (!EepromSettings.quadversity) {
        Ui::display.setCursor(128, 30 + 28*0 + 3);
        Ui::display.print( (100.0 * Receiver::antennaAOnTime) / (millis() / 1000) );
        Ui::display.print("%, ");
        Ui::display.print( (Receiver::rssiA));
        Ui::display.setCursor(128, 32 + 28*2 + 3);
        Ui::display.print( (100.0 * Receiver::antennaBOnTime) / (millis() / 1000) );
        Ui::display.print("%, ");
        Ui::display.print( (Receiver::rssiB));
//    }
//    if (EepromSettings.quadversity) {
//        Ui::setCursor(225, 25);
//        Ui::display.print( (100 * Receiver::antennaAOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28);
//        Ui::display.print( (100 * Receiver::antennaBOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28*2);
//        Ui::display.print( (100 * Receiver::antennaCOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28*3);
//        Ui::display.print( (100 * Receiver::antennaDOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//    }
//
//    // Draw RSSI Plots
//    if (!EepromSettings.quadversity) {
          Ui::display.rect(195, 12 + 28*0 + 3, 64*2, 28*2-1, 100);
          Ui::display.rect(195, 12 + 28*2 + 3, 64*2, 28*2-1, 100);
        for (uint8_t i=0; i < RECEIVER_LAST_DATA_SIZE-1; i++) {
          Ui::display.line(195+1*i, (12 + 28*2)-Receiver::rssiALast[i]/20, 195+1*(i+1), (12 + 28*2)-Receiver::rssiALast[i+1]/20, 100);
          Ui::display.line(195+1*i, (12 + 28*4)-Receiver::rssiBLast[i]/20, 195+1*(i+1), (12 + 28*4)-Receiver::rssiBLast[i+1]/20, 100);
        }
//    }
//    if (EepromSettings.quadversity) {
//          Ui::drawRect(250, 20 + 28*0 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*1 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*2 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*3 + 3, 64*3-1, 28*1-1, WHITE);
//        for (uint8_t i=0; i < RECEIVER_LAST_DATA_SIZE; i++) {
//          Ui::drawLine(250+3*i, (20 + 28*1)-Receiver::rssiALast[i]/4, 250+3*(i+1), (20 + 28*1)-Receiver::rssiALast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*2)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*2)-Receiver::rssiBLast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*3)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*3)-Receiver::rssiBLast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*4)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*4)-Receiver::rssiBLast[i+1]/4, WHITE);
//        }
//    }
//

    // Plot Spectrum 324 x 224
    for (uint8_t i=0; i<CHANNELS_SIZE; i++) {
        Ui::display.fillRect(18+CHANNELS_SIZE_DIVIDER*i, 214 - Receiver::rssiBandScanData[i]*8/100, CHANNELS_SIZE_DIVIDER, Receiver::rssiBandScanData[i]*8/100, Receiver::rssiBandScanData[i]/10);
    }
    Ui::display.line(0, 213, 323, 213, 100);
    Ui::display.setCursor( 1, 215);
    Ui::display.print(Channels::getFrequency(Channels::getOrderedIndex(0)));
    Ui::display.setCursor( 290, 215);
    Ui::display.print(Channels::getFrequency(Channels::getOrderedIndex(CHANNELS_SIZE-1)));

    // Marker triangle
    uint8_t markerX = Channels::getOrderedIndexFromIndex(Receiver::activeChannel);
    for (int i = 0; i < 7; i++) {
        Ui::display.line(18+CHANNELS_SIZE_DIVIDER*markerX, 214, 18+CHANNELS_SIZE_DIVIDER*markerX+(-3+i), 219, 100);
    }
    
    if (HomeStateHandler::isInAnalyzeRegion() && bandScanSelectedChannel> 18 && bandScanSelectedChannel < (324-13)) {
        //Ui::UiTimeOut.reset();
        Ui::display.fillRect( bandScanSelectedChannel - 33, 183, 33, 17, 10);
        Ui::display.setCursor( bandScanSelectedChannel - 32,184 );
        Ui::display.print(Channels::getName( 
                                            Channels::getOrderedIndex( 
                                                                     (bandScanSelectedChannel-18) / CHANNELS_SIZE_DIVIDER
                                                                     )
                                            )
                          );
        Ui::display.setCursor( bandScanSelectedChannel - 32, 192 );
        Ui::display.print(Channels::getFrequency( 
                                            Channels::getOrderedIndex( 
                                                                     (bandScanSelectedChannel-18) / CHANNELS_SIZE_DIVIDER
                                                                     )
                                            )
                          );
    }
    
}

void HomeStateHandler::doTapAction() {
    if(getBtnFlags() == 4){
        
        clearBtnFlags();
        if(menuLevel == 0){
            switch (internalSelectedMenu)
            {
                case 2: //scanning
                    menuLevel++;
                break;
                case 1: //analyze
                    menuLevel++;
                break;
                case 0: //Mode selection
                    menuLevel++;
                break;
            
            default: //menu
                
                menuLevel = 1;
                internalSelectedMenu = 0;
                EepromSettings.save();
                StateMachine::switchState(StateMachine::State::MENU);
            break;
            }
        } else {
            switch (internalSelectedMenu)
            {
                case 2: //scanning
                    menuLevel = 0;
                break;
                case 1: //analyze
                    menuLevel = 0;
                break;
                case 0: //Mode selection
                    menuLevel = 0;
                break;
            
            default: //menu
                menuLevel = 1;
                internalSelectedMenu = 0;
                EepromSettings.save();
                StateMachine::switchState(StateMachine::State::MENU);
            break;
            }
        }
    } else if(getBtnFlags() == 2){
        clearBtnFlags();
        if (menuLevel == 0){
                if(internalSelectedMenu < 3){
                internalSelectedMenu++;
                } else {
                    internalSelectedMenu = 0;
                }
        }else if(menuLevel == 1){
                    switch (internalSelectedMenu){
                    case 2: //scanning
                        scanNext = 1;
                    break;
                    case 1: //analyze
                        if(bandScanSelectedChannel < 306 && bandScanSelectedChannel >18){
                        bandScanSelectedChannel+=5;
                        } else {
                            bandScanSelectedChannel = 19;
                        }
                    break;
                    case 0: //Mode selection
                            if (EepromSettings.quadversity) {
                        switch ( EepromSettings.dockMode )
                        {
                            case DIVERSITY:
                                EepromSettings.dockMode = ANTENNA_A;
                                break;
                            case ANTENNA_A:
                                EepromSettings.dockMode = ANTENNA_B;
                                break;
                            case ANTENNA_B:
                                EepromSettings.dockMode = ANTENNA_C;
                                break;
                            case ANTENNA_C:
                                EepromSettings.dockMode = ANTENNA_D;
                                break;
                            case ANTENNA_D:
                                EepromSettings.dockMode = QUADVERSITY;
                                break;
                            case QUADVERSITY:
                                EepromSettings.dockMode = DIVERSITY;
                                break;
                        }
                    } else {        
                        switch ( EepromSettings.dockMode )
                        {
                            case ANTENNA_A:
                                EepromSettings.dockMode = ANTENNA_B;
            //                      //ReceiverSpi::rxStandby(Receiver::ReceiverId::A);
                                break;
                            case ANTENNA_B:
                                EepromSettings.dockMode = DIVERSITY;
            //                      //ReceiverSpi::rxPowerOn(Receiver::ReceiverId::A);
                                break;
                            case DIVERSITY:
                                EepromSettings.dockMode = ANTENNA_A;
                                break;
                        }

                    }
                
            
                EepromSettings.markDirty();
            

                break;
                
                default: //menu
                
                    EepromSettings.save();
                    StateMachine::switchState(StateMachine::State::MENU);
                break;
                
            }
        }
    }

}    
    
    /**
  if ( // Up band
      TouchPad::touchData.cursorX >= 0  && TouchPad::touchData.cursorX < 61 &&
      TouchPad::touchData.cursorY > 8 && TouchPad::touchData.cursorY < 54
     ) {
          this->setChannel(8);
          uint8_t payload[4] = {displayActiveChannel, 0, 1, 0};
          sendToExLRS(MSP_SET_VTX_CONFIG, sizeof(payload), (uint8_t *) &payload);
        }
  else if ( // Down band
      TouchPad::touchData.cursorX >= 0  && TouchPad::touchData.cursorX < 61 &&
      TouchPad::touchData.cursorY > 54 && TouchPad::touchData.cursorY < 99
     ) {
          this->setChannel(-8);
          uint8_t payload[4] = {displayActiveChannel, 0, 1, 0};
          sendToExLRS(MSP_SET_VTX_CONFIG, sizeof(payload), (uint8_t *) &payload);
        }
  else if ( // Up channel
      TouchPad::touchData.cursorX > 61  && TouchPad::touchData.cursorX < 122 &&
      TouchPad::touchData.cursorY > 8 && TouchPad::touchData.cursorY < 54
     ) {
          this->setChannel(1);
          uint8_t payload[4] = {displayActiveChannel, 0, 1, 0};
          sendToExLRS(MSP_SET_VTX_CONFIG, sizeof(payload), (uint8_t *) &payload);
        }
  else if ( // Down channel
      TouchPad::touchData.cursorX > 61  && TouchPad::touchData.cursorX < 122 &&
      TouchPad::touchData.cursorY > 54 && TouchPad::touchData.cursorY < 99
     ) {
          this->setChannel(-1);
          uint8_t payload[4] = {displayActiveChannel, 0, 1, 0};
          sendToExLRS(MSP_SET_VTX_CONFIG, sizeof(payload), (uint8_t *) &payload);
        }
  else if ( // Menu
      TouchPad::touchData.cursorX > 314  && TouchPad::touchData.cursorY < 8
     ) {
          EepromSettings.save();
          StateMachine::switchState(StateMachine::State::MENU);
        }
  else if ( // Change mode
      TouchPad::touchData.cursorX < 130 &&
      TouchPad::touchData.cursorY < 8
     ) {
          if (EepromSettings.quadversity) {
              switch ( EepromSettings.dockMode )
              {
                  case Receiver::dockMode::ANTENNA_A:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_B;
                      break;
                  case Receiver::dockMode::ANTENNA_B:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_C;
                      break;
                  case Receiver::dockMode::ANTENNA_C:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_D;
                      break;
                  case Receiver::dockMode::ANTENNA_D:
                      EepromSettings.dockMode = Receiver::dockMode::DIVERSITY;
                      break;
                  case Receiver::dockMode::DIVERSITY:
                      EepromSettings.dockMode = Receiver::dockMode::QUADVERSITY;
                      break;
                  case Receiver::dockMode::QUADVERSITY:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_A;
                      break;
              }
          } else {        
              switch ( EepromSettings.dockMode )
              {
                  case Receiver::dockMode::ANTENNA_A:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_B;
//                      //ReceiverSpi::rxStandby(Receiver::ReceiverId::A);
                      break;
                  case Receiver::dockMode::ANTENNA_B:
                      EepromSettings.dockMode = Receiver::dockMode::DIVERSITY;
//                      //ReceiverSpi::rxPowerOn(Receiver::ReceiverId::A);
                      break;
                  case Receiver::dockMode::DIVERSITY:
                      EepromSettings.dockMode = Receiver::dockMode::ANTENNA_A;
                      break;
              }

          }
          
          EepromSettings.markDirty();
          
        }
  else if ( // Select channel from spectrum
      HomeStateHandler::isInBandScanRegion()
     ) {
          Receiver::setChannel(
                              Channels::getOrderedIndex( (TouchPad::touchData.cursorX-18) / CHANNELS_SIZE_DIVIDER )
                              );
          HomeStateHandler::centreFrequency();
          displayActiveChannel = Receiver::activeChannel;
          
          EepromSettings.startChannel = displayActiveChannel;
          EepromSettings.markDirty();
        }
        
}
*/
void HomeStateHandler::setChannel(int channelIncrement) {

    int band = Receiver::activeChannel / 8;
    int channel = Receiver::activeChannel % 8;
    
    if (channelIncrement == 8) {
      band = band + 1;
    }
    
    if (channelIncrement == -8) {
      band = band - 1;
    }
    
    if (channelIncrement == 1 ) {
      channel = channel + 1;
      if (channel > 7) {
        channel = 0;
      }
    }
    
    if (channelIncrement == -1 ) {
      channel = channel - 1;
      if (channel < 0) {
        channel = 7;
      }
    }
    
    int newChannelIndex = band * 8 + channel;

    if (newChannelIndex >= CHANNELS_SIZE) {
      newChannelIndex = newChannelIndex - CHANNELS_SIZE;
    }
    if (newChannelIndex < 0) {
      newChannelIndex = newChannelIndex + CHANNELS_SIZE;
    }
    Receiver::setChannel(newChannelIndex);
    EepromSettings.startChannel = newChannelIndex;
    EepromSettings.markDirty();
    centred = false;

    displayActiveChannel = Receiver::activeChannel;
}

// Frequency 'Centring' function.
// The function walks up and then down from the currently Rx frequency 
// in 1 MHz steps until RSSI < threshold.  The Rx is then set to the 
// centre of these 2 frequencies.
void HomeStateHandler::centreFrequency() {

  uint16_t activeChannelFreq = Channels::getFrequency(Receiver::activeChannel);
  uint16_t centerFreq = Channels::getCenterFreq(activeChannelFreq);
  Receiver::setChannel(Channels::getClosestChannel(centerFreq));
  
  wasInBandScanRegion = false;
}
bool HomeStateHandler::isInAnalyzeRegion() {
    /**
    if (TouchPad::touchData.cursorY > 130 ) {
        return true;
    } else {
        return false;
    }
    */
   if(internalSelectedMenu == 1 && menuLevel == 1){
       return true;
   } else {
       return false;
   }
}
bool HomeStateHandler::isInBandScanRegion() {
    /**
    if (TouchPad::touchData.cursorY > 130 ) {
        return true;
    } else {
        return false;
    }
    */
   if(internalSelectedMenu == 2 && menuLevel == 1){
       return true;
   } else {
       return false;
   }
}



void HomeStateHandler::bandScanUpdate() {

    //Ui::UiTimeOut.reset();
    
    if (!wasInBandScanRegion) {
        orderedChanelIndex = Channels::getOrderedIndexFromIndex(displayActiveChannel); // Start from currently selected channel to prevent initial spike artifact.
    }
    
    if (Receiver::isRssiStable() && Receiver::hasRssiUpdated) {
    
        if (!EepromSettings.quadversity) {
            Receiver::rssiBandScanData[orderedChanelIndex] = max(Receiver::rssiA, Receiver::rssiB);
        }
        if (EepromSettings.quadversity) {
            Receiver::rssiBandScanData[orderedChanelIndex] = max(Receiver::rssiA, max(Receiver::rssiB, max(Receiver::rssiC, Receiver::rssiD)));
        }
        
        if(scanNext){
            scanNext = 0;
        orderedChanelIndex = orderedChanelIndex + 1;
        if (orderedChanelIndex == CHANNELS_SIZE) {
            orderedChanelIndex = 0;
        }
            Receiver::setChannel(Channels::getOrderedIndex(orderedChanelIndex));
        }
    }
    
}
