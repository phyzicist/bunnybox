// BunnyBox, version 1
// No photodetectors
// Uses the two buttons for forward/backward

// Uses Bounce2 library; example at: https://github.com/thomasfredericks/Bounce2

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>
#include <string.h>
#include <stdio.h>

void playFile(const char *filename);

// INSTANTIATE A Bounce OBJECT
Bounce btn1 = Bounce();
Bounce btn2 = Bounce();

// WE WILL attach() THE BUTTON TO THE FOLLOWING PIN IN setup()
#define BTN1_PIN 4
#define BTN2_PIN 5
#define BTN2_GND 6
#define NSONGS 40

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
                       "Itsy.wav",
                       "Baluga.wav",
                       "Hallo.wav",
                       "Crabby.wav",
                       "Wheels.wav",
                       "Snowman.wav",
                       "OpenDoor.wav",
                       "LetItGo.wav",
                       "Summer.wav",
                       "Fixer.wav",
                       "Waggles.wav",
                       "Thumb.wav",
                       "Hakuna.wav",
                       "Waltz.wav",
                       "Bananas.wav",
                       "Rover.wav",
                       "Bog.wav",
                       "BackUp.wav",
                       "King.wav",
                       "Happy.wav",
                       "Row.wav",
                       "KneeToe.wav",
                       "Xmas.wav",
                       "PBJ.wav",
                       "Jungle.wav",
                       "DuckRain.wav",
                       "SetFree.wav",
                       "Roar.wav",
                       "DownBay.wav",
                       "Shark.wav",
                       "WhereAre.wav",
                       "Banana.wav",
                       "HotDog.wav",
                       "Streets.wav",
                       "WalkWalk.wav",
                       "Happier.wav",
                       "Fox.wav",
                       "SharkEDM.wav",
                       "Tacos.wav",
                       "Proud.wav",
                       "MyShot.wav",
                       "Wild.wav",
                       "Malibu.wav",
                       "Hamilton.wav",
                       "HappyPh.wav",
                       "Gold.wav",
                       "UnderSea.wav",
                       "YouBack.wav",
                       "WhatThis.wav",
                       "ComeOut.wav",
                       "Feeling.wav",
                       "Shed.wav",
                   };
int i = 0;
                   
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

  // BUTTON SETUP 
  pinMode(BTN2_GND, OUTPUT); // This is a hack for not enough ground pins on Teensy and laziness; "creating" two more ground pins for buttons
  digitalWrite(BTN2_GND, LOW); 

  btn1.attach( BTN1_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  btn1.interval(5);   // DEBOUNCE INTERVAL IN MILLISECONDS
  //btn1.setPressedState(LOW);   // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  btn2.attach( BTN2_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  btn2.interval(5);   // DEBOUNCE INTERVAL IN MILLISECONDS
  //btn2.setPressedState(LOW);   // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  // Set up speaker volume
  double preamp = 0.05;
  amp1.gain(preamp);
  amp2.gain(preamp);

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
  btn1.update();
  btn2.update();
  if ((not playSdWav1.isPlaying()) or btn2.fell()) {   
    // Increment song counter, or loop back to zero
    if (i < NSONGS - 1) {
      i++;
    }
    else {
      i = 0;
    }
    
    // Play the new song
    Serial.println("Next song!");
    playFile(songs[i]);
  }
  else if (btn1.fell()){
    // Decrement song counter (if we are less than 5 seconds into current song), or stay at current index, or stay at zero
    if (i > 0 and playSdWav1.positionMillis() < 5000) {
      i--;
    }
    // Play the new song
    Serial.println("Prior song!");
    playFile(songs[i]);            
  }
  
  }
