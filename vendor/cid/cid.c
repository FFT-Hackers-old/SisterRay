#define _CRT_SECURE_NO_WARNINGS  1
#include <windows.h>

#define STRINGIFY(a)            #a
#define STRINGIFY2(a)           STRINGIFY(a)
#define INJECTED_TARGET         STRINGIFY2(CID_TARGET)
#define INJECTED_DLL            STRINGIFY2(CID_DLL)
#define INJECTED_INIT           STRINGIFY2(CID_INIT)

int main(void)
{
    PROCESS_INFORMATION proc;
    STARTUPINFOA si;

    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pLoadLibraryA = GetProcAddress(kernel32, "LoadLibraryA");
    FARPROC pGetProcAddress = GetProcAddress(kernel32, "GetProcAddress");

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcessA(INJECTED_TARGET, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &proc))
    {
        wchar_t buf[256];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
        MessageBoxW(NULL, buf, NULL, 0);
        return 1;
    }
    LPVOID injectBase = VirtualAllocEx(proc.hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    char buffer[4096];
    size_t i = 0;
    memset(buffer, 0, 4096);

    buffer[i++] = 0x83; // sub esp
    buffer[i++] = 0xec;
    buffer[i++] = 0x40;

    buffer[i++] = 0x68; // push
    *(DWORD*)(buffer + i) = (DWORD)injectBase + 2048;
    i += 4;

    buffer[i++] = 0xb8; // mov eax
    *(DWORD*)(buffer + i) = (DWORD)pLoadLibraryA;
    i += 4;

    buffer[i++] = 0xff; // call eax
    buffer[i++] = 0xd0;

    buffer[i++] = 0x68; // push
    *(DWORD*)(buffer + i) = (DWORD)injectBase + 2048 + strlen(INJECTED_DLL) + 1;
    i += 4;

    buffer[i++] = 0x50; // push eax

    buffer[i++] = 0xb8; // mov eax
    *(DWORD*)(buffer + i) = (DWORD)pGetProcAddress;
    i += 4;

    buffer[i++] = 0xff; // call eax
    buffer[i++] = 0xd0;

    buffer[i++] = 0xff; // call eax
    buffer[i++] = 0xd0;

    buffer[i++] = 0x31; // xor eax, eax
    buffer[i++] = 0xc0;

    buffer[i++] = 0x83; // add esp
    buffer[i++] = 0xc4;
    buffer[i++] = 0x40;

    buffer[i++] = 0xc2; // ret
    buffer[i++] = 0x04;
    buffer[i++] = 0x00;

    strcpy(buffer + 2048, INJECTED_DLL);
    strcpy(buffer + 2048 + strlen(INJECTED_DLL) + 1, INJECTED_INIT);

    WriteProcessMemory(proc.hProcess, injectBase, buffer, 4096, NULL);
    FlushInstructionCache(proc.hProcess, injectBase, 4096);
    HANDLE t = CreateRemoteThread(proc.hProcess, 0, 0, injectBase, NULL, 0, NULL);
    WaitForSingleObject(t, INFINITE);
    VirtualFreeEx(proc.hProcess, injectBase, 0, MEM_RELEASE);
    FlushInstructionCache(proc.hProcess, injectBase, 4096);
    ResumeThread(proc.hThread);
    return 0;
}
