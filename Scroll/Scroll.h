#pragma once

#include <tchar.h>
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"../OutParams/OutParams.h"

#define MAXSCROLLPOS LOWORD(0xffffffff)

int Scroll(HWND hwnd, WPARAM wParam, int nBar);
