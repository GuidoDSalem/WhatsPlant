#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode_.h"


#include "Observer.h"
//#include"Configuracion.h"


class Display{
    
  public:
    Display();
    static inline const int SCREEN_WIDTH = 128;
    static inline const int SCREEN_HEIGHT = 64;
    static inline const int SCREEN_ADDRESS = 0x3C;
    static inline const int OLED_RESET = -1; // Reset pin  

    void displayText(String text);
    void displayQR(String text);
    void clearDisplay();

  private:
  Adafruit_SSD1306 display;
  
  
  
};

#endif //DISPLAY_H
