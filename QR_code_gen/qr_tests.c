

#include <stdint.h>

#include "qr_logic.h"

int main(int argc, const char** argv)
{
    /* INFO: bitbuf test:*/
    BitBuf* buf = init_BitBuf();
    uint8_t data[] = {0xB0};
    bb_push(data, buf, 4);
    printf(" %x , length: %zu\n", buf->bytes[0], buf->bit_len);
    uint8_t data2 = 0xF0;
    bb_push(&data2, buf, 4);
    printf(" %x , length: %zu\n", buf->bytes[0], buf->bit_len);
    uint8_t data3 = 0x0F;
    bb_push(&data3, buf, 8);
    printf(" %x , length: %zu\n", buf->bytes[1], buf->bit_len);
    uint8_t boundary = 0xB0;
    bb_push(&boundary, buf, 5);
    uint8_t boundary2 = 0x68;
    bb_push(&boundary2, buf, 5);
    printf(" bytes[2] %x  bytes[3] %x, length: %zu\n", buf->bytes[2],
           buf->bytes[3], buf->bit_len);

    return 0;
}
