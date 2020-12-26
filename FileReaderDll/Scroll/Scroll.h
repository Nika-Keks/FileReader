#pragma once

#include <tchar.h>
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"../OutParams/OutParams.h"

#define MAXSCROLLPOS LOWORD(0xffffffff)


/*
    input
    HWND hwnd
    WPARAM wParam
    int nBar - scrollbar flag (SB_VERT or SB_HORZ)

    output
    int - number of scrolled lines
*/
int Scroll(HWND hwnd, WPARAM wParam, int nBar);
