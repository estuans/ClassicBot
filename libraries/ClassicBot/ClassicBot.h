#include "Arduino.h"
#include <TFT_ILI9163C.h>
#ifndef ClassicBot_h
#define ClassicBot_h


class TFT_ILI9163C;

class Bot {

  private:
    TFT_ILI9163C* _tft;

  public:
    Bot(TFT_ILI9163C* tft) : _tft(tft) {};
    DisplayLayer displayRegions[4];
    void sayHello();
    void bootSequence();
    void resetDisplay();
    void loop();
    void drawText(char *text, uint16_t color);
    void drawTextAt(char *text, uint16_t color, int x, int y, int size);

};

class DisplayLayer {

  public:
    int pos_x;
    int pos_y;
    int height;
    int width;
    const int center;
    DisplayLayer(int x, int y, int h, int w):
      center((int)(x + (w / 2))) {};
      pos_x(x)
      pos_y(y)
      height(h)
      width(y)
};


class Eye {

  public:
    String name;

    Eye(Bot* bot, String name, int home_x, int home_y, int height, int width);
    void setup();
    void blink(int times);
    void draw();

  private:
    Bot* _bot;
    int _home_x;
    int _home_y;
    int _x;
    int _y;
    int _height;
    int _width;
    int center;
};



#endif
