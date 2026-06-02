#include "csprng.h"
#include "hashing.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void csprng_free(CSPRNG_State *state) {
  state->reseed_counter = 0; // not really neaded
  state->C[0] = '\0';        // not really neaded
  state->V[0] = '\0';        // not really neaded

  free(state);
}
CSPRNG_State *instantiation(void) {
  CSPRNG_State *state = malloc(sizeof(CSPRNG_State));
  for (int i = 0; i < 32; i++) {
    state->C[i] = 0;
    state->V[i] = 0;
  }
  state->reseed_counter = 0;
  unsigned char temp[32];
  get_seed(temp);
  // for sure the whole thing must be decoded from char to diges,right?
  SHA256(temp, 32, state->V);
  SHA256(temp + 0x80, 32, state->C);
  state->reseed_counter = 1;
#if DEBUG
  int out_len = 1000;
  char *out = malloc(out_len);
  char *out2 = malloc(out_len);
  digest_to_hex(state->V, 32, out, out_len);
  digest_to_hex(state->C, 32, out2, out_len);
  printf("!!!Debug!!! this is the current random V: %s\n", out);
  printf("!!!DEBUG!!! this is the current random C: %s\n", out2);
#endif
  return state;
}

void reseed_entropy(CSPRNG_State *state) {
  unsigned char new_entropy[32];
  get_seed(new_entropy);

  uint8_t temp[128]; // neaded: 32 * 3 = 96; made it a bit bigger
  memcpy(temp, state->V, 32);
  memcpy(temp + 32, state->C, 32);
  memcpy(temp + 64, new_entropy, 32);

  SHA256(temp, 96, state->V);

  uint8_t temp1[96];
  memcpy(temp1, state->C, 32);
  memcpy(temp1 + 32, new_entropy, 32);
  SHA256(temp1, 64, state->C);

  state->reseed_counter = 1;
#if DEBUG
  int out_len = 1000;
  char *out = malloc(out_len);
  char *out2 = malloc(out_len);
  digest_to_hex(state->V, 32, out, out_len);
  digest_to_hex(state->C, 32, out2, out_len);
  printf("!!!Debug!!! this is the current random V: %s\n", out);
  printf("!!!DEBUG!!! this is the current random C: %s\n", out2);
#endif
}

void generate(CSPRNG_State *state, unsigned char *out, size_t bytes_needed) {
  // ERROR handling
  while (bytes_needed > 0) {
    SHA256(state->V, 32, state->V);
    uint8_t temp2[32];
    uint8_t temp[64];
    memcpy(temp, state->V, 32);
    memcpy(temp + 32, state->C, 32);
    SHA256(temp, 64, temp2);
    // copy bytes to output
    size_t copy = bytes_needed < 32 ? bytes_needed : 32;
    memcpy(out, temp2, copy);
    out += copy;
    bytes_needed -= copy;
  }
  state->reseed_counter++;
}
#if DEBUG
int main(int argc, char **argv) {
  CSPRNG_State random = instantiation();
  reseed_entropy(&random);
  unsigned char ra[16];
  generate(&random, ra, 16);
  printf("the first 8 byte number = %d\n", ra[1]);
  char *out = malloc(1000);
  digest_to_hex(ra, 16, out, 1000);
  printf("this is the random generated number: %s\n", out);
  return 0;
}
#endif
