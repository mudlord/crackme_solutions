#include <windows.h>
extern void _stdcall d2k2_crackme05_hash(DWORD *input, DWORD input_len,
                                         DWORD *output);

#define BUFFER_SIZE 0x40
void init() {}

void process_serial(char *name, char *serial_out) {
  unsigned char hashinp[BUFFER_SIZE] = {0};
  unsigned char d2k2_hashout[BUFFER_SIZE] = {0};
  unsigned char d2k2hash_formatted[BUFFER_SIZE] = {0};
  DWORD *hashbuf_ptr = (DWORD *)d2k2_hashout;
  int namelen = strlen(name);
  lstrcpy(hashinp, name);

  for (int ctr = 0; ctr < namelen; ctr++)
    hashinp[ctr] = LOBYTE(*(DWORD *)hashinp ^ (hashinp[ctr] + namelen));

  for (int ctr = 0; ctr != namelen; ctr++) {
    BYTE *hashbuf_ptr_b = (BYTE *)d2k2_hashout;
    int magic1 =
        ((((DWORD)0x10101010 & 0xFFFFFF00) | (DWORD)hashinp[ctr] & 0xFF) << 5) *
        namelen;
    int magic2 = ((0x68F6B76C * namelen) ^ magic1) + *(DWORD *)hashinp;
    *(DWORD *)hashinp = magic2;
    d2k2_crackme05_hash(hashinp, namelen, d2k2_hashout);
    hashbuf_ptr_b += 6;
    magic2 = _rotl(magic2 ^ (*(DWORD *)hashbuf_ptr_b), 7);
    hashbuf_ptr_b += 2;
    magic2 ^= (*(DWORD *)hashbuf_ptr_b);
    hashinp[ctr] = LOBYTE(magic2);
    d2k2_crackme05_hash(hashinp, namelen, d2k2_hashout);
    hashbuf_ptr_b += 2;
    magic2 = _rotl(magic2 ^ (*(DWORD *)hashbuf_ptr_b), 4);
    hashinp[ctr] = LOBYTE(magic2);
    d2k2_crackme05_hash(hashinp, namelen, d2k2_hashout);
  }

  for (int i = 0; i < 4; i++) {
    unsigned char text[9] = {0};
    wsprintf(text, "%1X", ((DWORD *)hashbuf_ptr[i]));
    strcat(d2k2hash_formatted, text);
  }
  int ebx = 0x10101010;
  int edx = 9;
  for (int i = 0; i < 0x20; i++) {
    ebx = _rotl(((DWORD)ebx & 0xFFFFFF00) | (DWORD)d2k2hash_formatted[i] & 0xFF,
                0x10);
    ebx = _byteswap_ulong(_byteswap_ulong(ebx) + 0x1A2B3C4D) ^ edx;
    int eax = ebx;
    edx = (eax >= 0) ? 0 : 0xFFFFFFFF;
    ebx = 0x19;
    edx = (eax % ebx) + 0x41;
    d2k2hash_formatted[i] = edx;
  }
  wsprintf(serial_out, "%s", d2k2hash_formatted);
}
