#include <string>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "impl.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static WNDPROC      editProc;
static std::string  consoleLine;
static std::string  consoleBuffer =
    "# Welcome to the lua console.\r\n"
    "# You can type and execute live lua code here.\r\n"
    "\r\n"
    "\r\n"
    "> ";
static std::string composeBuffer;

static std::string execLua(const std::string input)
{
    std::string str;
    std::string ret;

    str = "return (" + input + ")";
    luaL_dostring(gContext.L, str.c_str());
    ret = luaL_tolstring(gContext.L, -1, NULL);
    lua_pop(gContext.L, 2);

    return ret;
}

static LRESULT CALLBACK luaConsoleWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CHAR:
        switch (wParam)
        {
        case 0x08:
            // Backspace
            if (!consoleLine.empty())
                consoleLine.pop_back();
            break;
        case 0x0A:
        case 0x0D:
            consoleBuffer += consoleLine;
            consoleBuffer += "\r\n";
            consoleBuffer += execLua(consoleLine);
            consoleBuffer += "\r\n> ";
            consoleLine = "";
            break;

        default:
            consoleLine += (char)wParam;
            break;
        }
        composeBuffer = consoleBuffer + consoleLine;
        SendMessageA(hwnd, WM_SETTEXT, 0, (LPARAM)composeBuffer.c_str());
        return CallWindowProc(editProc, hwnd, message, wParam, lParam);

    case WM_SETFOCUS:
        while (ShowCursor(TRUE) < 0) {}
        return 0;

    case WM_KILLFOCUS:
        while (ShowCursor(FALSE) >= 0) {}
        return 0;
    }
    return CallWindowProc(editProc, hwnd, message, wParam, lParam);
}

SISTERRAY_API void srInitLuaConsole(void)
{
    gContext.console = CreateWindowA(
        "EDIT",
        "Lua Console",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        NULL,
        NULL);
    editProc = (WNDPROC)SetWindowLongPtr(gContext.console, GWLP_WNDPROC, (LONG_PTR)luaConsoleWndProc);
    SendMessageA(gContext.console, WM_SETTEXT, 0, (LPARAM)consoleBuffer.c_str());
    SendMessageA(gContext.console, EM_SETREADONLY, 1, 0);
    ShowWindow(gContext.console, SW_SHOW);
}
