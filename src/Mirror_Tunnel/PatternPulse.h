#ifndef PatterPulse_H
#define PatternPulse_H
#include "FastLED.h"
#include <Arduino.h>

class PatternPulse {
  public:
    PatternPulse(CRGB* leds_, int numLeds_);
    void draw();
    void update();
    void sanityCheck();
    void draw2();
    void dim();
    void setBright();
    void setHue(int hue_);

  private:
    int hue;
    int brightness;
    int minBrightness;
    int maxBrightness;

    int numLeds;
    CRGB* leds;
    CRGB color;
    unsigned int timer;
    int timeStep;


};

#endif
