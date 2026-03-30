#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

// Put terminal in raw mode to read responses
struct termios set_raw_mode(void) {
  struct termios old_tio, new_tio;
  tcgetattr(STDIN_FILENO, &old_tio);
  new_tio = old_tio;
  new_tio.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
  return old_tio;
}

// Read response with timeout (in milliseconds)
int read_response(char *buf, int maxlen, int timeout_ms) {
  fd_set set;
  struct timeval timeout;
  int pos = 0;

  timeout.tv_sec = timeout_ms / 1000;
  timeout.tv_usec = (timeout_ms % 1000) * 1000;

  while (pos < maxlen - 1) {
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    int result = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
    if (result <= 0)
      break; // Timeout or error

    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
      buf[pos++] = c;
      // Most responses end with specific characters
      if (c == 'c' || c == '\\') {
        // Might be end of response, wait a bit more
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10ms
      }
    }
  }
  buf[pos] = '\0';
  return pos;
}

int main(void) {
  char response[1024];

  // Save terminal state and switch to raw mode
  struct termios old_tio = set_raw_mode();

  // Check environment variables first (fastest)
  char *term = getenv("TERM");
  char *kitty_window = getenv("KITTY_WINDOW_ID");
  char *term_program = getenv("TERM_PROGRAM");

  printf("TERM: %s\n", term ? term : "not set");
  printf("KITTY_WINDOW_ID: %s\n", kitty_window ? kitty_window : "not set");
  printf("TERM_PROGRAM: %s\n", term_program ? term_program : "not set");

  // Detect terminal emulator
  if (term && strstr(term, "kitty")) {
    printf("\nTerminal: Kitty (from $TERM)\n");
  } else if (kitty_window) {
    printf("\nTerminal: Kitty (from $KITTY_WINDOW_ID)\n");
  } else if (term && strstr(term, "foot")) {
    printf("\nTerminal: Foot (from $TERM)\n");
  }

  // Test Kitty Graphics Protocol
  printf("\n--- Testing Kitty Graphics Protocol ---\n");
  printf("\x1b_Gi=31,a=q;AAAA\x1b\\");
  fflush(stdout);

  int len = read_response(response, sizeof(response), 100); // 100ms timeout
  if (len > 0 && strstr(response, "_G")) {
    printf("Kitty Graphics: SUPPORTED\n");
  } else {
    printf("Kitty Graphics: NOT SUPPORTED (no response)\n");
  }

  // Test Sixel support (Primary Device Attributes)
  printf("\n--- Testing Sixel Support ---\n");
  printf("\x1b[c"); // Primary DA, not Secondary DA!
  fflush(stdout);

  memset(response, 0, sizeof(response));
  len = read_response(response, sizeof(response), 100);
  printf("Response length: %d\n", len);
  if (len > 0) {
    printf("Raw response bytes: ");
    for (int i = 0; i < len; i++) {
      printf("%02x ", (unsigned char)response[i]);
    }
    printf("\n");
    printf("Decoded: ");
    for (int i = 0; i < len; i++) {
      if (response[i] == '\x1b')
        printf("ESC");
      else if (response[i] >= 32 && response[i] < 127)
        printf("%c", response[i]);
      else
        printf("<%02x>", (unsigned char)response[i]);
    }
    printf("\n");
    if (strstr(response, ";4;") || strstr(response, ";4c")) {
      printf("Sixel: SUPPORTED\n");
    } else {
      printf("Sixel: NOT SUPPORTED (no ;4; in response)\n");
    }
  } else {
    printf("Sixel: UNKNOWN (no response)\n");
  }

  // Restore terminal
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

  return 0;
}
