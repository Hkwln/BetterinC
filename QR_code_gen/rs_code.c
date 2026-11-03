/*INFO: here we implement some logic like galois field and reed solomon error
 * correcting code*/

#include "rs_code.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// size gf_exp: 255+256 + 1
uint8_t gf_exp[512], gf_log[256];

void gf_init(void)
{
    uint8_t x = 1;
    for (size_t i = 0; i < 255; i++) {
        gf_exp[i] = x;
        gf_log[x] = i;
        // calculate new x
        x = (uint8_t)((x << 1) ^ ((x & 0x80) ? 0x11D : 0));
    }
}

uint8_t gf_mul(uint8_t a, uint8_t b)
{
    // QUESTION: is this more efficient than making a switch case or if
    // statement?
    return ((a == 0) || (b == 0)) ? 0 : gf_exp[gf_log[a] + gf_log[b]];
}

uint8_t gf_div(uint8_t a, uint8_t b)
{
    return (a == 0)   ? 0
           : (b == 0) ? fprintf(stderr, "error division 0")
                      : gf_exp[gf_log[a] - gf_log[b] + 255];
}
