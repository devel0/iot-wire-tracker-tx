// generates a square signal train ( 50% duty ) on pin 9
// suitable for a wire tracker transmitter
// RED WIRE -> pin 9
// BLACK WIRE -> 10k resistor -> gnd
//
// signal pattern sequence:
// - 50ms @2500 hz
// - 50ms @833 hz
// - 50ms @625 hz

#include <Arduino.h>

#define OUT_PIN 9
#define USE_TIMER_1 true

#include <TimerInterrupt.h>

#define TIMER_FREQ 10000

// 1/TIMER_FREQ * 2 * CNT_PHASE = 50ms phase switch time
int CNT_PHASE = 250;

bool val = false;
int val_cnt = 0;

int phase = 0;
int phase_cnt = 0;

void TimerISR()
{
  if (phase_cnt > CNT_PHASE)
  {
    ++phase;
    if (phase > 2)
      phase = 0;
    phase_cnt = 0;
  }

  switch (phase)
  {

    // TIMER_FREQ / 4 = 2500hz
  case 0:
  {
    val = !val;
    digitalWrite(OUT_PIN, val ? HIGH : LOW);
  }
  break;

    // TIMER_FREQ / (4*3) = 833hz
  case 1:
  {
    if (val_cnt > 2)
    {
      val = !val;
      digitalWrite(OUT_PIN, val ? HIGH : LOW);
      val_cnt = 0;
    }
  }
  break;

    // TIMER_FREQ / (4*4) = 625hz
  case 2:
  {
    if (val_cnt > 3)
    {
      val = !val;
      digitalWrite(OUT_PIN, val ? HIGH : LOW);
      val_cnt = 0;
    }
  }
  break;
  }

  ++val_cnt;
  ++phase_cnt;
}

void setup()
{
  pinMode(9, OUTPUT);

  ITimer1.init();

  ITimer1.attachInterrupt(TIMER_FREQ, TimerISR, 0);
}

void loop()
{
}
