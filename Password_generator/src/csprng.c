// here with the help of sha we implement true randomness!!!
//
/*V = seed (entropy from OS)
counter = 0
loop:
    data = V || counter
    output = SHA256(data)
    counter++
    // if more output needed, append output to buffer
*/

/*state structure:
 * typedef struct {
    unsigned char V[32];   // current value (256 bits)
    unsigned char C[32];   // constant (derived from seed)
    uint64_t reseed_counter;
} CSPRNG_State;
 *
 *
 *
 * Instantiation function:
 * 1. Get entropy_seed[48 bytes] from OS
2. V = SHA256(entropy_seed)
3. C = SHA256(entropy_seed + 0x80)  // simple diversification
4. reseed_counter = 1
 *reset function:
 new_entropy = get_os_entropy(32)
V = SHA256(V || C || new_entropy)
C = SHA256(C || new_entropy)
reseed_counter = 1

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
