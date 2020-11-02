#include <windows.h>

/*
This particular crackme is MSDOS based.
You will need DOSBox and Turbo Debugger
as well as UNP.
*/
void init() {}

void process_serial(char *name, char *serial_out) {
  int namelen = strlen(name);
  namelen--;
  int magic_dword = name[namelen - 1];
  magic_dword *= 0x40 * namelen;
  magic_dword += 0x400;
  magic_dword *= namelen;
  wsprintf((char *)serial_out, "%d", magic_dword);
}
