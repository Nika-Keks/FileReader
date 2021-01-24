#include"Layout.h"

/*
    input
    TEXT text
    OUTPARAM op
    int lineOffset - number of lines to retreat

    output
    int* iStr - index of the new current line
    STR - new substring to start drawing with

    the function calculates the index of the new line and
    a new substring with the bark will start rendering,
    provided that the new line is closer to the end of the text than the current one
*/
STR stepToEnd(TEXT text, OUTPARAM op, int lineOffset, int* iStr)
{
    *iStr = op.iCurStr;
    STR substr = op.curSubStr;
    for (int i = 0; i < lineOffset; i++)
    {
        if (lenSubstr(text, *iStr, substr) >= nSymInLine(op))
            substr += nSymInLine(op);
        else
        {
            *iStr += 1;
            if ((*iStr) >= text.NStr)
                return NULL;
            substr = text.strings[*iStr];
        }
    }
    return substr;
}

/*
    input
    TEXT text
    OUTPARAM op
    int lineOffset - number of lines to retreat

    output
    int* iStr - index of the new current line
    STR - new substring to start drawing with

    the function calculates the index of the new line and
    a new substring with the bark will start rendering,
    provided that the new line is closer to the beginning of the text than the current one
*/
STR stepToBegin(TEXT text, OUTPARAM op, int lineOffset, int* iStr)
{
    *iStr = op.iCurStr;
    STR substr = op.curSubStr;
    for (int i = 0; i > lineOffset; i--)
    {
        if (lenStr(text, *iStr) - lenSubstr(text, *iStr, substr) >= nSymInLine(op))
            substr -= nSymInLine(op);
        else
        {
            *iStr -= 1;
            if (*iStr < 0)
            {
                return NULL;
            }
            substr = text.strings[*iStr] + (lenStr(text, *iStr) / nSymInLine(op)) * nSymInLine(op);
        }
    }
    return substr;
}

/*
    input
    TEXT text
    OUTPARAM op
    int lineOffset - number of lines to retreat

    output
    int* iStr - index of the new current line
    STR - new substring to start drawing with

    the function is combination of stepToEnd(...) and stepToBegin(...)
 */
STR stepBackN(TEXT text, OUTPARAM op, int lineOffset, int* newIStr)
{
    STR newSubstr = op.curSubStr;
    *newIStr = op.iCurStr;
    if (lineOffset > 0)
        newSubstr = stepToEnd(text, op, lineOffset, newIStr);
    else if (lineOffset < 0)
        newSubstr = stepToBegin(text, op, lineOffset, newIStr);

    return newSubstr;
}

/*
    input
    LPARAM lParam
    TEXT* text

    output
    OUTPARAM* op

    function that changes the output parameters according to the resizing of the window
*/
void LOProcChangeWSize(LPARAM lParam, OUTPARAM* op, TEXT* text)
{
    int prevCAreaW = op->CAreaW;
    op->CAreaW = LOWORD(lParam);
    op->CAreaH = HIWORD(lParam);
    op->nColums = nSymInLine(*op);

    if (prevCAreaW != LOWORD(lParam))
    {
        op->nLines = getNLines(*text, op->nColums);
        op->curSubStr = text->strings[op->iCurStr];
    }

    int iStr;
    if (op->nLines <= nLineOnPage(*op))
    {
        op->curSubStr = text->strings[0];
        op->iCurStr = 0;
    }
    else if (stepBackN(*text, *op, nLineOnPage(*op), &iStr) == NULL)
    {
        STR lastStr = text->strings[text->NStr - 1];
        op->curSubStr = lastStr + (lenStr(*text, text->NStr - 1) / op->nColums) * op->nColums;
        op->iCurStr = text->NStr - 1;
        op->curSubStr = stepBackN(*text, *op, -(int)nLineOnPage(*op) + 1, &iStr);
        op->iCurStr = iStr;
    }
}


/*
    input
    TEXT* text

    output
    OUTPARAM* op

    the function changes the output parameters after changing the output method
*/
void LOProcChangeMode(OUTPARAM* op, TEXT* text)
{
    op->nColums = nSymInLine(*op);
    op->nLines = getNLines(*text, op->nColums);
    op->curSubStr = text->strings[op->iCurStr];
}


/*
    input
    HDC hdc
    PAINTSTRUCT ps
    OUTPARAM op
    TEXT text

    the function redraws invalid areas of the client area
*/
void LOOut(HDC hdc, PAINTSTRUCT ps, OUTPARAM op, TEXT text)
{
    unsigned int y = (ps.rcPaint.top / op.CAreaH) * op.CAreaH;
    unsigned int yEnd =   ps.rcPaint.bottom;

    unsigned int strOfset = y / op.CharH;
    int iCurStr;
    STR curSubstr = stepBackN(text, op, strOfset, &iCurStr);
    unsigned int lineSize = nSymInLine(op);

    while (y < yEnd )
    {
        unsigned int len = lenSubstr(text, iCurStr, curSubstr);
        unsigned int nWholeLine = len / lineSize;
        for (int i = 0; i < nWholeLine && y < yEnd; i++)
        {
            TextOut(hdc, 0, y, curSubstr, lineSize);
            y += op.CharH;
            curSubstr += lineSize;
        }

        if (len % lineSize != 0 && y < yEnd)
        {
            TextOut(hdc, 0, y, curSubstr, lenSubstr(text, iCurStr, curSubstr));
            y += op.CharH;
        }
        iCurStr++;
        if (iCurStr >= text.NStr)
            break;
        curSubstr = text.strings[iCurStr];
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
void LOScrollV(HWND hwnd, WPARAM wParam, OUTPARAM *op, TEXT* text)
{
    if (op->nLines <= nLineOnPage(*op))
        return;

    int scrollShift = Scroll(hwnd, wParam, SB_VERT);
    if (scrollShift != 0)
    {
        int iStr;
        STR curStr = stepBackN(*text, *op, scrollShift, &iStr);
        if (curStr != NULL)
        {
            ScrollWindow(hwnd, 0, -scrollShift * op->CharH, NULL, NULL);
            op->curSubStr = curStr;
            op->iCurStr = iStr;
        }
        UpdateWindow (hwnd);
    }
}


/*
    input
    HWND hwnd
    OUTPARAM op
    TEXT text

    the function sets new scrolling parameters
*/
void LOSetScrollParam(HWND hwnd, OUTPARAM op, TEXT text)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);

    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = nLineOnPage(op);
    si.nMax = max((op.nLines - 1), si.nPage);
    si.nPos = (int)nLinesBefor(text, op.nColums, op.curSubStr);

    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    if (op.nLines <= nLineOnPage(op))
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
}
