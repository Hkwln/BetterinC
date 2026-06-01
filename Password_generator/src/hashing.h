#pragma once
// special thanks to my library without this implementation is not possible :)
#include <stddef.h>
#include <stdint.h>
#define SHA256_DIGEST_LENGTH 32
//clang-format off
#define LOAD32H(x, y)                                                          \
  (x) = ((uint32_t)(y)[0] << 24) | ((uint32_t)(y)[1] << 16) |                  \
        ((uint32_t)(y)[2] << 8) | (uint32_t)(y)[3]

#define ROR(x, y)                                                              \
  ((((uint32_t)(x) >> (uint32_t)((y) & 31)) |                                  \
    (((uint32_t)(x) & 0xFFFFFFFFUL) << (uint32_t)(32 - ((y) & 31)))) &         \
   0xFFFFFFFFUL)
//clang-format on
//
// non-linear:
#define CH(x, y, z) (z ^ (x & (y ^ z)))
#define MAJ(x, y, z) (((x | y) & z) | (x & y))
// zyklische bzw. logische rechtsverschiebung
#define S(x, n) ROR((x), (n))
#define R(x, n) (((x) & 0xFFFFFFFFUL) >> (n))

// fördern der diffusion
#define Sigma0(x) (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x) (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x) (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x) (S(x, 17) ^ S(x, 19) ^ R(x, 10))

typedef struct {
  unsigned char buf[64];
  unsigned long buflen, msglen;
  uint32_t S[8];
} sha256_state;

static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/**
 * Computes the SHA-256 hash of a message.
 *
 * @param msg - Pointer to the input message bytes
 * @param msgLen - Length of the message in bytes
 * @param digits - Output buffer (must be at least 32 bytes) to store the hash
 * @return 0 on success, non-zero on error
 */
int SHA256(const uint8_t *msg, size_t msgLen, uint8_t *digits);
