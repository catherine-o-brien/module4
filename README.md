## Musically Responsive LED Disco Ball

![Imgae of hollow spherical shape composed of LED lights and clear tubing, lit up in many colors](https://catherine-o-brien.github.io/images/musical-led-disco-ball/disco-ball.png "The disco ball I created!")

I have always wanted to create some sort of display that responds to music, and this final project for my Creative Embedded Systems class seemed like the perfect opportunity! I decided to create a disco ball-inspired LED display that responds to music. To do so, I put an LED strip into clear piping, shaped the piping into a sphere-like shape, and connected the LED strip to an ESP32 and a microphone. Using the data from the microphone, the LED strip lights up in different patterns.

[View this project on my website!](catherine-o-brien.github.io/musical-led-disco-ball)

## MATERIALS
* **Arduino ESP-32 TTGO T-display** [like this one](https://www.amazon.com/LILYGO-T-Display-Arduino-Development-CH9102F/dp/B099MPFJ9M)
* **USB-C cord** (make sure that your cord supports data transfer, not just power– [see more about that here](https://www.dignited.com/50330/usb-data-cable-vs-usb-charging-cable/). For this project, you will use this cable both to upload code to your device during development and to power the device once it is finished.)
* **Microphone** (I used the [Arduino Sound Sensor](https://microcontrollerslab.com/ky-038-microphone-sound-sensor-module-arduino-tutorial/))
* **Breadboard** (this isn't actually necessary, but very helpful for prototyping and troubleshooting your project!)
* **5V Battery pack**
* **LED Strip** (I used [this one](https://www.amazon.com/BTF-LIGHTING-Flexible-Individually-Addressable-Non-waterproof/dp/B01CDTEG1O/ref=sr_1_8?crid=3N19D1Q6X1QOS&keywords=programmable%2Bled%2Bstrips&qid=1681838388&sprefix=programmable%2Bled%2Bstri%2Caps%2C146&sr=8-8&th=1))
* **Clear flexible tubing** (encloses the LED strip)
* **Hearty wire and wire cutters** (needs to be strong enough to hold the flexible tubing in the spherical shape– I used wire about 4mm thick, similar to [this one](https://www.amazon.com/TecUnite-Aluminum-Bendable-Skeleton-Thickness/dp/B07CQL7Y5B/ref=sr_1_2_sspa?keywords=4mm%2Bwire&qid=1683783492&sr=8-2-spons&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFNTFhSVjlUQktLSUYmZW5jcnlwdGVkSWQ9QTAyNDY3ODVUVVlCSjRaQkpDRDAmZW5jcnlwdGVkQWRJZD1BMDU2NjI2MzIwMFFKNVpLS1JTRlImd2lkZ2V0TmFtZT1zcF9hdGYmYWN0aW9uPWNsaWNrUmVkaXJlY3QmZG9Ob3RMb2dDbGljaz10cnVl&th=1)) 
* **Xacto knife**
* **Arduino IDE** [download here](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)

## PROCESS
# HARDWARE
Using wires, I connected an ESP32 to an Arduino Sound Sensor, a microphone. The ESP32 provides power to the Arduino Sound Sensor, and it also reads data from the Sound Sensor's analog connection. I did not use the digital connection on the Sound Sensor for this project. I also connected an LED strip to a battery pack as a power supply, and then connected its data connection to the ESP32. The ESP32 is powered by a USB-C cable. 

[Check out my website for more specifics on hardware.](catherine-o-brien.github.io/musical-led-disco-ball)

I flashed my code from the Arduino IDE onto my ESP32, where it runs, using the USB-C cable.  

# SOFTWARE
Using the hardware setup, the ESP32 reads in all the analog values the microphone outputs over a 250 millisecond period. It tracks the highest and lowest values in that range, and when the 250 millisecond period is up, it computes the range of values it read. 
If the range of the values exceeds the threshold of 200, called in my code the beat_threshold, then the ESP32 registers a beat. 

```
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
```
The program then checks if the current beat matches the last beat. If they do match (ie the program just detected a beat and the last iteration was also a beat, or the opposite), then the program does nothing, because it should continue the same behavior as the previous iteration. 
If they do not match, its behavior depends on whether or not it detected a beat. If a beat was detected, the LED strip flashes off. If a beat was not detected, the LED strip flashes back on with a new color. 

```
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
```

## FINAL DEVICE
Check out the final product here! 
<iframe width="560" height="315" src="https://www.youtube.com/embed/Va2wmVxZMc4" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
