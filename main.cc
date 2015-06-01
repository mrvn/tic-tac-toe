#include <avr/io.h>

#include "timers.h"
#define TIMER_DIV 1024
#define TIMER_SCALE TIMER_SCALE_DIV_1024
#define delay msec2ticks(1000.0,TIMER_DIV)

#define ledpin 5			// pin 13 (onboard LED)
#define baudrate 9600			// 9600 19200 38400 57600 115200

int main() {
  Timer_Init(1);
  Timer_SetWave(1,TIMER1_WAVE_NORMAL);
  Timer_SetScale(1,TIMER_SCALE);
  int16_t update = Timer_Value(1) + delay;

  DDRB = 1 << ledpin;			// set pin to output
  PORTB = 1 << ledpin;			// set pin to HIGH

  while(1) {
    int16_t time = Timer_Value(1);	// wait for time to pass
    if (time - update < 0) {
      PINB = 1 << ledpin;		// toggle pin
      update = update + delay;
    }
  }
  return 0;
}
