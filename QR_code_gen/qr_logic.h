#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t* bytes;
    size_t bit_len;
    size_t bit_cap;
} BitBuf;
typedef struct {
    int version, size;    /* size = 4*version + 17 */
    uint8_t* module;      /* 0 = hell, 1 = dunkel */
    uint8_t* is_function; /* 1 = Funktionsmuster, nicht maskieren */
} Matrix;

BitBuf* init_BitBuf(void);

/* TODO: code resize buf function:*/
void resize_BitBuf(BitBuf* buf);

/*Hängt n bits an den bitbuffer*/
void bb_push(const uint8_t* src, BitBuf* dest_buf, size_t n);
