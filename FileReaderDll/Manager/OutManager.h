#ifndef __OUTMANAGER_H__
#define __OUTMANAGER_H__

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include<stdlib.h>
#include "../Modes/HScroll.h"
#include "../Modes/Layout.h"
#include "../../Res/ReaderMenu.h"

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




#ifdef __cplusplus
    extern "C"
    {
#endif

        DRAWABLETEXT* DLL_EXPORT CreateDText();

        void DLL_EXPORT fillDText(HWND hwnd, DRAWABLETEXT* dText, char* filePath);

        void DLL_EXPORT sizeChanged(HWND hwnd, LPARAM lParam, DRAWABLETEXT* dText);

        void DLL_EXPORT redrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, DRAWABLETEXT* dText);

        void DLL_EXPORT vScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText);

        void DLL_EXPORT hScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText);

        void DLL_EXPORT precChengeMode(HWND hwnd, DRAWABLETEXT* dText);

        void DLL_EXPORT closeText(HWND hwnd, DRAWABLETEXT* dText);

        void DLL_EXPORT openNewFile(HWND hwnd, DRAWABLETEXT* dText);

#ifdef __cplusplus
    }
#endif

#endif

