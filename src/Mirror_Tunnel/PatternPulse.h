#ifndef PatterPulse_H
#define PatternPulse_H
#include "FastLED.h"
#include <Arduino.h>

class PatternPulse {
  public:
    PatternPulse(CRGB* leds_, int numLeds_);
    void draw();
    void update();
    void dim();
    void setBright();
    void setHue(int hue_);

  private:
    uint8_t hue;
    uint8_t brightness;
    uint8_t minBrightness;
    uint8_t maxBrightness;

    int numLeds;
    CRGB* leds;
    CRGB color;
    unsigned int timer;
    int timeStep;


};

#endif
