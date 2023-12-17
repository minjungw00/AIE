#include <Adafruit_NeoPixel.h>
 
//define NeoPixel Pin and Number of LEDs
#define PIN 13
#define NUM_LEDS 32
//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
int i = 0;
 
void setup() {
  // start the strip and blank it out
  strip.begin();
  strip.show();
}
 
void loop() {
  strip.setPixelColor(0, 120, 60, 0);
 
  strip.show();
  delay(80);
 
}