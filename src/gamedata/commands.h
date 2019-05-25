#ifndef COMMANDS_H
#define COMMANDS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"

class SrCommandRegistry : public SrNamedResourceRegistry<CommandData, std::string> {
public:
    SrCommandRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<CommandData, std::string>(stream) {};
    SrCommandRegistry() : SrNamedResourceRegistry<CommandData, std::string>() {}
};


SISTERRAY_API void initCommands(SrKernelStream* stream);
#endif
