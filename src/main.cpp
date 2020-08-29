#include <Arduino.h>

int i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Alustan:");
}

void loop() {
  Serial.print(i);
  Serial.print(" ");
  i++;
  delay(500);
}