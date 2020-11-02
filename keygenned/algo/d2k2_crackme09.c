// https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher
#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

const BYTE validchar_tbl[] =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
#define validchar_tbllen 62

void init() {}

void process_serial(char *name, char *serial_out) {
  int seriallen = strlen(name);
  char *ciphertext = (char *)malloc(seriallen + 1);
  memset(ciphertext, 0, seriallen + 1);
  int shlvar = (seriallen << 2) >= 0x3C ? 0x1E : seriallen << 2;
  unsigned char subtable1[validchar_tbllen] = {0};
  for (int i = 0; i < validchar_tbllen; i++)
    subtable1[i] = validchar_tbl[(shlvar++ % validchar_tbllen)];
  uint8_t AL = 0, DL = 0;
  DL = _rotl8(name[0], 3);
  for (int i = 0;; i++) {
    unsigned char ciphertable[validchar_tbllen] = {0};
    int j = i + 1;
    AL = (name[i] ^ name[j]) + DL;
    DL += AL;
    AL = subtable1[AL % validchar_tbllen];
    int offset = 0;
    for (int k = 0; k < validchar_tbllen; k++)
      if (subtable1[k] == AL)
        offset = k;
    for (int k = 0; k < validchar_tbllen; k++)
      ciphertable[k] = subtable1[offset++ % validchar_tbllen];
    for (int k = 0; k < validchar_tbllen; k++)
      if (subtable1[k] == name[i])
        ciphertext[i] = ciphertable[k];
    if (j == seriallen)
      break;
  }
  wsprintf(serial_out, "%s", ciphertext);
  free(ciphertext);
}