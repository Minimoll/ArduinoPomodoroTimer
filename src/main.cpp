#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include "melody.h"
#include "logic.h"

// ────────────────────────────────────────────────────────────────────────────────
//                            GLOBAL VARIABLES & CONSTANTS
// ────────────────────────────────────────────────────────────────────────────────

// Current elapsed seconds (0–59) during a countdown.
int seconds = 0;

// Current elapsed minutes during a countdown.
int minutes = 0;

// Keeps track of how many cycles (study+break) have run so far.
int counter = 0;

// Number of minutes in a single study period.
const int STUDY_MINUTES = 25;

// Number of minutes in a short break.
const int SHORT_BREAK_MINUTES = 5;

// Number of minutes in the long break.
const int LONG_BREAK_MINUTES = 15;

// Total number of Pomodoro cycles (study → break) to run.
const int REPEATS = 4;

// LCD- Address, columns, rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Green LED for studying.
const int STUDY_LED = 6;

// Red LED for break.
const int BREAK_LED = 7;

const int BUTTON_PIN = 8;
const int BUZZER_PIN = 4;

int break_duration;


// ────────────────────────────────────────────────────────────────────────────────
//                            FUNCTION PROTOTYPES
// ────────────────────────────────────────────────────────────────────────────────
void playMelody();
void waitForButtonPress();

// ────────────────────────────────────────────────────────────────────────────────
//                                  SETUP
// ────────────────────────────────────────────────────────────────────────────────
void setup() {
  pinMode(STUDY_LED, OUTPUT);
  pinMode(BREAK_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); 

  // Initialize the LCD, enable backlight, and prompt the user.
  lcd.init();
  lcd.backlight();
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("start timer");

  // Wait here until the user presses BUTTON_PIN (goes from LOW to HIGH).
  waitForButtonPress();
  lcd.clear();
}

// ────────────────────────────────────────────────────────────────────────────────
//                                   LOOP
// ────────────────────────────────────────────────────────────────────────────────
void loop() {
  // Reset cycle counter at the start of each full run.
  counter = 0;
  
  // Repeat “study period + break” exactly 4 times.
  while (counter < REPEATS) {
    // ──────────────── STUDY PERIOD ───────────────────
    lcd.clear();
    lcd.print("Study Time!");

    // Turn on the study LED, turn off the break LED.
    digitalWrite(STUDY_LED, HIGH);
    digitalWrite(BREAK_LED, LOW);
    lcd.setCursor(0, 1);

    seconds = 0;
    minutes = 0;

    while (minutes < STUDY_MINUTES) {
      seconds = 0;
      while (seconds < 60) {
        lcd.setCursor(0, 1);
        if (minutes < 10) {
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) {
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        seconds++;
      }
      minutes++;
    }

    // After 25 minutes, play the melody to signal the end of the study period.
    playMelody();

    // Turn off study LED, turn on break LED for the upcoming break period.
    digitalWrite(STUDY_LED, LOW);
    digitalWrite(BREAK_LED, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);

    // If this is the final cycle take a long break, otherwise, a short break.
    if (counter == (REPEATS - 1)) {
      break_duration = LONG_BREAK_MINUTES;
      lcd.print("Long Break");
    }
    else {
      break_duration = SHORT_BREAK_MINUTES;
      lcd.print("Short Break");
    }
    lcd.setCursor(0, 1);
    seconds = 0;
    minutes = 0;

    while (minutes < break_duration) {
      seconds = 0;
      while (seconds < 60) {
        lcd.setCursor(0, 1);
        if (minutes < 10) {
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) {
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        seconds++;
      }
      minutes++;
    }

    // One full “study + break” cycle is done, increment counter.
    counter++;
  }
}
void playMelody() {
  int notesCount = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notesCount; i++) {
    int note     = melody[i];
    int length   = durations[i];
    int noteMs   = (1000 / length) * 1.3;    // e.g. length=4 → 250 ms

    if (note == 0) {
      // Rest: no sound, just wait
      delay(noteMs);
    } else {
      // Play the note at the given frequency for noteMs milliseconds
      tone(BUZZER_PIN, note, noteMs);
      delay(noteMs);
      noTone(BUZZER_PIN);
    }
    // Short pause between notes (30 ms)
    delay(30);
  }
}

void waitForButtonPress() {
  while (true) {
    bool rawState = (digitalRead(BUTTON_PIN) == HIGH);
    if (debouncedPress(millis(), rawState)) {
      pressHandler();
      break;
    }
  }
}