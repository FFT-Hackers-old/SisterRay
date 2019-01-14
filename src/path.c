#include <windows.h>
#include "impl.h"

SISTERRAY_API const char* srGetGamePath(const char* suffix)
{
    size_t len;
    static char buffer[MAX_PATH];

    memset(buffer, 0, MAX_PATH);
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    len = strlen(buffer);
    for (size_t i = len - 1; i > 0; --i)
    {
        if (buffer[i] == 0)
            break;
        if (buffer[i] == '/' || buffer[i] == '\\')
        {
            buffer[i] = 0;
            break;
        }
    }
    strcat_s(buffer, sizeof(buffer), "\\");
    strcat_s(buffer, sizeof(buffer), suffix);

    return buffer;
}
