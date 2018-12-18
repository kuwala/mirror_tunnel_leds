#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
// Set teensy to usbMIDI

// PINS on the octo28_adaptor
// 2 14 7 8 --  6 20 21 5
#define PIN1 2
#define PIN2 7

#define PIN4 8

#define PIN5 6
#define PIN6 20
#define PIN7 21
#define PIN8 5

// Eight Strips one on each edge inside the tunnel
// All Strips the same size
#define NUM_STRIPS 8

// about 55 Leds per Mirro Tunnel Segment
// 2 segment tunnel setup
#define NUM_PIXELS 110

// Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGBW + NEO_KHZ800);

// LED Parts

// Neopixel strips that drive the LED
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS, PIN1, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, PIN2, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_PIXELS, PIN3, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_PIXELS, PIN4, NEO_RGBW + NEO_KHZ800);

Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_PIXELS, PIN5, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUM_PIXELS, PIN6, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip7 = Adafruit_NeoPixel(NUM_PIXELS, PIN7, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip8 = Adafruit_NeoPixel(NUM_PIXELS, PIN8, NEO_RGBW + NEO_KHZ800);

// Fast led strips used for easier color manipulation
// CRGB fleds[NUM_PIXELS];
// CRGB fleds2[NUM_PIXELS];
// CRGB fleds3[NUM_PIXELS];
// CRGB fleds4[NUM_PIXELS];
// CRGB fleds5[NUM_PIXELS];
// CRGB fleds6[NUM_PIXELS];
// CRGB fleds7[NUM_PIXELS];
// CRGB fleds8[NUM_PIXELS];

CRGB fleds[NUM_STRIPS][NUM_PIXELS];

uint8_t brightness = 64;

// Timer for changing patterns :()
unsigned long timer = 0;
int timeStep = 200; // 2secs

void setup() {

  Serial.begin(115200);
  // usbMIDI.setHandleNoteOn(OnNoteOn);
  // usbMIDI.setHandleNoteOff(OnNoteOff);
  // usbMIDI.setHandleControlChange(OnControlChange);

  // do some kind of startup animation
 

  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
  strip6.begin();
  strip7.begin();
  strip8.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
  strip3.show(); // Initialize all pixels to 'off'
  strip4.show(); // Initialize all pixels to 'off'
  strip5.show(); // Initialize all pixels to 'off'
  strip6.show(); // Initialize all pixels to 'off'
  strip7.show(); // Initialize all pixels to 'off'
  strip8.show(); // Initialize all pixels to 'off'
  pinMode(13,OUTPUT);
  // testTest();
}

void loop() {
 
  if(millis() - timer > timeStep) {
    if(playhead == 0) {
      for(size_t i = 0; i < NUM_PIXELS; i ++) {
        fleds[i] = CHSV(h,255,brightness);
      }
    } else if(playhead == 1) {

    }

    playhead = (playhead+1) % 8;

  }

  usbMIDI.read();
  putOnStrips();
}


void putOnStrip() {
  // For Testing just the first 1 strip
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(fleds[i].g,fleds[i].r,fleds[i].b));
  }
  strip.show();
}
void putOnStrips() {
  // Puts the FastLED leds onto the NeoPixel Leds
  
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    strip1.setPixelColor(i, strip1.Color(fleds[0][i].g,fleds[0][i].r,fleds[0][i].b));
    strip2.setPixelColor(i, strip2.Color(fleds[1][i].g,fleds[1][i].r,fleds[1][i].b));
    strip3.setPixelColor(i, strip3.Color(fleds[2][i].g,fleds[2][i].r,fleds[2][i].b));
    strip4.setPixelColor(i, strip4.Color(fleds[3][i].g,fleds[3][i].r,fleds[3][i].b));
    strip5.setPixelColor(i, strip5.Color(fleds[4][i].g,fleds[4][i].r,fleds[4][i].b));
    strip6.setPixelColor(i, strip6.Color(fleds[5][i].g,fleds[5][i].r,fleds[5][i].b));
    strip7.setPixelColor(i, strip7.Color(fleds[6][i].g,fleds[6][i].r,fleds[6][i].b));
    strip8.setPixelColor(i, strip8.Color(fleds[7][i].g,fleds[7][i].r,fleds[7][i].b));
  }
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
  strip6.show();
  strip7.show();
  strip8.show();
}

void OnControlChange(byte c, byte n, byte v) {

}

void OnNoteOn (byte channel, byte n, byte v) {
  // map midi notes 0 - 3 to change states 0 - 3
  if (n < 4) {
    if (n==0) {
      // auto rainbow
      state = 0;
      Serial.println("state set to 0");
    } else if (n == 1) {
      // pulse
      state = 1;
      Serial.println("state set to 1");
    } else if (n == 2) {
      // marquee
      state = 2;
      Serial.println("state set to 2");
    } else if (n == 3) {
      state = 3;
      Serial.println("state set to 3");
    }
  }

  if (state == 1) {
    // pulse
    if (n == 5) {
      patternPulse.setHue((int)random(256));
      patternPulse.setBright();
    } else if (n == 6) {
      patternPulse2.setHue((int)random(256));
      patternPulse2.setBright();
    } else if (n == 7) {
      patternPulse3.setHue((int)random(256));
      patternPulse3.setBright();
    } else if (n == 8) {
      patternPulse4.setHue((int)random(256));
      patternPulse4.setBright();
    }


    // pulse whole strip

  } else if (state == 2) {
    // move midi note around
    int color = strip.Color(0,255,0);
    int pos = mapNoteToPos(n);
    Serial.print("Mapped Note to Pos: ");
    Serial.println(pos);
    segmentTest(pos, color);

  }
    // patternPulse.setHue((int)random(256));
    // patternPulse.setBright();

  digitalWrite(13,HIGH);
}
void OnNoteOff (byte c, byte n, byte v) {
  if (state == 0) {
    // patternPulse2.setHue((int)random(256));
    // patternPulse2.setBright();
    // patternPulse.setHue((int)random(256));
    // patternPulse.setBright();
  } else if (state == 1) {
    // turn off midi note
    int color = strip.Color(0,0,0);
    int pos = mapNoteToPos(n);
    segmentTest(pos, color);
  }
  digitalWrite(13,LOW);
}

int mapNoteToPos(byte n) {
  // int numLeds = NUM_PIXELS;
  int note = n;
  // midi note range 0 - 255;
  // led pixel range 120 (for now)
  int maped = map(note, 0, 255, 0, NUM_PIXELS / SEGMENT_LEN) * SEGMENT_LEN;
  return maped;
}

void clearStrip() {
  // set all LEDS to off (Black)
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    // strip.setPixelColor(i, color);
    fleds[i] = CRGB::Black;
  }
}
void fadeStripBy(int by) {
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    fleds[i].fadeLightBy(by);
  }
}
