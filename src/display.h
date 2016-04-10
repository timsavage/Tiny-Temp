#ifndef _DISPLAY__H
#define _DISPLAY__H

#include "defines.h"

#define CHAR_DP     10
#define CHAR_SPACE  11
#define CHAR_NEG    12
#define CHAR_A      13
#define CHAR_LOWER(c) (c - 'a' + CHAR_A)
#define CHAR_UPPER(c) (c - 'A' + CHAR_A)

/**
 * Initialise display
 */
void display_init(void);
/**
 * Update the display (this should be called as often as possible in run loop).
 *
 * Writes the current display buffer to a pair of 74HC595 latched shift registers.
 */
void display_update(void);

/**
 * Clear the display buffer.
 */
void display_clear(void);
/**
 * Write a character to the display and shifts the buffer left.
 *
 * Character map:
 * 	0-9 - Send value.
 *  DP - Use CHAR_DP
 *  Space - Use CHAR_SPACE
 *  Dash/Negative - Use CHAR_NEG
 *  a-c - Use CHAR_LOWER('a')
 */
void display_write(uint8_t character);
/**
 * Same as display_write but also enables the decimal point.
 */
void display_write_dp(uint8_t character);

#endif // !_DISPLAY__H
