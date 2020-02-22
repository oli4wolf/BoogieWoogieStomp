/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8

  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"
#include "PitchesTable.h"

// notes in the melody:
/*int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_DS4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_A4, NOTE_G4,
  NOTE_C4, NOTE_C4, NOTE_DS4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_AS4, NOTE_B4, NOTE_D4, NOTE_G4, NOTE_E4, NOTE_D4,
  NOTE_F4, NOTE_F4, NOTE_GS4, NOTE_A4, NOTE_C4, NOTE_F4, NOTE_D4, NOTE_C4
  };*/

int melodyShift[] = {
  0, 0, 3, 4, 7, 0, 9, 7,
  0, 0, 3, 4, 7, 0, 9, 7,
  7, 7, 10, 11, 14, 7, 16, 14,
  5, 5, 8, 9, 12, 5, 14, 12
};
int melodyPointer = 36;
int noteDurationVariable = 2;

// **** Buzzer Pin ****
const int buzzerPin = 8;

// **** Debounce and Input configuration ****
const int  buttonPinPitchUp = 6;
const int  buttonPinPitchDown = 7;
const int  buttonPinSpeedUp = 0;
const int  buttonPinSpeedDown = 1;
//Debounce Times
unsigned long lastDebounceTimePitchUp = 0;
unsigned long lastDebounceTimePitchDown = 0;
unsigned long lastDebounceTimeSpeedUp = 0;
unsigned long lastDebounceTimeSpeedDown = 0;
//Increase decrease Flag
int pitchUp = LOW;
int pitchDown = LOW;
int speedUp = LOW;
int speedDown = LOW;

void setup() {
  // initialize the pushbutton pin as an interrupt:
  attachInterrupt(digitalPinToInterrupt(buttonPinPitchUp), interruptDebouncePitchUp, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPinPitchDown), interruptDebouncePitchDown, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPinSpeedUp), interruptDebounceSpeedUp, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPinSpeedDown), interruptDebounceSpeedDown, CHANGE);

  // initialize Serial
  Serial.begin(9600);
}

void loop() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 32; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationVariable;
    Serial.println(melodyPointer);
    int shift = melodyShift[thisNote] + melodyPointer;
    Serial.println("Shift and Note");
    Serial.println(shift);
    Serial.println(melody[shift]);

    tone(8, melody[shift], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);

    //Check faster or slower Button pressed
    if (pitchUp == HIGH) {
      Serial.println("Pitch up");
      melodyPointer++;
      if (melodyPointer > 71) {
        melodyPointer = 71;
      }
      pitchUp=LOW;
    } else if (pitchDown == HIGH) {
      Serial.println("Pitch down");
      melodyPointer--;
      if (melodyPointer < 0) {
        melodyPointer = 0;
      }
      pitchDown=LOW;
    }

    if (speedUp == HIGH) {
      Serial.println("Speed up");
      noteDurationVariable = noteDurationVariable * 2;
      if (noteDurationVariable > 32) {
        noteDurationVariable = 32;
      }
      speedUp=LOW;
    } else if (speedDown == HIGH) {
      Serial.println("Speed down");
      noteDurationVariable = noteDurationVariable / 2;
      if (noteDurationVariable < 3) {
        noteDurationVariable = 2;
      }
      speedDown=LOW;
    }
  }
}

/** Debounce Interrupts **/
void interruptDebouncePitchUp() {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - lastDebounceTimePitchUp > 200)
  {
    pitchUp = HIGH;
  }
  lastDebounceTimePitchUp = interrupt_time;
}

void interruptDebouncePitchDown() {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - lastDebounceTimePitchDown > 200)
  {
    pitchDown = HIGH;
  }
  lastDebounceTimePitchDown = interrupt_time;
}

void interruptDebounceSpeedUp() {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - lastDebounceTimeSpeedUp > 200)
  {
    speedUp = HIGH;
  }
  lastDebounceTimeSpeedUp = interrupt_time;
}

void interruptDebounceSpeedDown() {
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - lastDebounceTimeSpeedDown > 200)
  {
    speedDown = HIGH;
  }
  lastDebounceTimeSpeedDown = interrupt_time;
}
