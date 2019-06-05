#include "commands.h"
#include "../impl.h"


SISTERRAY_API void initCommands(SrKernelStream* stream) {
    gContext.commands = SrCommandRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu commands", (unsigned long)gContext.commands.resource_count());
}
