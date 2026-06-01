#include "csprng.h"
#include "hashing.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*state structure:
 * Instantiation function:
 * 1. Get entropy_seed[32 bytes] from OS
 *
2. V = SHA256(entropy_seed)
3. C = SHA256(entropy_seed + 0x80)  // simple diversification
4. reseed_counter = 1
 *reset function:
 new_entropy = get_os_entropy(32)
V = SHA256(V || C || new_entropy)
C = SHA256(C || new_entropy)
reseed_counter = 1
*/
CSPRNG_State csprng_init() {
  CSPRNG_State *state = malloc(sizeof(CSPRNG_State));
  for (int i = 0; i < 32; i++) {
    state->C[i] = 0;
    state->V[i] = 0;
  }
  state->reseed_counter = 0;
}
void csprng_free(CSPRNG_State *state) {
  state->reseed_counter = 0; // not really neaded
  state->C[0] = '\0';        // not really neaded
  state->V[0] = '\0';        // not really neaded

  free(state);
}
void instantiation(CSPRNG_State *state) {
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
}

void reset_entropy(CSPRNG_State *state) {
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

/*
 *generate function:
 while (bytes_needed > 0) {
    V = SHA256(V);
    temp = SHA256(V || C);
    output = first min(bytes_needed, 32) bytes of temp
    bytes_needed -= output_len;
    concatenate output to result;
}
reseed_counter++;
if (reseed_counter > RESEED_LIMIT) trigger reseed (error or auto)
INFO:IS RESEED_LIMIT = security strenght?
 * */
void generate(CSPRNG_State *state, size_t bytes_needed,
              size_t security_strength) // maybe add prediciton resistance
                                        // and/or addtional input
{

  while (bytes_needed > 0) {
    SHA256(state->V, 32, state->V);
    uint8_t temp[64];
    memcpy(temp, state->V, 32);
    memcpy(temp + 32, state->C, 32);
    uint8_t temp2[32];
    SHA256(temp, 64, temp2);
  }
  state->reseed_counter++;
}
#if DEBUG
int main(void) {
  CSPRNG_State random = csprng_init();
  instantiation(&random);
  reset_entropy(&random);
  return 0;
}
#endif
