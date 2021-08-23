// generates a square signal train ( 50% duty ) on pin 9
// suitable for a wire tracker transmitter
// RED WIRE -> pin 9
// BLACK WIRE -> 10k resistor -> gnd
//
// signal pattern sequence:
// - 50ms @2500 hz
// - 50ms @833 hz
// - 50ms @625 hz

// #define DEBUG // TODO: comment this on release

#include <Arduino.h>

#define OUT_PIN 9
#define IN_PIN 2
#define USE_TIMER_1 true

#include <TimerInterrupt.h>

#define TIMER_FREQ 10000

// 1/TIMER_FREQ * 2 * CNT_PHASE = 50ms phase switch time
int CNT_PHASE = 250;

bool val = false;
int val_cnt = 0;

int phase = 0;
int phase_cnt = 0;

volatile bool disabled = false;
volatile uint32_t m_in = millis();

void TimerISR()
{
  if (disabled)// && millis() - m_in > 500)
  {
    digitalWrite(OUT_PIN, HIGH);
    auto v = digitalRead(IN_PIN);
    if (v == LOW)
    {
      #ifdef DEBUG
      Serial.print(millis());
      Serial.println(" RE-ENABLE");
      #endif
      digitalWrite(LED_BUILTIN, LOW);
      disabled = false;
    }
    else
    {    
      return;
    }
  }

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

void inChgISR()
{
  //if (millis() - m_in > 500)
  if (!disabled)
  {
#ifdef DEBUG
    Serial.print(millis());
    Serial.println(" DISABLED");
#endif
    disabled = true;

    m_in = millis();
  }
}

void setup()
{
#ifdef DEBUG
  Serial.begin(38400);
  Serial.println("STARTUP");
#endif

  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  ITimer1.init();

  ITimer1.attachInterrupt(TIMER_FREQ, TimerISR, 0);

  attachInterrupt(digitalPinToInterrupt(IN_PIN), inChgISR, RISING);
}

uint32_t m_disabledLedBlink = 0UL;

void loop()
{
  if (disabled)
  {
    if (millis() - m_disabledLedBlink > 100)
    {
      digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW ? HIGH : LOW);
      m_disabledLedBlink = millis();
    }
  }
}
