#include <avr/io.h>

#include "timer.h"
#define TIMER_DIV 1024
#define TIMER_SCALE Timer::SCALE_DIV_1024
#define delay Timer::usec2ticks<TIMER_DIV>(100.0)

class Display {
public:
  //  Display() : leds_(0x0FA688LU), count_(0) {
  Display() : leds_(0x7FAC688), count_(0) {
    DDRD = 0xFF;				// set pin to output
    DDRB = 0x3F;				// set pin to output
    PORTD = 0;
    PORTB = 0;
  }
  ~Display() {
    DDRD = 0;				// set pin to input
    DDRB = 0;				// set pin to input
    PORTD = 0;
    PORTB = 0;
  }
  void tick() {
    // turn on LED[count_]
    uint16_t bits = OFF;
    if ((leds_ & (1LU << count_)) != 0) {
      bits = bits ^ (1 << LED[count_][0]) ^ (1 << LED[count_][1]);
    }
    PORTD = bits & 0xFF;
    PORTB = bits >> 8;
    count_ = (count_ + 1) % NUM_LEDS;
  }
private:
  enum {
    NUM_LEDS = 27,
    OFF = 0x3BBB,		// 10, 6, 2 low, rest high
  };
  static const uint8_t LED[NUM_LEDS][2];
  uint32_t leds_;
  uint32_t count_;
};

const uint8_t Display::LED[NUM_LEDS][2] = {
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

Display display;

int main() {
  Timer1 timer1;
  timer1.wave(Timer1::WAVE_NORMAL);
  timer1.scale(TIMER_SCALE);
  int16_t update = timer1.value();

  while(1) {
    int16_t time = timer1.value();	// wait for time to pass
    if (time - update < 0) {
      update = update + delay;

      display.tick();
    }
  }
  return 0;
}
