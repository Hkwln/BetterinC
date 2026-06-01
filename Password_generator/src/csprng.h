#pragma once
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#define DEBUG 1
// Generel structure:
/*V = seed (entropy from OS)
counter = 0
loop:
    data = V || counter
    output = SHA256(data)
    counter++
    // if more output needed, append output to buffer
*/
typedef struct {
  unsigned char V[32]; // current value (256 bits)
  unsigned char C[32]; // constant (derived from seed)
  uint64_t reseed_counter;
} CSPRNG_State;

// HELPER FUNCTION:
// write a 32-byte seed to the given buffer
// TODO: Error handling
inline static void get_seed(unsigned char *buf) {
  int fd = open("/dev/urandom", O_RDONLY);
#if DEBUG
  printf("!!!DEBUG!!! number of bytes read: %zu\n", read(fd, buf, 32));
#endif
  close(fd);
}
// INFO: ONLY NEEDED FOR DEBUG:
//  helper function: length  of digest maximum 32 bytes
extern inline bool digest_to_hex(const uint8_t *digest, size_t dig_len,
                                 char *out, size_t out_len) {
  if (!digest || !out)
    return false;
  if (dig_len * 2 > out_len) {
    return 0;
  }
  size_t p = 0;
  for (size_t i = 0; i < dig_len; i++) {
    int dig = digest[i] >> 4;
    if (dig <= 9)
      out[p++] = (dig) + '0';
    else
      out[p++] = (dig) + 'a' - 10;

    dig = digest[i] & 0x0F;
    if (dig <= 9)
      out[p++] = (dig) + '0';
    else
      out[p++] = (dig) + 'a' - 10;
  }
  out[p] = '\0';

  return 1;
}
