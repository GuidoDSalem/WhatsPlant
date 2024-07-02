#include "DisplayManager.h"

Display::Display():display(Display::SCREEN_WIDTH, Display::SCREEN_HEIGHT, &Wire, Display::OLED_RESET){
    // Constructor body
}

void Display::displayQR(String text){
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, text.c_str());

  int scale = min(Display::SCREEN_WIDTH / qrcode.size, Display::SCREEN_HEIGHT / qrcode.size);

  int shiftX = (Display::SCREEN_WIDTH - qrcode.size * scale)/2;
  int shiftY = (Display::SCREEN_HEIGHT - qrcode.size * scale)/2;

  for(uint8_t y =0; y < qrcode.size; y++){
    for(uint8_t x=0; x < qrcode.size; x++){
      if(qrcode_getModule(&qrcode,x,y)){
        display.fillRect(shiftX + x*scale, shiftY + y*scale, scale, scale, WHITE);
      }
    }
  }
  display.display();

}


void Display::displayText(String text){
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(text.c_str());
  display.display();      // Show initial text
  delay(100);
}
