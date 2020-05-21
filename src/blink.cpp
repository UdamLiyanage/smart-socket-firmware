#include <Arduino.h>

int blink(int blinkPin) {
    digitalWrite(blinkPin, HIGH);
    delay(250);
    digitalWrite(blinkPin, LOW);
    delay(250);
    digitalWrite(blinkPin, HIGH);
    delay(250);
    digitalWrite(blinkPin, LOW);
    delay(250);
    digitalWrite(blinkPin, HIGH);
    delay(2000);
    return 1;
}