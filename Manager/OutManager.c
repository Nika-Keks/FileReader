#include"OutManager.h"
#define BGC 0x00FFFFFF

/*
    the function allocates memory for the structure of the drawn text and for its members
*/
DRAWABLETEXT* CreateDText()
{
    DRAWABLETEXT* dText = (DRAWABLETEXT*)malloc(sizeof(DRAWABLETEXT));
    dText->text = (TEXT*)malloc(sizeof(TEXT));
    dText->op = (OUTPARAM*)malloc(sizeof(OUTPARAM));

    return dText;
}

/*
    input
    HWND hwnd
    char* filePat - path to the file that contains the text

    output
    DRAWABLETEXT* dText

    the function fills the fields of the structure of the text being resubmitted with default parameters
    and reads the text, if filePath not NULL
*/
void fillDText(HWND hwnd, DRAWABLETEXT* dText, char* filePath)
{
    TEXT* text = dText->text;
    OUTPARAM* op = dText->op;

    dText->OutMode = CLOSED;
    if (filePath != NULL)
    {

        FILE* file = fopen(filePath, "rb");
        if (file != NULL)
        {
            ReadText(file, text);
            fclose(file);
            dText->OutMode = HSMODE;
        }
        else
        {
            text->maxStrLen = 0;
            text->NStr = 0;
            text->NSyms = 0;
            HMENU hmenu = GetMenu(hwnd);
            EnableMenuItem(hmenu, IDM_CLOSE, MF_GRAYED);
        }
    }

    dText->clearCArea = FALSE;

    RECT rect;
    GetClientRect(hwnd, &rect);
    op->CAreaH = rect.bottom - rect.top;
    op->CAreaW = rect.right - rect.left;

    HDC hdc = GetDC(hwnd);
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    ReleaseDC(hwnd, hdc);
    op->CharH = tm.tmHeight + tm.tmExternalLeading;
    op->CharW = tm.tmMaxCharWidth;

    op->iCurStr = 0;
    op->iFirstSym = 0;
    HSProcChangeMode(op, text);
    HSSetScrollParam(hwnd, *op);
}

/*
    input
    HWND hwnd

    output
    DRAWABLETEXT* dText

    the function clears the current content of the field structure storing the text
    and sets dText->clearCArea = TRUE;
*/
void closeText(HWND hwnd, DRAWABLETEXT* dText)
{
    if (dText->OutMode == CLOSED)
        return;
    freeText(dText->text);
    dText->clearCArea = TRUE;
    dText->OutMode = CLOSED;
    InvalidateRect(hwnd, NULL, FALSE);
    EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
    HMENU hmenu = GetMenu(hwnd);
    EnableMenuItem(hmenu, IDM_CLOSE, MF_GRAYED);
}


/*
    the function clears the entire client area
*/
void clearClientArea(HWND hwnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(BGC);
    FillRect(hdc, &rect, hBrush);
}

/*
    input
    HWND hwnd
    LPARAM lParam

    output
    DRAWABLETEXT* dText

    the function changes the state of the DRAWABLETEXT* dText in accordance with the changes in the window size
*/
void sizeChanged(HWND hwnd, LPARAM lParam, DRAWABLETEXT* dText)
{
    if (dText->OutMode == HSMODE)
    {
        HSProcChangeWSize(lParam, dText->op, dText->text);
        HSSetScrollParam(hwnd, *(dText->op));
    }
    else if (dText->OutMode == LOMODE)
    {
        LOProcChangeWSize(lParam, dText->op, dText->text);
        LOSetScrollParam(hwnd, *(dText->op), *(dText->text));
        dText->clearCArea = TRUE;
    }
    InvalidateRect(hwnd, NULL, FALSE);
}

/*
    input
    HWND hwnd
    HDC hdc
    PAINTSTRUCT ps
    DRAWABLETEXT* dText

    the function redraws invalid sections of the client area
*/
void redrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, DRAWABLETEXT* dText)
{
    if(dText->clearCArea == TRUE)
        clearClientArea(hwnd, hdc);

    if (dText->OutMode == HSMODE)
    {
        HSOut(hdc, ps, *(dText->op), *(dText->text));
    }
    else if (dText->OutMode == LOMODE)
    {
        LOOut(hdc, ps, *(dText->op), *(dText->text));
    }
    else if (dText->OutMode == CLOSED)
    {
        printf("%s\n", "file hase been closed");
    }
    dText->clearCArea = FALSE;
}

/*
    input
    HWND hwnd
    WPARAM wParam

    output
    DRAWABLETEXT* dText

    the function changes the state of the DRAWABLETEXT* dText in response to the vertical scroll and sets a new position of the slider
*/
void vScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText)
{
    if (dText->OutMode == HSMODE)
    {
        HSScrollV(hwnd, wParam, dText->op);
    }
    else if (dText->OutMode ==LOMODE)
    {
        LOScrollV(hwnd, wParam, dText->op, dText->text);
    }
}

/*
    input
    HWND hwnd
    WPARAM wParam

    output
    DRAWABLETEXT* dText

    the function changes the state of the DRAWABLETEXT* dText in response to the horizontal scroll and sets a new position of the slider
*/
void hScroll(HWND hwnd, WPARAM wParam, DRAWABLETEXT* dText)
{
    if (dText->OutMode == HSMODE)
    {
        HSScrollH(hwnd, wParam, dText->op);
    }
    else if (dText->OutMode == LOMODE)
    {

    }
}

/*
    input
    HWND hwnd
    DRAWABLETEXT* dText

    output
    DRAWABLETEXT* dText

    the function changes the state of the DRAWABLETEXT* dText in response to changing the withdrawal method using its current state
*/
void precChengeMode(HWND hwnd, DRAWABLETEXT* dText)
{
    if (dText->OutMode == LOMODE)
    {
        HSProcChangeMode(dText->op, dText->text);
        HSSetScrollParam(hwnd, *(dText->op));
        dText->OutMode = HSMODE;
    }
    else if (dText->OutMode == HSMODE)
    {
        LOProcChangeMode(dText->op, dText->text);
        LOSetScrollParam(hwnd, *(dText->op), *(dText->text));
        dText->OutMode = LOMODE;
    }
    dText->clearCArea = TRUE;
    InvalidateRect(hwnd, NULL, TRUE);
}

/*
    input
    HWHWND hwnd

    output
    DRAWABLETEXT* dText

    the function, using the standard dialog, gets the path to a new file and opens it
*/
void openNewFile(HWND hwnd, DRAWABLETEXT* dText)
{
    char fileNameBuf[1024];
    if (getNewFileName(hwnd, fileNameBuf, sizeof(fileNameBuf)) == FALSE)
        return;

    if (dText->OutMode != CLOSED)
        closeText(hwnd, dText);
    fillDText(hwnd, dText, fileNameBuf);
    InvalidateRect(hwnd, NULL, TRUE);

    HMENU hmenu = GetMenu(hwnd);
    EnableMenuItem(hmenu, IDM_CLOSE, MF_ENABLED);
}





