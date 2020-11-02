#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

void init() {}

#define shitcalc(var) {\
    regconst += serial_buf[var]; \
    regconst *= regconst; \
} \
  

void process_serial(char *name, char *serial_out) {
  BYTE serial_buf[9] = {'0', '0', '0', '0', '0', '0', '0', '0', 0x0};
  //the good boy address XORed by this.
  //a valid key is "<LT;S610"
  signed int check_dword = 0xFFFFE479 ^ 0x08EF21BD;
  // pad buf with zero in case it fails.
  bool done = false;
  int num_ser = 0;
  FILE *pFile = fopen("keys.txt", "w");
  BYTE *ser_ptr = serial_buf;
  while (!done) {
    // the bruteforce
    (*ser_ptr)++;
    if (*ser_ptr == 'Z') {
      // limit range to usable ASCII keyspace
      *ser_ptr++ = '0';
      if (*ser_ptr == 0) {
        fclose(pFile);
        wsprintf((char *)serial_out, "Nothing found.");
        return;
      } else
        continue;
    } else {
      ser_ptr = serial_buf;
      //nonsense shit in ESI. 0x00401065 times itself
      signed int regconst = 0x338cc7d9;
      shitcalc(0);
      shitcalc(1);
      shitcalc(2);
      shitcalc(3);
      shitcalc(4);
      shitcalc(5);
      shitcalc(6);
      // right regconst?
      if (regconst == check_dword) {
        fprintf(pFile, "%s\n", serial_buf);
        num_ser++;
      }
      if (num_ser == 6)
        done = true;
    }
  }
  fclose(pFile);
  wsprintf((char *)serial_out, "Serials listed in \"keys.txt\"!");
}
