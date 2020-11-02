
#include "resource.h"
#include <stdio.h>
#include <windows.h>
#include "dlg_colors.c"
#include "ft2play.c"
static BOOL CALLBACK DialogFunc(HWND hwndDlg, UINT msg, WPARAM wParam,
                                LPARAM lParam);
extern "C" void process_serial(char *name, char *serial);
extern "C" void init();
char *about = "Crackme solution\n"
              "keygenned by mudlord\n";

#define MIN_NAME 3
#define MAX_NAME 0x20
#define MAX_SERIAL 255
#define BUTTON_COLOR 0x00000000
#define BUTTON_TEXT_COLOR 0x00FFFFFF
#define BUTTON_FRAME_COLOR 0x00FFFFFF

const char *keygen_name = "Crackme Keygen";
const char *default_name = "mudlord";

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpCmdLine,
                     int nCmdShow) {
  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));
  wc.lpfnWndProc = DefDlgProc;
  wc.cbWndExtra = DLGWINDOWEXTRA;
  wc.hInstance = hinst;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszClassName = "keygen";
  RegisterClass(&wc);
  return DialogBox(hinst, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL,
                   (DLGPROC)DialogFunc);
}

DWORD WINAPI GenerateSerial(HWND hwnd) {
  TCHAR name[MAX_NAME] = {0};
  unsigned char serial[MAX_SERIAL] = {0};

  if (GetDlgItemText(hwnd, IDC_NAME, name, MAX_NAME) < MIN_NAME) {
    SetDlgItemText(hwnd, IDC_SERIAL, "Please enter a longer name...");
    return 1;
  }
  process_serial((char *)name, (char *)serial);
  SetDlgItemText(hwnd, IDC_SERIAL, (char *)serial);
  SetDlgItemText(hwnd, IDC_NAME, (char *)name);
  return 0;
}

void loadmusic(int free_mus) {
#ifndef _DEBUG
  static HGLOBAL myResourceData = NULL;
  if (!free_mus) {
    HRSRC myResource = FindResource(GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDR_DATA1), RT_RCDATA);
    unsigned int myResourceSize = SizeofResource(NULL, myResource);
    HGLOBAL myResourceData = LoadResource(NULL, myResource);
    void *pMyBinaryData = LockResource(myResourceData);
    ft2play_Init(44100, 1, 1);
    ft2play_LoadModule((uint8_t *)pMyBinaryData, myResourceSize);
    ft2play_PlaySong();
    UnlockResource(pMyBinaryData);
  } else {
    ft2play_FreeSong();
    ft2play_Close();
    UnlockResource(myResourceData);
    FreeResource(myResourceData);
  }
#endif
}

static BOOL CALLBACK DialogFunc(HWND hwndDlg, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
  HBRUSH hBrush;
  HBRUSH BackBrush = CreateSolidBrush(RGB(0, 0, 0, ));
  char *data;
  switch (msg) {
  case WM_INITDIALOG:
    init();
    loadmusic(0);
    SetWindowText(hwndDlg, keygen_name);
    SetDlgItemText(hwndDlg, IDC_NAME, default_name);
    GenerateSerial(hwndDlg);
    c_MakeOwnerDraw(hwndDlg, IDC_GENERATE);
    c_MakeOwnerDraw(hwndDlg, IDC_ABOUT);
    c_MakeOwnerDraw(hwndDlg, IDC_EXIT);

    return TRUE;
    break;
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDC_GENERATE:
      GenerateSerial(hwndDlg);
      break;
    case IDC_EXIT:
      EndDialog(hwndDlg, 0);
      break;
    case IDC_ABOUT:
      MessageBox(hwndDlg, about, "About this keygen....",
                 MB_OK | MB_ICONEXCLAMATION);
    }
    break;
  case WM_CLOSE:
    loadmusic(1);
    EndDialog(hwndDlg, 0);
    return TRUE;
  case WM_DRAWITEM:
    return (c_DrawButtonColor(hwndDlg, lParam, BUTTON_COLOR, BUTTON_TEXT_COLOR,
                              BUTTON_FRAME_COLOR));
    break;
  case WM_CTLCOLORDLG:
    return (BOOL)BackBrush;
  case WM_CTLCOLORSTATIC:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLOREDIT: {
    SetTextColor((HDC)wParam, RGB(255, 255, 255));
    SetBkColor((HDC)wParam, RGB(0, 0, 0));
    hBrush = CreateSolidBrush(RGB(0, 0, 0));
    return (BOOL)hBrush;
  }
  }
  return FALSE;
}
