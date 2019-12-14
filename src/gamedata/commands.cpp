#include "commands.h"
#include "../impl.h"

/*No patched offsets are needed here, so let's unify this*/
SrCommandRegistry::SrCommandRegistry(SrKernelStream* stream): SrNamedResourceRegistry<SrCommand, std::string>() {
    size_t read_size;
    CommandData object;

    /*Here we read from the KernelStream */
    auto commandIdx = 0;
    for (;;) {
        SrCommand srCommand = SrCommand();
        read_size = srKernelStreamRead(stream, &object, sizeof(object));
        if (read_size != sizeof(object))
            break;
        srCommand.gameCommand = object;
        srCommand.auxData.animationScriptIndex = getDefaultCmdAnimScript(commandIdx);
        srCommand.auxData.damageCalculationByte = getDefaultCmdDamage(commandIdx);
        srCommand.auxData.miscCommandFlags = getDefaultCmdFlags(commandIdx);
        srCommand.auxData.hasActions = getDefaultHasActions(commandIdx);
        registerDefaultCallbacks(commandIdx, srCommand);
        registerSelectCallbacks(commandIdx, srCommand);
        add_element(assembleGDataKey(commandIdx), srCommand);
        commandIdx++;
    }

    //Initialize enemy/game commands not loaded via kernel
    std::vector<u16> auxCommandIDs = { CMD_ENEMY_ACTION, 33, 34, CMD_POISONTICK };
    for (auto commandIdx : auxCommandIDs) {
        SrCommand srCommand = SrCommand();
        auto playerCommand = CommandData();
        srCommand.gameCommand = playerCommand;
        srCommand.auxData.animationScriptIndex = getDefaultCmdAnimScript(commandIdx);
        srCommand.auxData.damageCalculationByte = getDefaultCmdDamage(commandIdx);
        srCommand.auxData.miscCommandFlags = getDefaultCmdFlags(commandIdx);
        srCommand.auxData.hasActions = getDefaultHasActions(commandIdx);
        registerDefaultCallbacks(commandIdx, srCommand);
        registerSelectCallbacks(commandIdx, srCommand);
        add_element(assembleGDataKey(commandIdx), srCommand);
    }
}

void initCommands(SrKernelStream* stream) {
    gContext.commands = SrCommandRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu commands", (unsigned long)gContext.commands.resource_count());
}

const SrCommand& getCommand(u8 commandIdx) {
    auto& ret = gContext.commands.get_resource(commandIdx);
    srLogWrite("Fetching get command for command index, %d", commandIdx);
    return ret;
}

const SrAttack& getCommandAction(u8 commandIdx, u16 actionIdx) {
    auto actionTableIdx = 0;
    auto& srCommand = getCommand(commandIdx);
    srLogWrite("requesting command: %d action %d", commandIdx, actionIdx);
    if (actionIdx > srCommand.actionCount) {
        return gContext.attacks.get_resource(actionTableIdx);
    }
    actionTableIdx = srCommand.commandActions[actionIdx];
    srLogWrite("getting attack with true index %d", actionTableIdx);
    srLogWrite("fetched action name %s", gContext.attacks.get_resource(actionTableIdx).attackName.str());
    return gContext.attacks.get_resource(actionTableIdx);
}


SISTERRAY_API void addActionToCommand(const char* commandName, const char* actionName) {
    addCommandAction(std::string(commandName), std::string(actionName));
}

void addCommandAction(const std::string commandKey, const std::string actionKey) {
    auto& srCommand = gContext.commands.get_element(commandKey);
    srLogWrite("fetching index for actionKey %s", actionKey.c_str());
    auto trueAtkIdx = gContext.attacks.get_resource_index(actionKey);
    srLogWrite("adding true idx %d to command %s", trueAtkIdx, commandKey.c_str());
    srCommand.commandActions.push_back(trueAtkIdx);
}

void setCommandAction(const std::string commandKey, const std::string actionKey, u32 actionIndex) {
    auto& srCommand = gContext.commands.get_element(commandKey);
    auto attackIdx = gContext.attacks.get_resource_index(actionKey);
    if (actionIndex >= srCommand.commandActions.size()) {
        srCommand.commandActions.resize(actionIndex + 1);
    }
    srCommand.commandActions[actionIndex] = attackIdx;
}


/*run every initializer callback in order*/
SISTERRAY_API void runSetupCallbacks(const char* name) {
    auto idx = gContext.commands.get_resource_index(std::string(name));
    runSetupCallbacks(idx);
}

void runSetupCallbacks(u16 commandIdx) {
    CommandSetupEvent setupEvent = { gDamageContextPtr };
    auto& callbacks = getCommand(commandIdx).setupCallbacks;
    for (auto callback : callbacks) {
        callback(setupEvent);
    }
}

void runSelectCallbacks(EnabledCommandStruct& command, Menu* menu) {
    SelectCommandEvent setupEvent = { menu, &command };
    auto& callbacks = getCommand(command.commandID).selectCallbacks;
    for (auto callback : callbacks) {
        callback(&setupEvent);
    }
}

