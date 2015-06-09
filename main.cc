#include <avr/io.h>

#include "timer.h"
#define TIMER_DIV 1024
#define TIMER_SCALE Timer::SCALE_DIV_1024
#define delay Timer::msec2ticks<TIMER_DIV>(1000.0)

enum { NUM_LEDS = 27, };
const uint8_t LED[NUM_LEDS][2] = {
    {10,  9}, {10,  8}, {10,  7},
    { 6,  9}, { 6,  8}, { 6,  7},
    { 2,  9}, { 2,  8}, { 2,  7},

    {10,  5}, {10,  4}, {10,  3},
    { 6,  5}, { 6,  4}, { 6,  3},
    { 2,  5}, { 2,  4}, { 2,  3},

    {10, 13}, {10, 12}, {10, 11},
    { 6, 13}, { 6, 12}, { 6, 11},
    { 2, 13}, { 2, 12}, { 2, 11},
};

int main() {
  Timer1 timer1;
  timer1.wave(Timer1::WAVE_NORMAL);
  timer1.scale(TIMER_SCALE);
  int16_t update = timer1.value();
  uint16_t count = 0;

  DDRD = 0xFF;				// set all pins to output
  DDRB = 0x3F;

  // value to have all LEDs off
  enum { OFF = 0x3BBB, };		// 10, 6, 2 low, rest high

  while(1) {
    int16_t time = timer1.value();	// wait for time to pass
    if (time - update < 0) {
      update = update + delay;

      // turn on LED[count]
      uint16_t bits = OFF ^ (1 << LED[count][0]) ^ (1 << LED[count][1]);
      PORTD = bits & 0xFF;
      PORTB = bits >> 8;      

      count = (count + 1) % NUM_LEDS;
    }
  }
  return 0;
}
