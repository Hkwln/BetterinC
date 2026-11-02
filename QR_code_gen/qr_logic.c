#include "qr_logic.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t version;
    char error_correction;
    uint8_t data_codewords;
    uint8_t error_correction_codewords;
    uint8_t total_codewords;
    uint8_t byte_capacity;
} QrCapacity;

static const QrCapacity qr_capacity_table[] = {
    /* version, level, data codewords, EC codewords, total codewords, byte
       capacity */
    {1, 'M', 16, 10, 26, 14},
};

BitBuf* init_BitBuf(void)
{
    BitBuf* buf = malloc(sizeof(BitBuf));
    buf->bytes = malloc(64 * sizeof(uint8_t));
    buf->bit_cap = 64;
    buf->bit_len = 0;
    return buf;
}

void resize_BitBuf(BitBuf* buf)
{
    size_t new_length = (2 * buf->bit_cap);
    buf->bytes = realloc(buf->bytes, new_length);
    if (buf->bytes == NULL) {
        fprintf(stderr, "error while reallocating buffer\n");
        return;
    }
}

void bb_push(const uint8_t* src, BitBuf* dest_buf, size_t n)
{
    if (dest_buf == NULL) {
        fprintf(stderr, "initialize dest_buf before use\n");
        return;
    }

    /* TODO: resize dest_buf if it is too small*/
    if (n > (dest_buf->bit_cap * 8 - dest_buf->bit_len)) {
        resize_BitBuf(dest_buf);
    }

    for (size_t i = 0; i < n; i++) {
        // read bit:
        bool bit = (src[i / 8] >> (7 - i % 8)) & 1;
        size_t dest_pos = dest_buf->bit_len + i;
        // set bit:
        dest_buf->bytes[dest_pos / 8] |= (uint8_t)(bit << (7 - dest_pos % 8));
    }
    dest_buf->bit_len += n;
}