/*One off functions used to initialize data in the registries*/
u16 getDefaultCmdAnimScript(u16 commandIdx) {
    switch (commandIdx) {
        case 1: {
            return 0x14;
        }
        case 2:
        case 21: {
            return 0x1D;
            break;
        }
        case 3:
        case 22: {
            return 0x1F;
            break;
        }
        case 4:
        case 23: {
            return 0x21;
            break;
        }
        case 5: {
            return 0x22;
            break;
        }
        case 6: {
            return 0x26;
            break;
        }
        case 7: {
            return 0x21;
            break;
        }
        case 8: {
            return 0x21;
            break;
        }
        case 9: {
            return 0x28;
            break;
        }
        case 10: {
            return 0x2A;
            break;
        }
        case 11: {
            return 0x2C;
            break;
        }
        case 13: {
            return 0x1E;
            break;
        }
        case 17: {
            return 0x24;
            break;
        }
        case 24: {
            return 0x15;
            break;
        }
        case 25: {
            return 0x16;
            break;
        }
        case 26: {
            return 0x1C;
            break;
        }
        case 27: {
            return 0x18;
            break;
        }
        case 35: {
            return 0x2E;
            break;
        }
        default: {
            return 0;
            break;
        }
    }
}

//
u8 getDefaultCmdDamage(u16 commandIdx) {
    switch (commandIdx) {
        case 1: {
            return 0x11;
        }
        case 4:
        case 23: {
            return 0x37;
            break;
        }
        case 7: {
            return 0xA;
            break;
        }
        case 8: {
            return 0x9;
            break;
        }
        case 9: {
            return 0xB1;
            break;
        }
        case 10: {
            return 0x11;
            break;
        }
        case 11: {
            return 0x90;
            break;
        };
        case 17: {
            return 0x11;
            break;
        }
        case 18: {
            return 0x11;
            break;
        }
        case 24: {
            return 0x11;
            break;
        }
        case 25: {
            return 0x11;
            break;
        }
        case 26: {
            return 0x11;
            break;
        }
        case 27: {
            return 0x11;
            break;
        }
        case 35: {
            return 0x4;
            break;
        }
        default: {
            return 0;
            break;
        }
    }
}


u16 getDefaultCmdFlags(u16 commandIdx) {
    switch (commandIdx) {
        case 5: {
            return 0x6;
            break;
        }
        case 6: {
            return 0x412;
            break;
        }
        case 9: {
            return 0x2000;
            break;
        }
        case 17: {
            return 0x4;
            break;
        }
        case 18: {
            return 0x2;
            break;
        }
        case 25: {
            return 0x1000;
            break;
        }
        case 26: {
            return 0x2;
            break;
        }
        case 27: {
            return 0x1000;
            break;
        }
        default: {
            return 0;
            break;
        }
    }
}

void registerDefaultCallbacks(u16 commandIdx, SrCommand& auxCommand) {
    switch (commandIdx) {
        case 0:
        case 1: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 2:
        case 21:
        case 3:
        case 22: {
            auxCommand.setupCallbacks.push_back(&loadAbility);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 4:
        case 23: {
            auxCommand.setupCallbacks.push_back(&setupItem);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 5: {
            auxCommand.setupCallbacks.push_back(&setupSteal);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 6: {
            auxCommand.setupCallbacks.push_back(&setupSense);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 7: {
            auxCommand.setupCallbacks.push_back(&setupCoin);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 8: {
            auxCommand.setupCallbacks.push_back(&setupThrow);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 9: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&setupMorph);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 10: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&setupDeathblow);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 11: {
            auxCommand.setupCallbacks.push_back(&setupManipulate);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 12: {
            auxCommand.setupCallbacks.push_back(&setupMime);
            break;
        }
        case 13: {
            auxCommand.setupCallbacks.push_back(&loadAbility);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 17: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 18: {
            auxCommand.setupCallbacks.push_back(&setupMove);
            break;
        }
        case 19: {
            //Defend setup
            break;
        }
        case 20: {
            auxCommand.setupCallbacks.push_back(&setupLimit);
            auxCommand.setupCallbacks.push_back(&loadAbility);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 24: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 25: {
            auxCommand.setupCallbacks.push_back(&setupDoubleCut);
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 26: {
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&setupFlash);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 27: {
            auxCommand.setupCallbacks.push_back(&setupQuadCut);
            auxCommand.setupCallbacks.push_back(&weaponSetup);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 32: {
            auxCommand.setupCallbacks.push_back(&setupEnemyAttack);
            auxCommand.setupCallbacks.push_back(&loadAbility);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 33: {
            break;
        }
        case 34: {
            break;
        }
        case 35: {
            auxCommand.setupCallbacks.push_back(&setupPoison);
            auxCommand.setupCallbacks.push_back(&applyDamage);
            break;
        }
        case 36: {
            break;
        }
        case 37: {
            break;
        }
        default: {
            break;
        }
    }
}

void registerSelectCallbacks(u16 commandIdx, SrCommand& auxCommand) {
    switch (commandIdx) {
        case 0:
        case 1: {
            auxCommand.selectCallbacks.push_back(&handleWeaponTarget);
            break;
        }
        case 2: {
            auxCommand.selectCallbacks.push_back(&cmdMagicSelectHandler);
            break;
        }
        case 21: {
            auxCommand.selectCallbacks.push_back(&cmdWMagicSelectHandler);
            break;
        }
        case 3: {
            auxCommand.selectCallbacks.push_back(&cmdSummonSelectHandler);
            break;
        }
        case 22: {
            auxCommand.selectCallbacks.push_back(&cmdWSummonSelectHandler);
            break;
        }
        case 4: {
            auxCommand.selectCallbacks.push_back(&cmdItemSelectHandler);
            break;
        }
        case 23: {
            auxCommand.selectCallbacks.push_back(&cmdWItemSelectHandler);
            break;
        }
        default: {
            break;
        }
    }
}

u16 getDefaultHasActions(u16 commandIdx) {
    switch (commandIdx) {
    case 2:
    case 21:
    case 3:
    case 22: 
    case 4: 
    case 23:
    case 20:
    case 7:
    case 8:
    case 32: {
        return 1;
        break;
    }
    default: {
        return 0;
        break;
    }
    }
}
