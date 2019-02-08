#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
// Set teensy to usbMIDI

// PINS on the octo28_adaptor
// 2 14 7 8 --  6 20 21 5
#define PIN1 2
#define PIN2 7

#define PIN3 14
#define PIN4 5

#define PIN5 21
#define PIN6 20
#define PIN7 6
#define PIN8 8

// Pins for the pots
#define POT1 19
#define POT2 22
#define POT3 23

// Eight Strips one on each edge inside the tunnel
// All Strips the same size
#define NUM_STRIPS 8

// about 55 Leds per Mirro Tunnel Segment
// 2 segment tunnel setup
#define NUM_PIXELS 110
#define NUM_SHAPE_PIXELS 8

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
CRGB shape[NUM_SHAPE_PIXELS];

uint8_t brightness = 255;
uint8_t hue = 0;
uint8_t dimFactor = 0;

// Timer for changing patterns :()
unsigned long timer = 0;
int timeStep = 100; // 2secs
int minTimeStep = 1;
int maxTimeStep = 100;
int saturationMin = 0;
int saturationMax = 255;
int saturation = 255;
int playhead = 0;
float angle = 0;
float rate = 0.005;
int shapePosition = 0;

int mode = 0;
// hue rainbow mode
int hueDrift = 0;

void setup() {

  Serial.begin(115200);
  usbMIDI.setHandleNoteOn(OnNoteOn);
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
  // Potentiometer Setup
  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(POT3, INPUT);
}

void loop() {
  // Get input from Control Panel
  int pot1 = analogRead(POT1);
  int pot2 = analogRead(POT2);
  int pot3 = analogRead(POT3);
  Serial.print("Pots: ");
  Serial.print(pot1);
  Serial.print(" ");
  Serial.print(pot2);
  Serial.print(" ");
  Serial.print(pot3);
  Serial.println(" ");

  // Map Pots to parameter values
  timeStep = map(pot1, 7, 1023, minTimeStep, maxTimeStep);
  saturation = map(pot2, 0, 1023, saturationMax, saturationMin);
  shapePosition = map(pot3, 0, 1023, 0, NUM_PIXELS);

  if(pot1 <= 6) {
    mode = 0; // calm mode.
    brightness = map(pot3, 0, 1023, 1, 255);
  } else {
    mode = 1;
    brightness = 255;
    dimFactor = map(pot3,0,1023, 250,0);
  }

  if(mode == 0) {
    // Hue Rainbow;
    hueDrift += 1;
    int hue = hueDrift;
    for (size_t i = 0; i < NUM_PIXELS; i++) {
      hue = (hue + 1 ) % 256;
      // rainbow and marquee
      
      for(size_t j = 0; j < NUM_STRIPS; j ++ ) {
        fleds[j][i] = CHSV(hue,saturation,brightness);

      }
      
    }
  } else if (mode == 1) {
    // Spinning wheel
    angle += rate;
    hue = (hue+20) %256;
    for(int i = 0; i < NUM_STRIPS; i++ ) {

      //int fade = dimFactor 
      fadeStripBy(i,dimFactor);
      // fadeStripBy(i,60 +(angle)*100);
    }
   
    if(millis() - timer > timeStep) {
      //timeStep = abs(sin(angle)*35);

      int j = playhead;
        //int h = (int)random(255);
        for(size_t i = 0; i < NUM_PIXELS; i ++) {
          fleds[j][i] = CHSV(hue,saturation,brightness);
        }
        // drawShape(j,shapePosition);

      playhead = (playhead+1) % 8;
      timer = millis();
      // Serial.print(j);
      // Serial.println(" drawing strip");

    }
  }

  

  usbMIDI.read();
  putOnStrips();
}


// * * * * * * * * * * * * * * * * *
// * * * * Strip Functions * * * * *
// * * * * * * * * * * * * * * * * *
void drawShape(int strip, int position) {
  for (int i = 0; i < NUM_SHAPE_PIXELS; ++i)
  {
    /* code */
    shape[i] = CHSV(hue,saturation,brightness);
  }

  position = position % NUM_PIXELS; // wrap around position
  int length = 8;
  for (int i = 0; i < length; ++i)
  {
    if(position < NUM_PIXELS && position >= 0) {
      fleds[strip][position+i] = shape[i];
    }
  }
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

// void clearStrip(int strip) {
//   // set all LEDS to off (Black)
//   for (uint16_t i = 0; i < strip.numPixels(); i++) {
//     // strip.setPixelColor(i, color);
//     fleds[strip][i] = CRGB::Black;
//   }
// }
void fadeStripBy(int strip, int by) {
  for (size_t i = 0; i < NUM_PIXELS; i++) {
    fleds[strip][i].fadeLightBy(by);
  }
}
// * * * * * * * * * * * * * * * * *
// * * * * *. MIDI stuff * * * * * *
// * * * * * * * * * * * * * * * * *
void OnNoteOn (byte channel, byte n, byte v) {
playhead = (playhead+1) % 8;
Serial.println("got midi");
}
/*
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
*/
