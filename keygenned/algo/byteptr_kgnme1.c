#include <stdbool.h>
#include <wincrypt.h>
#include <windows.h>

void MD5(BYTE *data, ULONG len, BYTE *hash_data) {
  HCRYPTPROV hProv = 0;
  HCRYPTPROV hHash = 0;
  CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
  CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
  CryptHashData(hHash, data, len, 0);
  DWORD cbHash = 16;
  CryptGetHashParam(hHash, HP_HASHVAL, hash_data, &cbHash, 0);
  CryptDestroyHash(hHash);
  CryptReleaseContext(hProv, 0);
}

void init() {}

void process_serial(char *name, char *serial_out) {
  BYTE hash_bytes[16] = {0};
  int namelen = lstrlen(name);
  MD5((BYTE *)name, namelen, (BYTE *)hash_bytes);
  for (int i = 0; i < 4; i++) {
    DWORD *bufptr = hash_bytes;
    DWORD bytes = *(DWORD *)(bufptr + i);
    *(DWORD *)(bufptr + i) = _byteswap_ulong(bytes);
  }
  BYTE *bufptr = hash_bytes;
  DWORD EAX_ = *(DWORD *)(bufptr);
  DWORD EBX_ = *(DWORD *)(bufptr + 4);
  EAX_ ^= EBX_;
  EBX_ ^= 0xFBD0099;
  DWORD ECX_ = *(DWORD *)(bufptr + 8);
  ECX_ ^= EBX_;
  DWORD EDX_ = *(DWORD *)(bufptr + 0x0C);
  wsprintf(serial_out, "%.8X%.8X%.8X%.8X", EAX_, EBX_, ECX_, EDX_);
}
