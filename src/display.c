#include <util/delay.h>
#include "display.h"

/*
  Character set for 7 Segment display
   ___
  | a |
f |___| b
  | g |
e |___| c
    d   * .
*/
uint8_t character_set[] = {
  //abcdefg.
  0B11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  //abcdefg.
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110, // 9
  //abcdefg.
  0b00000001, // .
  0b00000000, // (space)
  0b00000010, // -
  0b11101110, // A
  0b00111110, // b
  0b10011100, // C
  //abcdefg.
};

uint32_t display_buffer = 0;

void display_init(void) {
  PORTB |= 0b00000111;
  DDRB |= 0b00000111;
  PORTB &= ~(0b00000111);
}

inline void _shift_out(uint8_t val) {
    uint8_t idx;
    for (idx = 0; idx < 8; idx++) {
      // Clear value on datapin
      PORTB &= ~_BV(DISPLAY_DATA_PIN);
      // Set value of datapin from the current bit
      PORTB |= !!(val & (1 << idx)) << DISPLAY_DATA_PIN;
      // Toggle clock
      PORTB |= _BV(DISPLAY_CLOCK_PIN);
      PORTB &= ~_BV(DISPLAY_CLOCK_PIN);
    }
}

void display_update(void) {
  for (uint8_t r = 0; r < 4; r++) {
    // Clear latch
    PORTB &= ~_BV(DISPLAY_LATCH_PIN);
    _shift_out(1 << r);
    _shift_out((uint8_t)(display_buffer >> (r * 8)));
    PORTB |= _BV(DISPLAY_LATCH_PIN);
    _delay_ms(0.5);
  }
}

void display_clear(void) {
  display_buffer = 0;
}

void display_write(uint8_t character) {
  display_buffer <<= 8;
  display_buffer |= character_set[character];
}

void display_write_dp(uint8_t character) {
  display_buffer <<= 8;
  display_buffer |= character_set[character] | 1;
}
