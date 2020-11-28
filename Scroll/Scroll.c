#include"Scroll.h"


/*
    input
    WPARAM wParam - LOWORD(wParam) is scroll flag
    SCROLLINFO* si - fields are used: nPage, nTrackPos

    output
    SCROLLINFO* si - the field responsible for the position of the slider changes

    changes the position of the slider
*/
void ChangScrollPos(WPARAM wParam, SCROLLINFO* si)
{

    switch(LOWORD(wParam))
    {
    case SB_LINEUP :
        si->nPos -= 1;
        break;
    case SB_LINEDOWN :
        si->nPos += 1;
        break;
    case SB_PAGEUP :
        si->nPos -= si->nPage;
        break;
    case SB_PAGEDOWN :
        si->nPos += si->nPage;
        break;
    case SB_THUMBPOSITION :
        si->nPos = si->nTrackPos;;
        break;
    default :
        break;
    }
}

/*
    input
    HWND hwnd
    WPARAM wParam
    int nBar - scrollbar flag (SB_VERT or SB_HORZ)

    output
    int - number of scrolled lines
*/

int Scroll(HWND hwnd, WPARAM wParam, int nBar)
{

    SCROLLINFO si;
    si.cbSize = sizeof (si);
    si.fMask  = SIF_ALL;
    GetScrollInfo (hwnd, nBar, &si);
    int iPrevScrollPos = si.nPos;
    ChangScrollPos(wParam, &si);
    si.nPos = max(si.nMin, min(si.nMax - (int)si.nPage + 1, si.nPos));

    if (si.nPos != iPrevScrollPos)
    {
        SetScrollPos(hwnd, nBar, si.nPos, TRUE);
    }

    return  si.nPos - iPrevScrollPos;
}

