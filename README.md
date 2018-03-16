# Mirror Tunnel Leds
A teensy 3.1/3.2 based LED controller. Driving Several WS8212 RGBW LED Strips.
Programming to Map MIDI to trigger color patterns and such.

## Hardware
Teensy 3.1/3.2
[Octo28 Adaptor](https://www.pjrc.com/store/octo28_adaptor.html)

## Tests Log
Discovered FastLEDS wont work with the RGBW type strips so sticking to NeoPixel
Library. The previous code on there was running in RGB mode and so it just had
random colors moving around on the strips.

Running Adafruit StrandTest Working on the Teensy.

## Todo

## Things to find Out
how many Strips
how many leds per strip 300
estimated power usage 18W per Meter 18Amps per 5meter strip

teensy usbback power cut??

## Ideas for Experimentation

There are 16 midi channels
each can send a lot of data
note data and CC data.

we can map CC data to control parameters of segmens on the Strips
Things like Position, Color ( R G B ),
