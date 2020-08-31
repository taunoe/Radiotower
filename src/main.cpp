/*!
 * @file main.cpp
 * 
 * @mainpage Radiotower - Freeform electronic sculpture
 * 
 * @section author Author
 * Written by Tauno Erik
 * 
 * 
 */

// Libraries
#include <Arduino.h>

#include "constants.h"
#include "pitches.h"
#include "M62429_volume.h" //


// Defines
#define DEBUG // Enable debug info serial print
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int volume_level = 40; // 30-100

/**
 * 
 */
void amp_on(){
  digitalWrite(MUTE_PIN, LOW);
  DEBUG_PRINTLN("Amp ON!");
}

/**
 * 
 */
void amp_off(){
  digitalWrite(MUTE_PIN, HIGH);
  DEBUG_PRINTLN("Amp OFF!");
}


/**
 * Function to read rotary encoder turn direction
 * Returns: -1, 0, or 1
 * -1 == Right turn
 *  1 == Left turn
 *  0 == No turn
 **/
int get_encoder_turn() {
  /**
   * The static keyword is used to create variables that are visible to only one function. 
   * However unlike local variables that get created and destroyed every time a function is called, 
   * static variables persist beyond the function call, preserving their data between function calls.
   * Variables declared as static will only be created and initialized the first time a function is called. 
   **/
  static int old_a = 1;
  static int old_b = 1;
  int result = 0;
  int new_a = digitalRead(RE_CLK_PIN);
  int new_b = digitalRead(RE_DT_PIN);

  // If the value of CLK pin or the DT pin has changed
  if (new_a != old_a || new_b != old_b) {
    if (old_a == 1 && new_a == 0) {
      result = (old_b * 2 - 1);
    }
  }

  old_a = new_a;
  old_b = new_b;

  return result;
}



int i = 0;

void setup() {
  Serial.begin(9600); // !monitor_speed = 19200
  DEBUG_PRINTLN("Radiotower begins!");
  DEBUG_PRINT("Compiled: ");
  DEBUG_PRINT(__TIME__);
  DEBUG_PRINT(" ");
  DEBUG_PRINTLN(__DATE__);
  DEBUG_PRINTLN("Made by Tauno Erik.");

  // Output Pins setup
  pinMode(MUTE_PIN, OUTPUT);
  pinMode(VOLUME_CLK_PIN, OUTPUT);
  pinMode(VOLUME_DT_PIN, OUTPUT);
  // Input Pins setup
  pinMode(RE_CLK_PIN, INPUT);
  pinMode(RE_DT_PIN, INPUT);

  set_volume(volume_level);

  amp_on();

  // https://www.arduino.cc/en/Tutorial/ToneMelody
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(TONE_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.50;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(TONE_PIN);
  }

  amp_off();
  
}

void loop() {
  int rotate = get_encoder_turn();

  // Left turn
  if (rotate > 0) {  
    //DEBUG_PRINT(rotate);
    --i;
    DEBUG_PRINT(i);
    DEBUG_PRINTLN(" Left turn");
  }
  // Right turn
  else if (rotate < 0) {
    //DEBUG_PRINT(rotate);
    ++i;
    DEBUG_PRINT(i);
    DEBUG_PRINTLN(" Right turn");
  }
/*
  Serial.print(i);
  Serial.print(" ");
  i++;
  delay(500);
  */
}