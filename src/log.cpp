#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "impl.h"

static void LogExitHandler(void)
{
    fclose(gContext.logFile);
}

SISTERRAY_API void initLog(void)
{
    gContext.logFile = fopen(srGetGamePath("SisterRay.log"), "wb");
    atexit(LogExitHandler);
}

SISTERRAY_API void srLogWrite(const char* format, ...)
{
    char buffer[4096];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    fwrite(buffer, strlen(buffer), 1, gContext.logFile);
    fwrite("\r\n", 2, 1, gContext.logFile);
    fflush(gContext.logFile);
    va_end(ap);
}
