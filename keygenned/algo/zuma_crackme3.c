#include <stdbool.h>
#include <windows.h>
#include <stdint.h>

extern void _stdcall mars_setkey(DWORD* input, DWORD input_len);
extern void _stdcall mars_encrypt(DWORD* input, DWORD* output);

void init() {}

void process_serial(char *name, char *serial_out) {
  unsigned namelen = lstrlen(name);
  uint8_t* mars_key = "FEABCBFFFF183461";
  mars_setkey(mars_key, strlen(mars_key));
  //pad out blocksize to match block coming out, sym crypt 101
  uint8_t namestr[0x10] = {0};
  uint8_t mars_out[0x20] = { 0 };
  uint8_t mars_tbl[0x50] = { 0 };

  uint8_t magic[0x6] = { 0 };
  unsigned namesum = 0;
  for (unsigned i = 0; i < namelen; i++)
      namesum += name[i];
  wsprintf(namestr, "%lX", namesum);

  mars_encrypt(namestr, mars_out);

  uint32_t* mars_ptr = mars_out;
  uint8_t* marsstrptr = mars_tbl;
  marsstrptr += 6;
  for (unsigned i = 0; i < 0x20; i += 4)
  {
      wsprintf(&marsstrptr[i*2], "%d", *mars_ptr);
      mars_ptr++;
  }

  unsigned marstbloutlen = strlen(marsstrptr);
  magic[0] = mars_tbl[5 + marstbloutlen];
  magic[1] = mars_tbl[4 + marstbloutlen];
  magic[2] = mars_tbl[3 + marstbloutlen];
  magic[3] = mars_tbl[1 + marstbloutlen];
  magic[4] = mars_tbl[marstbloutlen];
  wsprintf(serial_out, "%s-55600",magic);
}
