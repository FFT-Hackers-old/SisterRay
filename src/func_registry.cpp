#include <unordered_map>
#include <string>
#include "impl.h"

#define REGISTER(x) srRegisterFunction(#x, &x)

static std::unordered_map<std::string, const void*> gFuncReg;

SISTERRAY_API void initFunctionRegistry(void)
{
    REGISTER(srLoadFunction);
    REGISTER(srRegisterFunction);

    /* Mog */
    srRegisterFunction("srRedirectFunction", &mogRedirectFunction);
    srRegisterFunction("srReplaceFunction", &mogReplaceFunction);
    srRegisterFunction("srReplaceNop", &mogReplaceNop);
    srRegisterFunction("srReplaceSkip", &mogReplaceSkip);
    srRegisterFunction("srVirtualAddress", &mogVirtualAddress);
}

SISTERRAY_API const void* srLoadFunction(const char* name)
{
    return gFuncReg[name];
}

SISTERRAY_API const void* srRegisterFunction(const char* name, const void* func)
{
    const void* old;

    old = srLoadFunction(name);
    gFuncReg[name] = func;
    return old;
}
