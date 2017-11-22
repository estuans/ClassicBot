#include "Arduino.h"
#include "ClassicBot.h"
#include "pins_arduino.h"
#include "wiring_private.h"
#include "_resources/img.h"
#include "_resources/settings.h"


char* str2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}


void Bot::drawTextAt(char *text, uint16_t color, int x, int y, int size=2) {
  _tft->setCursor(x, y);
  _tft->setTextColor(color);
  _tft->setTextWrap(true);
  _tft->setTextSize(size);
  _tft->print(text);
};

void Bot::drawText(char *text, uint16_t color) {
  _tft->setCursor(1, 1);
  _tft->setTextColor(color);
  _tft->setTextWrap(true);
  _tft->setTextSize(2);
  _tft->print(text);
};

void Bot::sayHello(){
  analogWrite(TFT_LITE, 96);
  _tft->drawBitmap(0,0, helloBitmap,128,128, WHITE, BLACK);
};

void Bot::bootSequence(){
  _tft->drawBitmap(0,0, searchBoot,128,128, WHITE, BLACK);
  delay(2000);
  _tft->drawBitmap(0,0, bootMac,128,128, WHITE, BLACK);
  delay(5000);

  DisplayLayer clock_region = defineRegion(CLOCK_X, CLOCK_Y, CLOCK_HEIGHT, CLOCK_WIDTH);
  DisplayLayer face_region = defineRegion(FACE_X, FACE_Y, FACE_HEIGHT, FACE_WIDTH);


};

void Bot::resetDisplay(){
  _tft->fillScreen(BG_COLOUR);
};

DisplayLayer Bot::defineRegion(int x, int y, int h, int w){
  for(int i = 0; i < sizeof(displayRegions); i++){
    next = displayRegions[i];
    if !next {
      DisplayLayer dl = new DisplayLayer(x,y,h,w);
      displayRegions[i] = dl;
      return dl;
    }
  }
};

void Bot::homeScreen(){
  // Draw basic Eyes
  _tft->fillRoundRect(LEFT_EYE_X, LEFT_EYE_Y, DEFAULT_EYE_WIDTH, DEFAULT_EYE_HEIGHT, 2, FG_COLOUR);
  _tft->fillRoundRect(RIGHT_EYE_X, RIGHT_EYE_Y, DEFAULT_EYE_WIDTH, DEFAULT_EYE_HEIGHT, 2, FG_COLOUR);

};

void Bot::loop(){
  resetDisplay();
  // Draw a pair of eyes, then start the blink loop
  Eye eyes[] = {
      Eye(this, "left", LEFT_EYE_X, LEFT_EYE_Y, DEFAULT_EYE_WIDTH, DEFAULT_EYE_HEIGHT),
      Eye(this, "right", RIGHT_EYE_X, RIGHT_EYE_Y, DEFAULT_EYE_WIDTH, DEFAULT_EYE_HEIGHT)
    };

  for(int i=0;i < sizeof(eyes);i++){
    drawText(str2char(eyes[i].name), BLACK);
    //eyes[i].draw();
  }


};


Eye::Eye(Bot* bot, String name, int home_x, int home_y, int height, int width){
  _bot = bot;
};

void Eye::draw(){
  _bot->drawText(str2char(this->name), RED);
  //_tft->fillRoundRect(_x, _y, _width, _height, 2, FG_COLOUR);
};

void Eye::blink(int times){

  int counter = 0;

  while (counter <= times){
    counter++;
  }
};


DisplayLayer::DisplayLayer(int x, int y, int h, int w):{}
