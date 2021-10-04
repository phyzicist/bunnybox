/// FroggySpeaker, version 1

// Uses TeensyDuino Audio library: https://www.pjrc.com/teensy/td_libs_Audio.html
// Decreases volume down after a specified amount of time.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <string.h>
#include <stdio.h>
#include <millisDelay.h> // Requires "SafeString" library

#define LEDR_PIN 3 // Right eye LED
#define LEDR_GND 2 // Right eye faux-ground pin
#define LEDL_PIN 5 // Left eye LED
#define LEDL_GND 9 // Left eye faux-ground pin
#define NSONGS 11
#define FADE1MINS 30 // Total minutes to wait before first fading volume
#define FADE2MINS 60 // Total minutes to wait before second fading volume
#define PREAMP 0.05 // Initial pre-amplifier value

void playFile(const char *filename);

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=193,202
AudioAmplifier           amp2; //xy=389,220
AudioAmplifier           amp1;           //xy=390,177
AudioOutputI2S           i2s1;           //xy=617,213
AudioConnection          patchCord1(playSdWav1, 0, amp1, 0);
AudioConnection          patchCord2(playSdWav1, 1, amp2, 0);
AudioConnection          patchCord3(amp2, 0, i2s1, 1);
AudioConnection          patchCord4(amp1, 0, i2s1, 0);
// GUItool: end automatically generated code

// Song List
char songs[][13] = {  // filenames are always uppercase 8.3 format
                       "02-01.wav",
                       "02-02.wav",
                       "02-03.wav",
                       "02-04.wav",
                       "02-05.wav",
                       "02-06.wav",
                       "02-07.wav",
                       "02-08.wav",
                       "02-09.wav",
                       "02-10.wav",
                       "02-11.wav",
                   };
int i = 0;

// Audio fade timing
millisDelay fadeDelay1;
millisDelay fadeDelay2;

void setup() {
  Serial.begin(9600);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);
  
  if (!(SD.begin(BUILTIN_SDCARD))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  // LED SETUP
    
  // Configure faux-ground pins
  // This is a hack for not enough ground pins on Teensy and laziness; "creating" two more ground pins for LEDs
  pinMode(LEDL_GND, OUTPUT);
  digitalWrite(LEDL_GND, LOW); 
  pinMode(LEDR_GND, OUTPUT);
  digitalWrite(LEDR_GND, LOW); 
  
  // Configure left and right LED pins
  pinMode(LEDR_PIN, OUTPUT);
  digitalWrite(LEDR_PIN, HIGH); 
  
  pinMode(LEDL_PIN, OUTPUT);
  digitalWrite(LEDL_PIN, HIGH); 
  
  // Set up speaker volume
  amp1.gain(PREAMP);
  amp2.gain(PREAMP);

  // Set up a delay timer for a fading of audio volume
  fadeDelay1.start(FADE1MINS * 60 * 1000);
  fadeDelay2.start(FADE2MINS * 60 * 1000);
  
  // Start the second song
  i = 0;
  playFile(songs[i]);
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playSdWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(50);
}


void loop() {
  if ((not playSdWav1.isPlaying())) {   
    // Increment song counter, or loop back to zero
    if (i < NSONGS - 1) {
      i++;
    }
    else {
      i = 0;
    }

    // Adjust the volume if appropriate
    if (fadeDelay1.justFinished()){
      amp1.gain(PREAMP*0.75);
      amp2.gain(PREAMP*0.75);    
    }
    if (fadeDelay2.justFinished()){
      amp1.gain(PREAMP*0.5);
      amp2.gain(PREAMP*0.5);    
    }
    
    // Play the new song
    Serial.println("Next song!");
    playFile(songs[i]);
  }
  }
