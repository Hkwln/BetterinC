#include "csprng.h"
#include "hashing.h"
#include <stdlib.h>

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
void instantiation(void) {
  CSPRNG_State *state = malloc(sizeof(CSPRNG_State));
  // TODO: fix that
  unsigned char temp[32];
  get_seed(temp);
  // for sure the whole thing must be decoded from char to diges,right?
  SHA256(temp, 32, state->V);
  SHA256(temp + 0x80, 32, state->C);
  state->reseed_counter = 1;
#if DEBUG
  printf("!!!DEBUG!!! this is the random seed: %s\n", temp);
  printf("!!!Debug!!! this is the current random V: %s\n", state->V);
  printf("!!!DEBUG!!! this is the current random C: %s\n", state->C);
#endif
}

void reset_entropy(CSPRNG_State *state) {
  unsigned char new_entropy[32];
  get_seed(new_entropy);
  // FIXME: add the V C and the new entropy sequentially in memcpy
  SHA256(state->V || state->C || new_entropy, 32, state->V);
  state->reseed_counter = 1;
}

#if DEBUG
int main(void) {
  instantiation();
  return 0;
}
#endif
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
 * */
