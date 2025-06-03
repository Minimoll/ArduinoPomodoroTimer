# Arduino Pomodoro Timer

A PlatformIO-based Pomodoro timer for Arduino UNO R4 WiFi that includes button debounce logic, LED indicators, an LCD display, and a buzzer melody. This project implements a standard Pomodoro workflow (25 min study, 5 min short break, 15 min long break) with four cycles, and includes unit tests for the debounce logic using Unity.

---

## Project Overview

This repository implements a Pomodoro timer on an Arduino UNO R4 WiFi. When powered on, the LCD displays a “Press button to start timer” prompt. Pressing the button—debounced in software—initiates a series of four Pomodoro cycles:

- **Study Period:** 25 minutes (LED1 ON), with a countdown displayed on a 16×2 I²C LCD.  
- **Alarm Melody:** At the end of each study period, a melody plays on a buzzer.  
- **Break Period:** 5 minutes short break (LED2 ON), except after the fourth cycle, when a 15 minute long break occurs.  
- **Repeat:** After breaks, cycles repeat until all four are completed.  

All button presses are filtered through a 50 ms debounce routine in `logic.cpp`. Unit tests (using Unity) validate this debounce logic in a host‐environment build.

---

## Features

- **Pomodoro Workflow**  
  - 25 minutes study, 5 minutes short break × 3 cycles  
  - 25 minutes study, 15 minutes long break on 4th cycle  
- **LCD Countdown**  
  - “MM:SS” format displayed on a 16×2 I²C LCD (address `0x27`)  
- **LED Indicators**  
  - `STUDY_LED` (pin 6) lights during study periods  
  - `BREAK_LED` (pin 7) lights during break periods  
- **Alarm Melody**  
  - Plays at the end of each 25 minute study session via a buzzer on `BUZZER_PIN` (pin 4)  
- **Debounced Button**  
  - Button connected to pin 8 with an external pull-down resistor  
  - Debounce timeout: 50 ms (implemented in `lib/logic/logic.cpp`)  
- **Unit Tests**  
  - Unity‐based tests for `debouncedPress()` and `pressHandler()`  
  - Runs under the `native` environment in PlatformIO  

---

## Hardware Requirements

- **Microcontroller:**  
  - Arduino UNO R4 WiFi (or any compatible AVR-based board)  
- **16×2 I²C LCD Module**  
  - SDA → A4, SCL → A5 (on UNO R4 WiFi) or as documented for your board  
  - I²C address: `0x27` (adjust in code if different)  
- **Tactile Pushbutton**  
  - One side connected to +5 V  
  - Other side connected to digital pin 8  
  - External 10 kΩ resistor from pin 8 to GND (pull-down)  
- **LEDs & Resistors**  
  - `STUDY_LED`: LED + 220 Ω resistor from pin 6 to GND  
  - `BREAK_LED`: LED + 220 Ω resistor from pin 7 to GND  
- **Buzzer / Speaker**  
  - Piezo buzzer on pin 4 (or any PWM-capable pin) to GND  

---

## Software Requirements

- **PlatformIO CLI**  
  - Installed via `pip install platformio` or using the PlatformIO IDE plugin  
- **Arduino Framework**  
  - Specified in `platformio.ini` for the `uno_r4_wifi` environment  
- **LiquidCrystal_I2C Library**  
  - Dependency: `marcoschwartz/LiquidCrystal_I2C@^1.1.2`  
- **Unity Testing Framework**  
  - Provided automatically under the `native` environment in PlatformIO  

---

## Dependencies

- [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C) (PlatformIO: `lib_deps = marcoschwartz/LiquidCrystal_I2C@^1.1.2`)

---

## Credits

- Melody and durations adapted from “Playing Popular Songs with Arduino and a Buzzer” by @tmekinyan on Arduino Project Hub:
  https://projecthub.arduino.cc/tmekinyan/playing-popular-songs-with-arduino-and-a-buzzer-546f4a
