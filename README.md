https://discord.gg/gG2sjjK

Diversity receiver designed around an ESP32 and touchpad UI.

  *   15 kHz antenna switching rate e.g. once every NTSC video line.
  *   72 selectable frequencies
  *   Rapid spectrum analyser with selectable peaks.  See below for more details.
  *   Switch from diversity mode, to single antenna mode (A or B).

Menu demo (36 seconds) https://www.youtube.com/watch?v=VTEIN2PMyIs

Indoor flight (29 seconds) https://www.youtube.com/watch?v=gvrL2I_6KYo

<img src="docs/img/InGoggles.jpg" width="50%">

**Calibration**

When first built and turned on you will be greeted with a calibration screen.  Set a VTx to 25 mW and on your favourite band/frequency (F4 for 5800 MHz) and place the VTx next to the goggles.  Tap the touchpad and the module will sweep all frequencies tracking maximum and minimum RSSI values.  When completed you will see the final min/max values for each receiver and can tapping again saves these values.
press any button to step the process.
The calibration process can be enhanced by selecting "enhance calibration" from menu or reset by selecting "factory reset" from internal settings menu.

**Diversity or single antenna selection**

Tapping on mode cycles through diversity, antenna A, and antenna B modes.

**button navigation**

use fatshark up/down CH as the button. changing value (going up or going down) will cycle
menu. to simulate enter button, change the direction, for example, after cycling with up button, to select the item, press the down button.

**note : due to fatshark "garbage" button system as Jye said, when button up or down has been pressed too many time (fatshark will do longer beep when pressed), menu cycling will stop. to keep cycling the menu, go with the opposite direction, it will not simulate enter button from a max-ed value button.

## BOM

  *   1 x PCB - Upload the Gerber.zip found in the PCB folder to jlcpcb.com, select 1mm thickness and your favourite colour. https://easyeda.com/jyesmith/esp32-fatshark-module-v3
  *   1 x GlidePoint circle pads Part Number (SPI) TM023023-2024-002
     *   https://www.cirque.com/glidepoint-circle-trackpads
     *   https://au.mouser.com/ProductDetail/Cirque/TM023023-2024-002?qs=sGAEpiMZZMs0s%252BS7KO2gVrGbL5oW3XVsIhhcv%2FqrLlZvbAloChXE1Q%3D%3D
  *   1 x TTGO Micro-32
     *   https://www.aliexpress.com/item/Ttgo-Micro-32-V2-0-Wifi-Wireless-Bluetooth-Module-Esp32-Pico-D4-Ipex-Esp-32/32982085061.html
  *   2 x RX5808.  The receiver linked below has given better results than the more commonly available receiver (the version without the 2 extra tabs).  The RSSI voltage range is  larger and the minimum RSSI is consistent across all frequencies.
     *   https://www.aliexpress.com/item/Swiftflying-Boscam-FPV-5-8G-Wireless-Audio-Video-Receiving-Module-For-RX5808/32841092602.html
  *   2 x Antenna Connectors
     *   https://www.aliexpress.com/item/5-pcs-RF-Coaxial-Long-SMA-Female-Straight-Edge-15mm-tooth-PCB-Connector-Adapter/32913905715.html
  *   3 x  Switch CD4065
  *   3.3V Regulator AMS1117-3.3
  *   2 x 10uF 0603 SMD Capacitors
  *   1 x 0.1uF 0603 SMD Capacitor
  *   1 x 10k 0603 SMD Resistors (EN pullup)
  *   2 x 100k 0603 SMD Resistors (RSSI pulldown)
  *   3 x 510 ohm 0603 SMD Resistors (LEDs) (Optional)
  *   3 x 0603 LEDs.  1 (red) for power and 2 (green) for indicating which antenna is selected (Optional)
  *   1 x boot button
  *   1 x 9 pin header
  *   2 x 4mm M2 standoffs and screws

# Wiring

https://easyeda.com/jyesmith/esp32-fatshark-module-v3

<img src="PCB/Schematic.png" width="50%"> <img src="PCB/PCB.png" width="50%">

The touchpad works better with a cover.  I use a piece of electric tape.

<img src="docs/img/InGoggles.jpg" width="50%"> <img src="docs/img/BackFinished.jpg" width="50%">

<img src="docs/img/Front1Build.jpg" width="50%"> <img src="docs/img/Front2Build.jpg" width="50%">

<img src="docs/img/Front3Build.jpg" width="50%"> <img src="docs/img/TouchPadWiring.png" width="50%">

<img src="docs/img/BackBuild.jpg" width="50%">

## Flashing
initial flashing :
Tx and Rx pins to the TTGO are broken out to the upper 2 pins that plug into the goggles.  Connect these, along with GND (pin 3rd from bottom) and 5V (bottom pin), then flash using your favourite FTDI.

next update flashing can use OTA.

## Recognition

This code is a forked from [sheaivey/rx5808-pro-diversity](https://github.com/sheaivey/rx5808-pro-diversity).

## Code

The code is distributed under the [MIT license](LICENSE.md).
