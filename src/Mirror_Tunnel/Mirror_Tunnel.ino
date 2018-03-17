#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
#include "PatternPulse.h"
// Set teensy to usbMIDI

// PINS on the octo28_adaptor
// 2 14 7 8 6 20 21 5
#define PIN 2
#define NUM_PIXELS 300
#define SEGMENT_LEN 3
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGBW + NEO_KHZ800);

// LED Parts
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGBW + NEO_KHZ800);
// Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGBW + NEO_KHZ800);
CRGB fleds[NUM_PIXELS];
uint8_t brightness = 255;
uint8_t testHue = 0;

PatternPulse patternPulse = PatternPulse(fleds, NUM_PIXELS);

// Timer for changing patterns :()
unsigned long timer = 0;
int timeStep = 2000; // 2secs

// Overal State     0 autoLights,  1 midiInputs
int state = 0;

void setup() {

  Serial.begin(115200);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleControlChange(OnControlChange);
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    fleds[i] = CHSV(62,255,brightness);
  }

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(13,OUTPUT);
  testTest();
}


void rotateHue() {
  testHue = (testHue + 1 ) % 256;
  fleds[0] = CHSV(testHue, 255, brightness);
}


void hueOnLight() {
  int i = 0;
  strip.setPixelColor(i, strip.Color(fleds[i].g,fleds[i].r,fleds[i].b));
  // strip.show();
}
void putOnStrip() {
  int i = 0;
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(fleds[i].r,fleds[i].g,fleds[i].b));
  }
  strip.show();

}

void OnControlChange(byte c, byte n, byte v) {

}
int mapNoteToPos(byte n) {
  // int numLeds = NUM_PIXELS;
  int note = n;
  // midi note range 0 - 255;
  // led pixel range 120 (for now)
  int maped = map(note, 0, 255, 0, NUM_PIXELS / SEGMENT_LEN) * SEGMENT_LEN;
  return maped;
}

void OnNoteOn (byte c, byte n, byte v) {
  patternPulse.setHue((int)random(256));
  patternPulse.setBright();
  // map midi notes 0 - 3 to change states 0 - 3
  if (n < 4) {
    if (n==0) {
      state = 0;
    } else if (n == 1) {
      state = 1;
    }
  }

  int color = strip.Color(0,255,0);
  int pos = mapNoteToPos(n);
  Serial.print("Mapped Note to Pos: ");
  Serial.println(pos);
  segmentTest(pos, color);
  digitalWrite(13,HIGH);
}
void OnNoteOff (byte c, byte n, byte v) {
  int color = strip.Color(0,0,0);
  int pos = mapNoteToPos(n);
  segmentTest(pos, color);
  digitalWrite(13,LOW);
}

void loop() {
  // strip.show();
  usbMIDI.read();
  //delay(5);


  int state = 0;

  if (millis() - timer > timeStep) {
    //change pattern
    timer = millis();
  }

  if(state == 0) {
    //autoLights state
    patternPulse.update();

  } else if(state == 1) {
    //midiPlay state
  }

  // rotateHue();
  // hueOnLight();

  // Put the CRGB FASTLED leds onto the neopixel strip and show it
  putOnStrip();

}

void testTest() {
  for (size_t i = 40; i > 0; i--) {
  clearStrip();
  uint32_t color = strip.Color(i,255,0);
  //int pos = mapNoteToPos(i);
  segmentTest(i, color);
  strip.show();
  // delay(100);
  }
  clearStrip();
}

void clearStrip() {
  // set all LEDS to off (Black)
  uint32_t color = strip.Color(0,0,0);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    /* code */
    strip.setPixelColor(i, color);
  }
}

void segmentTest(int pos, uint32_t color) {
  // map note to a position
  uint32_t len = 3;
  // int color = strip.Color((pos * 4)% 256, (pos * 8) % 256, (pos* 2) % 256);

  // Make sure new position is in range
  if ( (strip.numPixels() - (pos + len) ) > 0 ) {

    Serial.print("segment pos : ");
    Serial.println(pos);

    for (size_t i = 0; i < len; i++) {
      int index = pos + i;
      strip.setPixelColor(index, color);
    }
    strip.show();
  } else {
    Serial.print("segment pos out of rangei: ");
    Serial.println(pos);
  }

}

// Explorations and Ideas
// midiMap
/*
I want LED segments to move around


*/

// NeoPixel Examples
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
