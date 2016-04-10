#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "defines.h"
#include "display.h"


float milliVolts = 0;
uint8_t sampleCount = 0;

void tmp34_update(void) {
  if (bit_is_clear(ADCSRA, ADSC)) {
    // Update sample buffer
    uint16_t value = ADCL | (ADCH << 8);
    milliVolts += (value * OPERATING_VOLTAGE) / 1024.0;
    sampleCount++;

    // Start next conversion
    ADCSRA |= _BV(ADSC);

    if (sampleCount >= 10) {
      milliVolts /= 10;

      // Round to 1 DP for display (offset by 10^-1)
      int16_t dispTemp = (milliVolts - 0.5) * 1000 + 0.5;

      // Generate display
      if (dispTemp < 0) display_write(CHAR_NEG);
      display_write(dispTemp / 100);
      dispTemp %= 100;
      display_write_dp(dispTemp / 10);
      display_write(dispTemp % 10);
      if (dispTemp >= 0) display_write(CHAR_SPACE);

      // Reset status
      milliVolts = 0;
      sampleCount = 0;
    }
  }
}

void tmp34_init(void) {
  // Configure ADC pre-scaler
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  // Set mux for ADC3
  ADMUX |= 0b11;
  // Enable ADC
  ADCSRA |= _BV(ADEN);
  // Start fist conversion
  ADCSRA |= _BV(ADSC);
}

void loop(void) {
  uint8_t scaler = 0;
  while(1) {
    // Call temperature update every 16 cycles.
    if (!((scaler++) % 16)) tmp34_update();
    display_update();
  }
}

int main(void) {
  wdt_disable();

  tmp34_init();
  display_init();

  loop();
  return 1;
}
