#ifndef MidiKeys_H
#define MidiKeys_H
// Maps Midi note input to turn on a segment on the led Strip

#include "FastLED.h"
class MidiKeys {
  public:
    MidiKeys(CRGB* leds_, NUM_LEDS);
    void update();
    void midiIn(int note);
  private:
    CRGB* leds;
    int numLeds;

};


#endif
