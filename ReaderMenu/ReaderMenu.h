#pragma once


#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#define IDM_MENU 107
#define IDM_OPEN 1
#define IDM_CLOSE 2
#define IDM_EXIT 3


WINBOOL getNewFileName(HWND hwnd, PSTR fileNameBuffer, int nMaxFile);
