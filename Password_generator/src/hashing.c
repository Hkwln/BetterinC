// SHA-256 Implementation from scratch
#include "hashing.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void int_to_bit(uint8_t* pad_bin_msg, int offset, uint64_t length);
int get_blocks(size_t message_len);
/**
 * Pads the input message according to SHA-256 specification.
 * Appends '1' bit, zeros, and 64-bit message length to make total length a
 * multiple of 512 bits .
 * binary = original message
 * pad_bin_msg output padded
 * returns padding lenght;
 * the pad_bin_msg should be the right size
 */
int pad_msg(uint8_t* pad_bin_msg, const uint8_t* binary, size_t message_len)
{
    int blocks = get_blocks(message_len);
    // if only one block is needed
    if (blocks >= 1) {
        memcpy(pad_bin_msg, binary, message_len);
        // appand 1
        pad_bin_msg[message_len] = 0x80;
        memset(pad_bin_msg + message_len + 1, 0,
               (blocks * 64) - 8 - message_len);
        int_to_bit(pad_bin_msg, (blocks * 64) - 8, message_len * 8);
        return blocks * 64;
    }
    return -1;
}

/**
 * Converts integer values to their bit representation.
 * Helper function for processing message blocks.
 */
void int_to_bit(uint8_t* pad_bin_msg, int offset, uint64_t length)
{
    for (int i = 0; i < 8; i++) {
        pad_bin_msg[offset + i] = (length >> (56 - 8 * i)) & 0xFF;
    }
}

/**
 * Calculates the number of 512-bit blocks needed after padding.
 *
 * @return Number of 512-bit blocks
 */
int get_blocks(size_t message_len)
{
    return (message_len + 9 + 63) / 64;
}

uint32_t right_rotate(const uint32_t word, int offset)
{
    uint32_t temp = word;
    offset %= 32;
    return (temp >> offset) | (temp << (32 - offset));
}

void sha_256_init(sha256_state* md)
{
    md->S[0] = 0x6a09e667;
    md->S[1] = 0xbb67ae85;
    md->S[2] = 0x3c6ef372;
    md->S[3] = 0xa54ff53a;
    md->S[4] = 0x510e527f;
    md->S[5] = 0x9b05688c;
    md->S[6] = 0x1f83d9ab;
    md->S[7] = 0x5be0cd19;
    md->buflen = md->msglen = 0;
}

static void shal_compress(sha256_state* md)
{
    uint32_t W[80], a, b, c, d, e, f, g, h, t, t0, t1;
    // unsigned x = unsigned int x;
    unsigned x;
    for (x = 0; x < 16; x++) {
        LOAD32H(W[x], md->buf + 4 * x);
    }
    /* Calc W[16...63]*/
    for (x = 16; x < 64; x++) {
        W[x] = Gamma1(W[x - 2]) + W[x - 7] + Gamma0(W[x - 15]) + W[x - 16];
    }
    // copy the status:
    a = md->S[0];
    b = md->S[1];
    c = md->S[2];
    d = md->S[3];
    e = md->S[4];
    f = md->S[5];
    g = md->S[6];
    h = md->S[7];
    // Wanna do it like this or with a makro?
    for (x = 0; x < 64; x++) {
        t0 = h + Sigma1(e) + CH(e, f, g) + K[x] + W[x];
        t1 = Sigma0(a) + MAJ(a, b, c);
        d += t0;
        h = t0 + t1;
        t = h;
        h = g;
        g = f;
        f = e;
        e = d;
        d = c;
        c = b;
        b = a;
        a = t;
    }
    // QUESTION: wie war nochmal der spaltenmodus in nvim ?
    md->S[0] += a;
    md->S[1] += b;
    md->S[2] += c;
    md->S[3] += d;
    md->S[4] += e;
    md->S[5] += f;
    md->S[6] += g;
    md->S[7] += h;
}

void sha256_process(sha256_state* md, const unsigned char* buf,
                    unsigned long len)
{
    unsigned long x, y;
    // wir kopieren 64 bytes in den buffer, und dann komprimieren wir es; buflen
    // wird zurückgesetzt
    while (len) {
        x = (64 - md->buflen) < len ? 64 - md->buflen : len;
        len -= x;
        for (y = 0; y < x; y++) {
            (md->buf[md->buflen++] = *buf++);
        }
        if (md->buflen == 64) {
            shal_compress(md);
            md->buflen = 0;
            md->msglen += 64;
        }
    }
}

void sha256_done(sha256_state* md, unsigned char* digits)
{
    // Extrahiere die 8 × 32-bit Werte im big-endian Format
    for (int i = 0; i < 8; i++) {
        uint32_t val = md->S[i];
        digits[i * 4 + 0] = (val >> 24) & 0xFF;
        digits[i * 4 + 1] = (val >> 16) & 0xFF;
        digits[i * 4 + 2] = (val >> 8) & 0xFF;
        digits[i * 4 + 3] = val & 0xFF;
    }
}

/**
 * Computes the SHA-256 hash of a message.
 *
 * @param msg - Pointer to the input message bytes
 * @param msgLen - Length of the message in bytes
 * @param digits - Output buffer (must be at least 32 bytes) to store the hash
 * @return 0 on success, non-zero on error
 */
int SHA256(const uint8_t* msg, size_t msg_len, uint8_t* digits)
{
    sha256_state md;
    sha_256_init(&md);
    // TODO: pad the message:
    size_t padded_len = get_blocks(msg_len) * 64;
    uint8_t* in = malloc(padded_len);
    pad_msg(in, msg, msg_len);
    sha256_process(&md, in, padded_len);
    sha256_done(&md, digits);
    free(in);
    return 1;
}
