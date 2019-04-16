#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "impl.h"

#define ID_EDITCHILD 100

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static LRESULT CALLBACK luaConsoleWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndEdit;

    CHAR lpszLatin[] = "Lorem ipsum dolor sit amet, consectetur "
        "adipisicing elit, sed do eiusmod tempor "
        "incididunt ut labore et dolore magna "
        "aliqua. Ut enim ad minim veniam, quis "
        "nostrud exercitation ullamco laboris nisi "
        "ut aliquip ex ea commodo consequat. Duis "
        "aute irure dolor in reprehenderit in "
        "voluptate velit esse cillum dolore eu "
        "fugiat nulla pariatur. Excepteur sint "
        "occaecat cupidatat non proident, sunt "
        "in culpa qui officia deserunt mollit "
        "anim id est laborum.";

    switch (message)
    {
    case WM_CREATE:
        hwndEdit = CreateWindowExA(
            0,
            "EDIT",
            NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            0, 0, 0, 0,
            hwnd,
            (HMENU)ID_EDITCHILD,
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);

        SendMessageA(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);

        return 0;

    case WM_COMMAND:
        return DefWindowProc(hwnd, message, wParam, lParam);

    case WM_SETFOCUS:
        SetFocus(hwndEdit);
        return 0;

    case WM_SIZE:
        // Make the edit control the size of the window's client area. 

        MoveWindow(hwndEdit,
            0, 0,                  // starting x- and y-coordinates 
            LOWORD(lParam),        // width of client area 
            HIWORD(lParam),        // height of client area 
            TRUE);                 // repaint window 
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return NULL;
}

SISTERRAY_API void srInitLuaConsole(void)
{
    WNDCLASSA luaConsoleClass;

    luaConsoleClass.style = 0;
    luaConsoleClass.lpfnWndProc = luaConsoleWndProc;
    luaConsoleClass.cbClsExtra = 0;
    luaConsoleClass.cbWndExtra = 0;
    luaConsoleClass.hInstance = (HINSTANCE)&__ImageBase;
    luaConsoleClass.hIcon = NULL;
    luaConsoleClass.hCursor = NULL;
    luaConsoleClass.hbrBackground = NULL;
    luaConsoleClass.lpszMenuName = NULL;
    luaConsoleClass.lpszClassName = "LuaConsole";

    RegisterClassA(&luaConsoleClass);
    gContext.console = CreateWindowA(
        "LuaConsole",
        "Lua Console",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        (HINSTANCE)&__ImageBase,
        NULL
    );
    ShowWindow(gContext.console, SW_SHOW);
}
