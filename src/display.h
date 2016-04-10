#ifndef _DISPLAY__H
#define _DISPLAY__H

#include "defines.h"

#define CHAR_DP     10
#define CHAR_SPACE  11
#define CHAR_NEG    12
#define CHAR_A      13
#define CHAR_LOWER(c) (c - 'a' + CHAR_A)
#define CHAR_UPPER(c) (c - 'A' + CHAR_A)

void display_init(void);
void display_update(void);

void display_clear(void);
void display_write(uint8_t character);
void display_write_dp(uint8_t character);

#endif // !_DISPLAY__H
