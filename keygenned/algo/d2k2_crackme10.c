#include <ctype.h>
#include <intrin.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <wincrypt.h>
#include <windows.h>

static const char base64digits[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define BAD -1

static const char base64val[] = {
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, 62,  BAD,
    BAD, BAD, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  BAD, BAD,
    BAD, BAD, BAD, BAD, BAD, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
    10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
    25,  BAD, BAD, BAD, BAD, BAD, BAD, 26,  27,  28,  29,  30,  31,  32,  33,
    34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  BAD, BAD, BAD, BAD, BAD};

#define DECODE64(c) (isascii(c) ? base64val[c] : BAD)

/* Encode into base64 */
void base64_encode(unsigned char *out, const unsigned char *in, int inlen) {
  /* raw bytes in quasi-big-endian order to base 64 string (NUL-terminated) */
  for (; inlen >= 3; inlen -= 3) {
    *out++ = base64digits[in[0] >> 2];
    *out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
    *out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
    *out++ = base64digits[in[2] & 0x3f];
    in += 3;
  }

  if (inlen > 0) {
    unsigned char fragment;

    *out++ = base64digits[in[0] >> 2];
    fragment = (in[0] << 4) & 0x30;
    if (inlen > 1)
      fragment |= in[1] >> 4;
    *out++ = base64digits[fragment];
    *out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
    *out++ = '=';
  }
  *out = '\0';
}

/*
 * Decode from base64.
 *
 * base 64 to raw bytes in quasi-big-endian order, returning count of bytes
 * maxlen limits output buffer size, set to zero to ignore.
 */
int base64_decode(unsigned char *out, const unsigned char *in, int maxlen) {
  register unsigned char digit1, digit2, digit3, digit4;
  int len = 0;

  if (in[0] == '+' && in[1] == ' ')
    in += 2;
  if (*in == '\r')
    return (0);

  do {
    digit1 = in[0];
    if (DECODE64(digit1) == BAD)
      return (-1);
    digit2 = in[1];
    if (DECODE64(digit2) == BAD)
      return (-1);
    digit3 = in[2];
    if (digit3 != '=' && DECODE64(digit3) == BAD)
      return (-1);
    digit4 = in[3];
    if (digit4 != '=' && DECODE64(digit4) == BAD)
      return (-1);
    in += 4;
    ++len;

    if (maxlen && len > maxlen)
      return (-1);

    *out++ = (DECODE64(digit1) << 2) | (DECODE64(digit2) >> 4);
    if (digit3 != '=') {
      ++len;
      if (maxlen && (len > maxlen))
        return (-1);

      *out++ = ((DECODE64(digit2) << 4) & 0xf0) | (DECODE64(digit3) >> 2);
      if (digit4 != '=') {
        ++len;
        if (maxlen && (len > maxlen))
          return (-1);
        *out++ = ((DECODE64(digit3) << 6) & 0xc0) | DECODE64(digit4);
      }
    }
  } while (*in && (*in != '\r') && (digit4 != '='));

  return (len);
}

typedef union {
  uint64_t rx;
  uint32_t ex;
  uint16_t x;
  struct {
    uint8_t lo, hi;
  } b;
} Register;

void init() {}

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

#define DIM 4
#define SIZE (DIM * DIM)
#define EMPTY 0x10

BYTE soduku[16][16] = {0x10};

int sodu_avail(int x, int y, unsigned char z) {
  int i, j, bi, bj;

  // number is already in this cell
  if (soduku[x][y] == z)
    return 1;

  // cell is empty
  if (soduku[x][y] != EMPTY)
    return 0;

  // check row and col
  for (i = 0; i < SIZE; i++) {
    if (soduku[i][y] == z)
      return 0;
    if (soduku[x][i] == z)
      return 0;
  }

  // check the box
  bi = x / DIM;
  bj = y / DIM;
  for (i = bi * DIM; i < (bi + 1) * DIM; i++)
    for (j = bj * DIM; j < (bj + 1) * DIM; j++)
      if (soduku[i][j] == z)
        return 0;

  // all checks successful passed
  return 1;
}

void sodu_solve(int x, int y, int *f) {
  int n, t;

  if (*f == 1)
    return; // if finished -> go out
  if (x == SIZE)
    *f = 1; // if board filled -> set finish
  else
    for (n = 0; n < SIZE; n++)
      if (sodu_avail(x, y, n)) {
        /* Try n at row, col */
        t = soduku[x][y];
        soduku[x][y] = n;

        /* Move on to next square */
        if (y == SIZE - 1) {
          sodu_solve(x + 1, 0, f);
          if (*f == 1)
            return;
        } else {
          sodu_solve(x, y + 1, f);
          if (*f == 1)
            return;
        }
        /* Undo n at row, col (backtrack) */
        soduku[x][y] = t;
      }
}

void process_serial(char *name, char *serial_out) {
  BYTE md5_hash[16] = {0};
  BYTE buf1[16] = {0};
  BYTE buf2[16] = {0};

  unsigned char name2[50] = {0};
  strcpy(name2, name);
  strcat(name2, "-diablo2oo2");
  int namelen = lstrlen(name2);
  MD5((BYTE *)name2, namelen, (BYTE *)md5_hash);

  for (int i = 0; i < 16; i++)
    buf1[i] = i;
  // 1234567890ABCDEF
  // 0040153B - alters MD5 hash.
  for (int k = 0; k < 16; k++) {
    for (int i = 0; i < 16; i++) {
      Register EAX, EDX, EBX;
      EAX.ex = md5_hash[i];
      EDX = EAX;
      EAX.b.lo &= 0xF;
      EDX.b.lo >>= 4;
      EBX.b.lo = buf1[EAX.ex];
      buf1[EAX.ex] = buf1[EDX.ex];
      buf1[EDX.ex] = EBX.b.lo;
      md5_hash[i] += EBX.b.lo;
      md5_hash[i] ^= 0x17;
    }
  }

  for (int i = 0, k = 0; i < 8; i++, k += 2) {
    int j = buf1[k] << 4;
    int l = buf1[k + 1] & 0x0F;
    buf2[i] = j + l;
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++)
      soduku[i][j] = EMPTY;
  }
  for (int i = 0; i < 16; i++)
    soduku[i][i] = buf1[i];
  int flag = 0;
  sodu_solve(0, 0, &flag);

  BYTE b64encstr[128] = {0};
  unsigned char *in = soduku;
  unsigned char *out = b64encstr;
  for (int i = 0, k = 0; i < sizeof(b64encstr); i++, k += 2) {
    int j = in[k] << 4;
    int l = in[k + 1] & 0x0F;
    out[i] = j + l;
  }

  int64_t *base64_ptr = b64encstr;
  __m128i buf2_mask_mmx = _mm_loadl_epi64(buf2);
  for (int i = 0; i < 16; i++) {
    __m128i var1_mmx = _mm_loadl_epi64(base64_ptr);
    var1_mmx = _mm_xor_si128(buf2_mask_mmx, var1_mmx);
    _mm_storeu_si64(base64_ptr, var1_mmx);
    base64_ptr++;
  }

  BYTE b64encstr2[256] = {0};
  base64_encode(b64encstr2, b64encstr, 0x80);
  wsprintf(serial_out, "%s", b64encstr2);
}
