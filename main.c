#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<stdio.h>

#include"Manager/OutManager.h"
#include "ReaderMenu/ReaderMenu.h"
#include "ReaderMenu/ReaderMenu.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS | CS_VREDRAW | CS_CLASSDC;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = (LPCSTR)MAKEINTRESOURCEW(IDM_MENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* ClassName */
           _T("Code::Blocks Template Windows App"),
           WS_OVERLAPPEDWINDOW  |
           WS_HSCROLL | WS_VSCROLL ,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           1000,
           600,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           lpszArgument
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DRAWABLETEXT* dText = NULL;

    PAINTSTRUCT ps;
    HDC hdc;
    CREATESTRUCT* tmp;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            // preparation of drawn text
            tmp = (CREATESTRUCT*)lParam;
            dText = CreateDText();
            fillDText(hwnd, dText, (char *const)(tmp->lpCreateParams));
            break;

        case WM_SIZE:
            sizeChanged(hwnd, lParam, dText);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            redrawText(hwnd, hdc, ps, dText);
            EndPaint(hwnd, &ps);
            break;

        case WM_VSCROLL :
            vScroll(hwnd, wParam, dText);
            break;

        case WM_HSCROLL :
            hScroll(hwnd, wParam, dText);
            break;

        case WM_KEYDOWN:
            switch(LOWORD(wParam))
            {
            case VK_LEFT:
                hScroll(hwnd, SB_LINELEFT, dText);
                break;

            case VK_RIGHT:
                hScroll(hwnd, SB_LINERIGHT, dText);
                break;

            case VK_UP:
                vScroll(hwnd, SB_LINEUP, dText);
                break;

            case VK_DOWN:
                vScroll(hwnd, SB_LINEDOWN, dText);
                break;

            case VK_F3:
                precChengeMode(hwnd, dText);
                InvalidateRect(hwnd, NULL, FALSE);
                break;
            }
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
            case IDM_OPEN:
                openNewFile(hwnd, dText);
                break;
            case IDM_CLOSE:
                closeText(hwnd, dText);
                break;
            case IDM_EXIT:
                SendMessage(hwnd, WM_CLOSE, 0, 0L);
                break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



