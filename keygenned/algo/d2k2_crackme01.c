#include <windows.h>
void init() {}

void process_serial(char *name, char *serial_out) {
  unsigned char buffer[32] = {0};

  for (int i = 5, j = 0; i != 0; i--, j++) {
    byte ascii_val = (name[j] ^ 0x29) + i;
    buffer[j] = (ascii_val < 'A' || ascii_val > 'Z') ? 0x52 + i : ascii_val;
    byte ascii_val2 = (name[j] ^ 0x27) + i;
    ascii_val2++;
    buffer[j + 5] =
        (ascii_val2 < 'A' || ascii_val2 > 'Z') ? 0x4D + i : ascii_val2;
  }

  int ctr = 0;
  do {
    byte gen = (buffer[ctr]) + 5;
    if (gen > 'Z')
      gen -= 0xD;
    gen ^= 0xC;
    if (gen < 'A')
      gen = 0x4B + ctr;
    if (gen > 'Z')
      gen = 0x4B - ctr;
    buffer[ctr] = gen;
    ctr++;
  } while (buffer[ctr]);

  wsprintf(serial_out, "%s", buffer);
}
