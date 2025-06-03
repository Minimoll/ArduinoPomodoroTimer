#include "logic.h"

unsigned long count = 0;
static bool lastButtonState = false;
static unsigned long timeStamp = 0;
#define DEBOUNCE_MS 50

bool debouncedPress(unsigned long currentMillis, bool buttonState)
{
  if (currentMillis - timeStamp > DEBOUNCE_MS) {
    if (buttonState != lastButtonState) {
      timeStamp = currentMillis;
      lastButtonState = buttonState;
      if (buttonState) { // true = pressed
        return true;
      }
    }
  }
  return false;
}

void pressHandler()
{
  count++;
}