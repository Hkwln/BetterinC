#include "csprng.h"
#include "datastructure.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* TODO: pronouncable passwords;
 * how many passwords do you want to generate?
 * random password length( 8 - 30 more likely)
 */
void create_password(unsigned int length, _Bool numbers, _Bool specials,
                     char **password, CSPRNG_State *rng) {
  *password = malloc((length + 1) * sizeof(char));
  char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int charset_size = sizeof(charset) - 1;
  char numberset[] = "1234567890";
  int numberset_size = sizeof(numberset) - 1;
  char specialset[] = "@!$%&/(=?)*'-_<>|^~+#.,[]{};:\\`";
  int specialset_size = sizeof(specialset) - 1;
  size_t out_size = length * 2;
  unsigned char out[out_size];
  generate(rng, out, out_size);
  size_t idx = 0;
  for (int i = 0; i < length; i++) {
    int choice = out[idx++] % 101;
    if (numbers == true && specials == false) {
      if (choice <= 54) {
        (*password)[i] = charset[out[idx++] % charset_size];
      } else if (choice > 54 && choice <= 100) {
        (*password)[i] = numberset[out[idx++] % numberset_size];
      }
    } else if (numbers == true && specials == true) {
      if (choice <= 34) {
        (*password)[i] = charset[out[idx++] % charset_size];
      } else if (choice > 34 && choice <= 66) {
        (*password)[i] = numberset[out[idx++] % numberset_size];
      } else if (choice > 66 && choice <= 100) {
        (*password)[i] = specialset[out[idx++] % specialset_size];
      }
    } else if (numbers == false && specials == true) {
      if (choice <= 54) {
        (*password)[i] = charset[out[idx++] % charset_size];
      } else if (choice > 54 && choice <= 100) {
        (*password)[i] = specialset[out[idx++] % specialset_size];
      }
    } else {
      (*password)[i] = charset[out[idx++] % charset_size];
    }
  }
  (*password)[length] = '\0';
}

// helper functions
void add_vocale(char **password) {
  char vocale[] = "aeiou";
  int choose = rand() % 5;
  strncat(*password, &vocale[choose], 1);
}
// returns the length of the added umlaut
int add_umlaut(char **password) {
  char vo;
  _Bool length = false;
  char *together[] = {"ch", "ng", "nk", "pf", "ph", "rh", "th", "st", "sp",
                      "sk", "kn", "gn", "ts", "tz", "qu", "bl", "br", "dr",
                      "fl", "fr", "gl", "kl", "kr", "pl", "pr", "sm", "tr"};

  char charset[] = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
  int choose =
      rand() %
      69; // 79 because first 26 together + 42 charseta + 1 for the [0];
  if (choose <= 27) {
    strncat(*password, together[choose], 2);
    length = 1;
  } else {
    vo = charset[choose];
  }
  if (length) {
    return 2;
  } else {
    strncat(*password, &vo, 1);
    return 1;
  }
}

void create_pronouncable(unsigned int length, char **password) {
  *password = malloc((length + 1) * sizeof(char));
  (*password)[0] = '\0';
  // now we mesh together both functions: start random, then always switch
  // between umlaut and vocale
  int choice = rand() % 2;
  _Bool vo = false;
  if (choice == 0)
    vo = true;
  else
    vo = 0;
  int i = 0;
  do {
    if (vo) {
      i += add_umlaut(password);
      vo = false;
    } else {
      add_vocale(password);
      i++;
      vo = true;
    }
  } while (i < length);
  // TODO: i might be larger then  length
  (*password)[i] = '\0';
}

unsigned int parse_length(void) {
  unsigned int number;
  char temp[100];
  // use while instead of for
  for (int i = 0; i < 99;) {
    char b = getchar();
    if (b == '0' || b == '1' || b == '2' || b == '3' || b == '4' || b == '5' ||
        b == '6' || b == '7' || b == '8' || b == '9') {
      temp[i] = b;
      i++;
    } else if (b == ' ' || b == '\n' || b == '\0') {
      temp[i] = '\0';
      number = (unsigned int)strtol(temp, NULL, 10);
      return number;
    } else {
      number = 5 + rand() % 55;
      return number;
    }
  }
  return number;
}

struct information get_info() {
  unsigned int lenght;
  _Bool numbers = false;
  _Bool specials = false;
  char helpnumbers;
  char helpspecials;

  printf("this creates a random password for you :)\n");
  printf("what is you desired password lenth? (if empty it is random)\n");
  lenght = parse_length();
  printf("!!debug!!, length: %u\n", lenght);
  printf("do you want numbers in you password? (y/n)\n");
  scanf(" %c", &helpnumbers);
  if (helpnumbers == 'y') {
    numbers = true;
  }
  printf("do you want special characters in you password? (y/n)\n");
  scanf(" %c", &helpspecials);
  if (helpspecials == 'y') {
    specials = true;
  }

  float entropy = lenght * log2(52 + (numbers * 10) + (specials * 28));

  return (struct information){lenght, numbers, specials, entropy};
}

int main(void) {
  CSPRNG_State *rng = instantiation();
  struct information info = get_info();
  char *password = NULL;
  create_password(info.length, info.numbers, info.specials, &password, rng);
  char *p4ssword = NULL;
#if 0
  create_pronouncable(info.length, &p4ssword);
#endif
  printf("this is your password:\n%s\ndifficulty: %f\n and this is your "
         "pronouncable password: \n%s\n",
         password, info.difficulty, p4ssword);

  free(password);
  free(p4ssword);
  csprng_free(rng);
  return 0;
}
