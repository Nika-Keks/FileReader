#include "ReaderMenu.h"

/*
    input
    HWND hwnd
    int sizeOFNBuf - buffer size

    output
    PSTR OFNBuffer - buffer for the path to the new file
    WINBOOL - TRUE if the user selected a new file
              FALSE if the user pressed "cancel" or chose a path that does not fit in the buffer

*/
WINBOOL getNewFileName(HWND hwnd, PSTR OFNBuffer, int sizeOFNBuf)
{
    OPENFILENAME ofn;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = "Text\0*.TXT\0";
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = OFNBuffer;
    ofn.nMaxFile = sizeOFNBuf;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "txt";
    ofn.lCustData = 0L;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

        return GetOpenFileName(&ofn);
}

