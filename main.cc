#include <avr/io.h>

#include "timer.h"
#define TIMER_DIV 1024
#define TIMER_SCALE Timer::SCALE_DIV_1024
#define delay Timer::msec2ticks<TIMER_DIV>(100.0)

#define ledpin 5			// pin 13 (onboard LED)
#define baudrate 9600			// 9600 19200 38400 57600 115200

#define GREEN (1 << 13)

int main() {
  Timer1 timer1;
  timer1.wave(Timer1::WAVE_NORMAL);
  timer1.scale(TIMER_SCALE);
  int16_t update = timer1.value() + delay;
  uint16_t count = 0;

  //DDRB = 1 << ledpin;			// set pin to output
  //PORTB = 1 << ledpin;			// set pin to HIGH
  DDRD = 0xFF;				// set pin to output
  DDRB = 0x3F;				// set pin to output

  while(1) {
    int16_t time = timer1.value();	// wait for time to pass
    if (time - update < 0) {
      //PINB = 1 << ledpin;		// toggle pin
      update = update + delay;
      //count = (count << 1) | (count >> 15);
      count += 4;
      PORTD = count & 0xFF;
      PORTB = count >> 8;
    }
  }
  return 0;
}
