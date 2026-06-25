// here we implement putchar() function without libc :)

// parameter: char which is represented as  int in ASCII value
int syscall_putchar(int character) {
  char c = (char)character;
  long result;
  // syscall write(1, msg, len)
  asm volatile("mov $1, %%rax\n"  // syscall number for write
               "mov $1,  %%rdi\n" // file descriptor 1 == stdout
               "leaq %1, %%rsi\n" // leaq = load effective address quadword like
                                  // rsi = &character
               "mov $1, %%rdx\n"  // length = 1 byte
               "syscall\n"
               "mov %%rax, %0"
               : "=r"(result)
               : "m"(c) // input memory operant for  the character
               : "rax", "rdi", "rsi", "rdx", "memory" // clobbered register
  );
  if (result == 1) {
    return character;
  } else
    return -1;
}

// this does not work on a system with an os
static int cursor = 0;
int vga_putchar(int character) {
  volatile char *vga = (volatile char *)0xB8000; // VGA text buffer
  if (character == '\n') {
    cursor = ((cursor / 80) + 1) * 80;
  } else {
    vga[2 * cursor] = character;  // character byte
    vga[(2 * cursor) + 1] = 0x0F; // Attribute type
  }
  cursor++;
  return character;
}
// This test should work
int main(void) {
  const char *str = "this is a test for linux syscall :)\n";
  while (*str) {
    syscall_putchar(*str++);
  }

  return 1;
}
