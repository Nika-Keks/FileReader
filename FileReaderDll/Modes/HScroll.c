#include"HScroll.h"

/*
    input
    HDC hdc
    PAINTSTRUCT ps
    OUTPARAM op
    TEXT text

    the function redraws invalid areas in accordance with the "horizontal scroll" output method
*/
void HSOut(HDC hdc, PAINTSTRUCT ps, OUTPARAM op, TEXT text)
{
    unsigned int iFirstStr =  op.iCurStr + ps.rcPaint.top / op.CharH;
    unsigned int iLastStr = min (text.NStr - 1, op.iCurStr + ps.rcPaint.bottom / op.CharH);
    unsigned int iFirstSym = op.iFirstSym + ps.rcPaint.left / op.CharW;
    unsigned int iLastSym;

    int x = ps.rcPaint.left - (ps.rcPaint.left % op.CharW);
    int y = ps.rcPaint.top - (ps.rcPaint.top % op.CharH);

    for (int iStr = iFirstStr; iStr <= iLastStr; iStr++)
    {
        iLastSym = min((int)lenStr(text, iStr), (int)iFirstSym + ps.rcPaint.right / (int)op.CharW);
        int len = iLastSym - iFirstSym;

        if (len > 0)
        {
            STR curSubstr = &(text.strings[iStr][iFirstSym]);
            TextOutA(hdc, x, y, curSubstr, len);
        }
        y += op.CharH;
    }
}

/*
    input
    TEXT* text

    output
    OUTPARAM* op

    the function changes the output parameters after changing the output method
*/
void HSProcChangeMode(OUTPARAM* op, TEXT* text)
{
    op->nLines = text->NStr;
    op->nColums = text->maxStrLen;
    op->iCurStr = max(0, min((int)op->iCurStr, (int)op->nLines - (int)nLineOnPage(*op)));
}

/*
    input
    LPARAM lParam
    TEXT* text

    output
    OUTPARAM* op

    function that changes the output parameters according to the resizing of the window
*/
void HSProcChangeWSize(LPARAM lParam, OUTPARAM* op, TEXT* text)
{
    op->CAreaH = max(HIWORD(lParam), 1);
    op->CAreaW = max(LOWORD(lParam), 1);

    op->iCurStr = max(0, min(op->iCurStr, op->nLines - nLineOnPage(*op)));
    op->iFirstSym = max(0, min(op->iFirstSym, op->nColums - nSymInLine(*op)));
}

/*
    input
    HWND hwnd
    WPARAM wParam
    TEXT* text

    output
    OUTPARAM *op

    the function changes the output parameters and scrolls the window
*/
void HSScrollV(HWND hwnd, WPARAM wParam, OUTPARAM *op)
{
    if (op->nLines <= nLineOnPage(*op))
        return;

    int scrollShift = Scroll(hwnd, wParam, SB_VERT);
    if (scrollShift != 0)
    {
        ScrollWindow(hwnd, 0, -scrollShift * op->CharH, NULL, NULL);
        op->iCurStr = max(0, min(op->iCurStr + scrollShift, op->nLines - nLineOnPage(*op)));
        UpdateWindow (hwnd);
    }
}

/*
    input
    HWND hwnd
    WPARAM wParam
    TEXT* text

    output
    OUTPARAM *op

    the function changes the output parameters and scrolls the window
*/
void HSScrollH(HWND hwnd, WPARAM wParam, OUTPARAM *op)
{
    if (op->nColums <= nSymInLine(*op))
        return;

    int scrollShift = Scroll(hwnd, wParam, SB_HORZ);
    if (scrollShift != 0)
    {
        ScrollWindow(hwnd, -scrollShift * op->CharW, 0, NULL, NULL);
        op->iFirstSym = max(0, min(op->iFirstSym + scrollShift, op->nColums - nSymInLine(*op)));
        UpdateWindow (hwnd);
    }
}

/*
    input
    HWND hwnd
    OUTPARAM op

    the function sets new scrolling parameters
*/
void HSSetScrollParam(HWND hwnd, OUTPARAM op)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);

    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = nLineOnPage(op);
    si.nMax = max((op.nLines - 1), si.nPage);
    si.nPos = max(si.nMin, min(si.nMax, op.iCurStr));
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    if (op.nLines <= nLineOnPage(op))
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
    else
        EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);

    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = nSymInLine(op);
    si.nMax = max((op.nColums  - 1), si.nPage);
    si.nPos = max(si.nMin, min(si.nMax, op.iFirstSym));
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

    if (op.nColums <= nSymInLine(op))
        EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    else
        EnableScrollBar(hwnd, SB_HORZ, ESB_ENABLE_BOTH);
}
