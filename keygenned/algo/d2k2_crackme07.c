#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#define BUFFER_SIZE 0x80

void init() {}

uint32_t crc32(const void *data, unsigned int length) {
  static const uint32_t crc32tab[16] = {
      0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 0x76DC4190, 0x6B6B51F4,
      0x4DB26158, 0x5005713C, 0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
      0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C};
  uint8_t *buf = (uint8_t *)data;
  uint32_t crc = 0xFFFFFFFF;
  uint32_t i;
  if (length == 0) {
    return 0;
  }
  for (i = 0; i < length; ++i) {
    crc ^= buf[i];
    crc = crc32tab[crc & 0x0F] ^ (crc >> 4);
    crc = crc32tab[crc & 0x0F] ^ (crc >> 4);
  }
  return crc ^ 0xFFFFFFFF;
}

void process_serial(char *name, char *serial_out) {
  uint8_t serialbuffer[16] = {0};
  uint8_t namebuf1[8] = {0};
  uint8_t namelen = lstrlen(name);
  memcpy(namebuf1, name, namelen);
  uint8_t *nameptr = namebuf1;
  uint32_t enc_key;

  {
    uint32_t crc = crc32(namebuf1, 1);
    *(uint32_t *)nameptr += crc;
    uint32_t cnt = crc;
    cnt &= 0xFFFFFF;
    cnt -= 0xF0000;
    while (cnt != 0) {
      crc = crc32(namebuf1, 1);
      *(uint32_t *)nameptr += crc;
      cnt--;
    }
    enc_key = crc;
  }

  uint8_t lut[4] = {0};
  DWORD *lutptr = lut;
  *(DWORD *)(lutptr) = _rotl(crc32(name, namelen) ^ namelen, 3);

  for (int i = 0; i < 4; i++) {
    int gen = (uint8_t)lut[i];
    gen ^= enc_key;
    gen = _byteswap_ulong(gen);
    gen = _rotl(gen, 4);
    gen += namelen;
    gen ^= enc_key;
    DWORD *bufptr = serialbuffer;
    *(DWORD *)(bufptr + i) = _byteswap_ulong(gen);
  }

  for (int i = 0; i < 16; i++) {
    wsprintf(&serial_out[i * 2], "%02X", serialbuffer[i]);
  }
}
