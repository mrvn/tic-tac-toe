#ifndef __TIMER_H__
#define __TIMER_H__ 1

/*
  Provides basic hardware timer abstraction for AVRs.

  Usage:
    Timer_Init(0);
    Timer_SetInterval(0,255);
    Timer_SetValue(0,23);
    Timer_SetWave(0,TIMER_WAVE_PWM);
    Timer_SetScale(0,TIMER_SCALE_DIV64);
    sleep(3);
    value = Timer_Value(0);
*/

class Timer {
public:
  enum Scale {
    SCALE_STOPPED,
    SCALE_1,
    SCALE_DIV_8,
    SCALE_DIV_64,
    SCALE_DIV_256,
    SCALE_DIV_1024,
  };

  enum IRQ {
    IRQ_OVERFLOW = 1,
    IRQ_OUTPUT_COMPARE_A = 2,
    IRQ_OUTPUT_COMPARE_B = 4,
    IRQ_OUTPUT_COMPARE_C = 8,
    IRQ_INPUT_COMPARE = 32,
  };

  /* convert times to ticks at compile time
   * div is typically one of 1, 8, 64, 256, 1024, corresponding to the
   * Timer::SCALE_DIV_* constant used.
   */
  template<int32_t div>
  static constexpr int32_t usec2ticks(int32_t t) {
    return int32_t(t * 1e-6 * F_CPU / div);
  }

  template<int32_t div>
  static constexpr int32_t msec2ticks(int32_t t) {
    return int32_t(t * 1e-3 * F_CPU / div);
  }
  template<int32_t div>
  static constexpr int32_t sec2ticks(int32_t t) {
    return int32_t(t * 1.0 * F_CPU / div);
  }
  
  /*
  // probably useless, as on chip we will want to calculate everything in ticks:
  // DO NOT use the TIMER_SCALE_DIV_* constant directly here.
  #define ticks2usec(t,div) ((t)*1e6*(div)/F_CPU)
  #define ticks2msec(t,div) ((t)*1e3*(div)/F_CPU)
  #define ticks2sec(t,div) ((t)*1.0*(div)/F_CPU)
  */

  virtual void scale(Scale scale) = 0;
  //virtual void wave(Wave wave) = 0;
  virtual uint16_t value() const = 0;
  virtual uint8_t irq_mask() const = 0;
  virtual void irq_mask(uint8_t mask) = 0;
  virtual uint8_t irq_flags() const = 0;
  virtual void irq_flags(uint8_t mask) = 0;
    
  void enable() {
    scale(SCALE_1);
  }

  void disable() {
    scale(SCALE_STOPPED);
  }

  void irq_enable(uint8_t mask) {
    irq_mask(irq_mask() | mask);
  }

  void irq_disable(uint8_t mask) {
    irq_mask(irq_mask() & ~mask);
  }

  // Clearing an IF means writing a 1 to it. Writing a 0 does not
  // change a thing.
  void irq_clear(uint8_t mask) {
    irq_flags(mask);
  }
};

class Timer0 : public Timer {
public:
  enum Wave {
    WAVE_NORMAL,
    WAVE_PWM,
    WAVE_CTC,
    WAVE_FAST_PWM,
    WAVE_PWM_OCRA = 5,
    WAVE_FAST_PWM_OCRA = 7,
  };

  Timer0() {
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    TCNT0  = 0;
    TIFR0  = 0xff;
  }

  void scale(Scale scale) {
    TCCR0B = (scale << CS00);
  }

  void wave(Wave wave) {
    TCCR0A  = (((wave & 2) >> 1) << WGM01) | ((wave & 1) << WGM00);
    TCCR0B  = (TCCR0B & ~(1 << WGM02)) | (((wave & 4) >> 2) << WGM02);
  }

  uint16_t value() const {
    return TCNT0;
  }

  uint8_t irq_mask() const {
    return TIMSK0;
  }

  void irq_mask(uint8_t mask) {
    TIMSK0 = mask;
  }

  uint8_t irq_flags() const {
    return TIFR0;
  }

  void irq_flags(uint8_t mask) {
    TIFR0 = mask;
  }
};
  
class Timer1 : public Timer {
public:
  enum Wave {
    WAVE_NORMAL,
    WAVE_PWM_8BIT,
    WAVE_PWM_9BIT,
    WAVE_PWM_10BIT,
    WAVE_CTC,
    WAVE_FAST_PWM_8BIT,
    WAVE_FAST_PWM_9BIT,
    WAVE_FAST_PWM_10BIT,
    WAVE_PWM_FR_ICR,
    WAVE_PWM_FR_OCRA,
    WAVE_PWM_ICR,
    WAVE_PWM_OCRA,
    WAVE_CTC_ICR,
    WAVE_FAST_PWM_ICR,
    WAVE_FAST_PWM_OCRA,
  };

  Timer1() {
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCNT1  = 0;
    TIFR1  = 0xff;
  }

  void scale(Scale scale) {
    TCCR1B = (scale << CS10);
  }

  void wave(Wave wave) {
    TCCR1A  = (((wave & 2) >> 1) << WGM11) | ((wave & 1) << WGM00);
    TCCR1B  = (TCCR1B & ~(1 << WGM12)) | (((wave & 12) >> 2) << WGM12);
  }

  uint16_t value() const {
    return TCNT1;
  }

  uint8_t irq_mask() const {
    return TIMSK1;
  }

  void irq_mask(uint8_t mask) {
    TIMSK1 = mask;
  }

  uint8_t irq_flags() const {
    return TIFR1;
  }

  void irq_flags(uint8_t mask) {
    TIFR1 = mask;
  }
};

#endif // #ifndef __TIMER_H__
