#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "defines.h"
#include "display.h"

void tmp34_init(void) {
  // Configure pre-scaler
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  // Set mux for ADC3
  ADMUX |= 0b11;
  // Enable ADC
  ADCSRA |= _BV(ADEN);
}

uint16_t tmp34_read(void) {
  // Start conversion
  ADCSRA |= _BV(ADSC);
  // Wait till finished
  while (ADCSRA & _BV(ADSC)) {}
  // Return value
  return ADCL | (ADCH << 8);
}

void tmp34_update(void) {
  uint8_t idx;

  // Calculate millivolts averaged over 10 samples
  float milliVolts = 0.0;
  for (idx = 0; idx < 5; idx++) {
    milliVolts += (tmp34_read() * 3.3) / 1024.0;
  }
  milliVolts /= 5;

  // Round to 1 DP for display (offset by 10^-1)
  int16_t dispTemp = (milliVolts - 0.5) * 1000 + 0.5;

  // Generate display
  display_write((dispTemp < 0) ? CHAR_NEG : CHAR_SPACE);
  display_write(dispTemp / 100);
  dispTemp %= 100;
  display_write_dp(dispTemp / 10);
  display_write(dispTemp % 10);
}

void init(void) {
  wdt_disable();

  tmp34_init();
  display_init();
}

void loop(void) {
  uint8_t scaler = 0;
  while(1) {
    if (!scaler) tmp34_update();
    scaler += 2;
    display_update();
  }
}

int main(void) {
  init();
  loop();
  return 1;
}
