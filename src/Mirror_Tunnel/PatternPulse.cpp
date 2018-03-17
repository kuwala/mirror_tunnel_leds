#include "PatternPulse.h"

PatternPulse::PatternPulse (CRGB* leds_, int numLeds_) {
  leds = leds_;
  numLeds = numLeds_;
  hue = 0;
  brightness = 32;
  minBrightness = 0;
  maxBrightness = 64;
  color = CHSV(hue, 255, brightness);
  timer = 0;
  timeStep = 2000;
}
void PatternPulse::dim() {
  brightness -= 1;
  if (brightness < minBrightness) {
    brightness = minBrightness;
  }
}

void PatternPulse::update() {
  // if timer set maxBrightness
  // if((millis() - timer ) > timeStep) {
  //   setBrightness();
  //   timer = millis();
  // } else {
  //   dim();
  // } // end else

  // save color
  dim();
  color = CHSV(hue, 255, brightness);
  // hue = (hue + 1) %256;
  draw();
}
void PatternPulse::setBright() {
  //timer = millis();
  brightness = maxBrightness;
}
void PatternPulse::setHue(int hue_) {
  hue = hue_;
}
void PatternPulse::draw() {
  for (size_t i = 0; i < numLeds; i++) {
    leds[i] = color;
  }
}