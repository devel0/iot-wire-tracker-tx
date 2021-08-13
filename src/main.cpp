// generates a 2.5khz ( 50% duty ) square signal on pin 9
// suitable for a wire tracker transmitter
// RED WIRE -> pin 9
// BLACK WIRE -> 10k resistor -> gnd

#include <Arduino.h>

#define OUT_PIN 9
#define USE_TIMER_1 true

#include <TimerInterrupt.h>

bool val = false;

void TimerISR()
{
  digitalWrite(OUT_PIN, val ? HIGH : LOW);
  val = !val;
}

void setup()
{
  pinMode(9, OUTPUT);

  ITimer1.init();
  
  ITimer1.attachInterrupt(10000, TimerISR, 0);
}

void loop()
{
}
