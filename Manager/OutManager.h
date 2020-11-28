#pragma once
#include<stdlib.h>

#include "../Modes/HScroll.h"
#include "../Modes/Layout.h"
#include "../ReaderMenu/ReaderMenu.h"

#define CLOSED 1
#define HSMODE 2
#define LOMODE 3

typedef struct DRAWABLETEXT_t
{
    OUTPARAM* op; // text output options
    TEXT* text;   // structure containing text
    int OutMode;  // display mode flag
    WINBOOL clearCArea; // flag indicating the need to clear the window in the next message handler WM_PAINT
}DRAWABLETEXT;


DRAWABLETEXT* CreateDText();


void fillDText(HWND hwnd, DRAWABLETEXT* dText, char* filePath);

void sizeChanged(HWND hwnd, LPARAM lParam, DRAWABLETEXT* dText);

void redrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, DRAWABLETEXT* dText);

void vScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText);

void hScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText);

void precChengeMode(HWND hwnd, DRAWABLETEXT* dText);


void closeText(HWND hwnd, DRAWABLETEXT* dText);

void openNewFile(HWND hwnd, DRAWABLETEXT* dText);




