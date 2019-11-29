#include <string>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "lua_api.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static HWND         editHwnd;
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
    int ret;
    std::string str;
    std::string out;

    str = "return (" + input + ")";
    ret = luaL_loadstring(gContext.L, str.c_str());
    if (ret != LUA_OK)
    {
        lua_pop(gContext.L, 1);
        ret = luaL_loadstring(gContext.L, input.c_str());
    }
    if (ret == LUA_OK)
    {
        lua_pcall(gContext.L, 0, 1, 0);
    }
    out = luaL_tolstring(gContext.L, -1, NULL);
    lua_pop(gContext.L, 2);

    return out;
}

static LRESULT CALLBACK consoleEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

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

static LRESULT CALLBACK consoleWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hwnd, &rect);
        editHwnd = CreateWindowA(
            "EDIT",
            "",
            WS_CHILD | WS_VSCROLL | WS_VISIBLE |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            0,
            0,
            rect.right,
            rect.bottom,
            hwnd,
            NULL,
            NULL,
            NULL
        );
        editProc = (WNDPROC)SetWindowLongPtr(editHwnd, GWLP_WNDPROC, (LONG_PTR)consoleEditProc);
        SendMessageA(editHwnd, WM_SETTEXT, 0, (LPARAM)consoleBuffer.c_str());
        SendMessageA(editHwnd, EM_SETREADONLY, 1, 0);
        return 0;

    case WM_SIZE:
        GetClientRect(hwnd, &rect);
        SetWindowPos(editHwnd, NULL, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
        return 0;

    case WM_SETFOCUS:
        SetFocus(editHwnd);
        return 0;
    }
    return DefWindowProcA(hwnd, message, wParam, lParam);
}

SISTERRAY_API void srInitLuaConsole(void)
{
    ATOM consoleAtom;
    WNDCLASSA consoleClass;

    consoleClass.style = 0;
    consoleClass.cbClsExtra = 0;
    consoleClass.cbWndExtra = 0;
    consoleClass.hbrBackground = NULL;
    consoleClass.hIcon = NULL;
    consoleClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    consoleClass.hInstance = (HINSTANCE)&__ImageBase;
    consoleClass.lpfnWndProc = consoleWndProc;
    consoleClass.lpszMenuName = NULL;
    consoleClass.lpszClassName = "LuaConsole";

    consoleAtom = RegisterClassA(&consoleClass);

    gContext.console = CreateWindowA(
        (LPCSTR)consoleAtom,
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
