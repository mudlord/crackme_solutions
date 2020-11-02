#include <windows.h>

/*
This particular crackme uses Yoda's Protector for DRM.
You might need to use Scyllahide or x64dbg's Patch PEB
functions.

To unpack, load up crackme in x32dbg, place an
singleshot memory breakpoint on execution on the
"code" PE section.

Then use Scylla to rebuild imports, etc.
*/
void init() {}

void process_serial(char *name, char *serial_out) {
  char magic_buf[10] = {0};
  char tabl[] = "SJKAZBVTECGIDFNG";
  int namelen = strlen(name);
  int magic_dword = 0;

  for (int i = 0, j = 0; i < namelen; i++, j += 2)
    tabl[j] = toupper(name[i]);

  for (int i = 0; i != 0x10; i++)
    magic_dword += tabl[i];
  magic_dword = _byteswap_ulong((magic_dword * (namelen * 0xFF)) ^ 0xACEBDFAB);
  wsprintf((char *)magic_buf, "%1X", magic_dword);

  for (int i = 0; i < 8; i++) {
    byte val = magic_buf[i];
    if (val < 0x3A)
      val += 0x11;
    magic_buf[i] = val;
  }

  for (int i = 0, j = 0; i < 0x10; i += 2, j++) {
    byte val = (magic_buf[j]) + 5;
    tabl[i + 1] = val;
  }

  wsprintf(serial_out, "%s", &tabl[0]);
}
