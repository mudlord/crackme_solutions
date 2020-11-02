#include <stdint.h>
#include <windows.h>
extern void _stdcall d2k2_crackme06_unknown128bhash(DWORD *input,
                                                    DWORD input_len,
                                                    DWORD *output);
extern void _stdcall d2k2_crackme06_unknown64bhash(DWORD *output,
                                                   DWORD input_len,
                                                   DWORD *input);
extern void _stdcall d2k2_crackme06_haval(DWORD *output, DWORD input_len,
                                          DWORD *input);
extern int _stdcall d2k2_crackme06_crc32(DWORD input_len, DWORD *input);
unsigned int d2k2_magic1; // variable needed for unknown hash function.
unsigned int d2k2_magic2; // variable needed for CRC32 checksum.

typedef union {
  uint64_t rx;
  uint32_t ex;
  uint16_t x;
  struct {
    uint8_t lo, hi;
  } b;
} Register;

#define BUFFER_SIZE 0x80

void init() {}

char base36enc(int num) {
  // pick a char from the table according to the input hex number
  const char *base36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  return (base36[num]);
}

void process_serial(char *name, char *serial_out) {
  Register EB, EC, EA;
  unsigned char havalbuf1[BUFFER_SIZE] = {0};
  unsigned char havalbuf2[BUFFER_SIZE] = {0};
  char d2k2_testtable[0x130] = {0};
  unsigned char d2k2_crackme06_serial[0x11] = {0};
  unsigned char inputbuf1[0x40] = {0};

  size_t namelen = strlen(name);
  memcpy(inputbuf1, name, namelen);
  d2k2_magic1 = 0xABCDEFAC;

  DWORD d2k2_magicarray[2] = {0xC3F801AF, 0};

  EC.ex = 0;
  EB.ex = 0;
  EA.ex = namelen;
  // first portion

  for (int i = 0; i < namelen; i++) {
    BYTE *d2k2_unk1hashptr = havalbuf1;
    d2k2_crackme06_unknown128bhash(inputbuf1, namelen, havalbuf1);
    EB.b.lo = (inputbuf1[i] + i) ^ namelen;
    havalbuf1[i] += EB.b.lo;
    EB.ex = *(DWORD *)(d2k2_unk1hashptr + 0x04);
    EA.ex =
        (*(DWORD *)(d2k2_unk1hashptr + 0x05) + EB.ex + 0x2004) + i + 0x4064C7;
    d2k2_magic1 = EA.ex;
    EB.b.lo ^= EA.b.lo;
    inputbuf1[i] += EB.b.lo;
  }
  d2k2_crackme06_haval(havalbuf2, namelen, inputbuf1);

  // second portion
  EA.ex = 0x004063AD;
  DWORD *d2k2_testtblptr = d2k2_testtable;
  int test = 0;
  for (int i = 0; i < 0x10; i++) {

    EB.ex = (int8_t)havalbuf1[i] + d2k2_magic1;
    EB.ex ^= (int8_t)havalbuf2[i];
    EB.ex = (EB.ex * 4) + 0x1024;
    *(DWORD *)(d2k2_testtblptr++) = EB.ex;
    EB.ex += d2k2_magicarray[0];
    EB.ex ^= EA.ex;
    EB.ex ^= i;
    *(DWORD *)(d2k2_testtblptr++) = EB.ex;
    EB.ex += d2k2_magic1;
    EB.ex ^= EA.ex;
    EB.ex ^= i;
    *(DWORD *)(d2k2_testtblptr++) = EB.ex;
    EB.ex += d2k2_magicarray[0];
    EB.ex ^= EA.ex;
    EB.ex ^= i;
    *(DWORD *)(d2k2_testtblptr++) = EB.ex;
    d2k2_magic1 = EB.ex;
    havalbuf1[i] = EB.b.lo;

    d2k2_crackme06_haval(havalbuf1, 0x10, havalbuf2);
    d2k2_crackme06_unknown128bhash(havalbuf1, 0x10, havalbuf2);
    d2k2_crackme06_unknown64bhash(d2k2_magicarray, 0x10, havalbuf2);
    d2k2_magic2 = d2k2_magicarray[0];
    havalbuf2[i] = LOBYTE(d2k2_crackme06_crc32(0x10, havalbuf2));
    EA.ex += 0x10;
  }

  for (int i = 0; i < 0x10; i++)
    d2k2_crackme06_serial[i] = base36enc(rand() % 36);

  char str[] = "FU";
  d2k2_testtable[0] ^= d2k2_crackme06_serial[0];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0]);
  d2k2_crackme06_serial[0x02] = str[1];

  d2k2_testtable[0x57] ^= d2k2_crackme06_serial[0x06];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x57]);
  d2k2_crackme06_serial[0x01] = str[1];

  d2k2_testtable[0x38] ^= d2k2_crackme06_serial[0x0D];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x38]);
  d2k2_crackme06_serial[0x04] = str[1];

  d2k2_testtable[0x70] ^= d2k2_crackme06_serial[0x09];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x70]);
  d2k2_crackme06_serial[0x07] = str[1];

  d2k2_testtable[0xCB] ^= d2k2_crackme06_serial[0x0F];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0xCB]);
  d2k2_crackme06_serial[0x08] = str[1];

  d2k2_testtable[0x58] ^= d2k2_crackme06_serial[0x05];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x58]);
  d2k2_crackme06_serial[0x0A] = str[1];

  d2k2_testtable[0x2A] ^= d2k2_crackme06_serial[0x03];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x2A]);
  d2k2_crackme06_serial[0x0B] = str[1];

  d2k2_testtable[0x9C] ^= d2k2_crackme06_serial[0x0C];
  wsprintf(str, "%2.2X", (BYTE)d2k2_testtable[0x9C]);
  d2k2_crackme06_serial[0x0E] = str[1];

  wsprintf(serial_out, "%s", d2k2_crackme06_serial);
}
