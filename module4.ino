#include "Freenove_WS2812_Lib_for_ESP32.h"
#define LEDS_COUNT 200 // The number of led
#define LEDS_PIN 21 // define the pin connected to the Freenove 8 led strip 
#define MIC_PIN 37 // define the pin connected to the Electret Microphone
#define CHANNEL 0 // RMT channel
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB); 

int brightness = 1;
int beat_threshold = 200;
const int sampleWindow = 200; // Sample window width in mS 
bool beat = true; 
bool last_beat = false; //false means last read was no beat, true means last read was a beat
int current_color = 0;
unsigned int mic_val;

void setup() {
  Serial.begin(9600);
  strip.begin();
  pinMode(MIC_PIN, INPUT);
}

void loop() {
  //Adapted from:
  //https://media.digikey.com/pdf/Data%20Sheets/Sparkfun%20PDFs/Electret_Mic_Breakout_Brd_HookupGuide_Web.pdf

  unsigned long start= millis(); // Start of sample window
  unsigned int peakToPeak = 0; // peak­to­peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for length of sample window
  while (millis() - start < sampleWindow) {
    mic_val = int(analogRead(MIC_PIN));
    if (mic_val < 1024 && mic_val > 0) {//This is the max and min of the 10­bit ADC so this loop will include all readings 
      if (mic_val > signalMax) {
        signalMax = mic_val; // save just the max levels
      } else if (mic_val < signalMin) {
        signalMin = mic_val; // save just the min levels
      }
    }
  }

  //decipher whether a beat occurred
  peakToPeak = signalMax - signalMin; // max ­ min = peak­peak amplitude
  Serial.println(peakToPeak);
  beat = (peakToPeak > beat_threshold);
  if (beat) { 
    Serial.println("BEAT!");
  }

  if (beat != last_beat) {
    //turn LEDs off or on according to whether or not a beat was detected
    for (int i=0; i < LEDS_COUNT; i++) {
      if (beat) {
        strip.setLedColorData(i, 0, 0, 0);
      } else {
        strip.setLedColorData(i, current_color); //TODO: make this randomized
      }
    }
    strip.show();
  }
  last_beat = beat;
  current_color = random(0, 256);
  }
