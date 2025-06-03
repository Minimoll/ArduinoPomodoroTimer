#ifndef LOGIC_H
#define LOGIC_H

extern unsigned long count;

bool debouncedPress(unsigned long currentMillis, bool buttonState);
void pressHandler();

#endif