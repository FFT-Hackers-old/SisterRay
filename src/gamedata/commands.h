#ifndef COMMANDS_H
#define COMMANDS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "command_callbacks.h"
#include "gdata_utils.h"

#define KERNEL_COMMAND_COUNT 32

typedef struct {
    AuxCommandData auxData;
    std::vector<SRPFNCOMMANDSETUP> setupCallbacks;
} PAuxCommandData;

class SrCommandRegistry : public SrNamedResourceRegistry<CommandData, std::string> {
public:
    SrCommandRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<CommandData, std::string>(stream) {};
    SrCommandRegistry() : SrNamedResourceRegistry<CommandData, std::string>() {}
};

class SrAuxCommandRegistry : public SrNamedResourceRegistry<PAuxCommandData, std::string> {
public:
    SrAuxCommandRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<PAuxCommandData, std::string>(stream) {};
    SrAuxCommandRegistry() : SrNamedResourceRegistry<PAuxCommandData, std::string>() {}
};


SISTERRAY_API void runSetupCallbacks(const char* name);
void runSetupCallbacks(u16 commandIdx);
SISTERRAY_API void initCommands(SrKernelStream* stream);
#endif
