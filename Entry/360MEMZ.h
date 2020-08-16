#ifndef _360MEMZ_H
#define _360MEMZ_H

#define UNICODE
#define NTDDI_VERSION NTDDI_WIN7
#define _WIN32_WINNT 0x603
//Windows APIS
#include <windows.h>
#include <dwmapi.h>
#include <uxtheme.h>
// Standard C-runtime
#include <stdio.h> 
#include <string.h>
#include <locale.h>
//For  IsUserAnAdmin()
#include <ShlObj.h>
#define IDR_360MEMZ 0x1001
void Kill();
DWORD WINAPI CreateTerminater(LPVOID lpParameter);
BOOL IsWin7OrLater();


#endif
