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
