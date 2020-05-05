#include "attacks.h"
#include "../impl.h"
#include "gdata_utils.h"
#include "element_names.h"

#define KERNEL_MAGIC_CUTOFF 56
#define KERNEL_SUMMON_CUTOFF 72
#define KERNEL_ESKILL_CUTOFF 96

SrAttackRegistry::SrAttackRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrAttack, std::string>() {
    size_t read_size;
    AttackData baseAttack;
    std::string srAttackID;
    SrAttack attack96;
    SrAttack attack97;

    auto idx = 0;
    while (1) {
        SrAttack attack;
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseAttack, sizeof(baseAttack));
        if (read_size != sizeof(baseAttack))
            break;
        attack.attackData = baseAttack;
        attack.attackName = gContext.gameStrings.magic_names.getResource(idx);
        attack.attackDescription = gContext.gameStrings.magic_descriptions.getResource(idx);
        if (idx == 96 || idx == 97) {
            attack.attackID = idx;
            cmdIdx = CMD_SUMMON;
            attack.attackID = 0xFFFF;
            attack.animationType = SUMMON;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
            if (idx == 96)
                attack96 = attack;
            if (idx == 97)
                attack97 = attack;
            continue;
        }
        else if (idx < KERNEL_MAGIC_CUTOFF) {
            attack.attackID = idx;
            cmdIdx = CMD_MAGIC;
            attack.animationType = MAGIC;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = getDefaultMagicUseMulti(idx);
            attack.multiEffect = getDefaultMagicMultiEffects(idx);
        }
        else if (idx < KERNEL_SUMMON_CUTOFF) {
            attack.attackName = gContext.gameStrings.magic_names.getResource(idx);
            attack.attackDescription = gContext.gameStrings.magic_descriptions.getResource(idx);
            attack.attackID = idx;
            cmdIdx = CMD_SUMMON;
            attack.animationType = SUMMON;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        else if (idx < KERNEL_ESKILL_CUTOFF) {
            attack.attackID = idx;
            cmdIdx = CMD_ENEMY_SKILL;
            attack.animationType = ENEMY_SKILL;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        else {
            attack.attackID = idx;
            cmdIdx = CMD_LIMIT;
            attack.animationType = LIMIT;
            attack.animScriptIndex = idx;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        initializeActionElements(attack);
        initializeStatusAfflictions(attack);
        setSrDamageInfo<SrAttack>(attack, baseAttack.damageFormula);
        addElement(assembleGDataKey(attack.attackID), attack);
        auto& cmd = gContext.commands.getResource(cmdIdx);
        cmd.actionCount++;
        cmd.commandActions.push_back(getResourceIndex(assembleGDataKey(attack.attackID)));
        ++idx;
    }
    // Init gunge lance and fat chocobo
    addElement(assembleGDataKey(attack96.attackID), attack96);
    addCommandAction(assembleGDataKey(CMD_SUMMON), assembleGDataKey(attack96.attackID));
    addElement(assembleGDataKey(attack97.attackID), attack96);
    addCommandAction(assembleGDataKey(CMD_SUMMON), assembleGDataKey(attack97.attackID));

    //Initialize Limit Break Attacks
}

//Set elements 
void initializeActionElements(SrAttack& attack) {
    auto& attackElements = attack.attackElements;
    for (auto elementIdx = 0; elementIdx < 16; elementIdx++) {
        if (!(attack.attackData.elementMask & (1 << elementIdx))) {
            continue;
        }
        attackElements.push_back(getElementIDFromIndex(elementIdx));
    }
}

void initializeStatusAfflictions(SrAttack& attack) {
    auto& statusAttack = attack.statusAttack;
    if (attack.attackData.statusMask == 0xFFFFFFFF) {
        return;
    }
    for (auto statusIdx = 0; statusIdx < 32; statusIdx++) {
        if (!(attack.attackData.statusMask & (1 << statusIdx))) {
            continue;
        }
        srLogWrite("added infliction for status: %s", getStatusIDFromIndex(statusIdx).c_str());
        StatusInfliction infliction{ getStatusIDFromIndex(statusIdx), attack.attackData.statusInflictType & 0x3F, attack.attackData.statusInflictType & 0x80, attack.attackData.statusInflictType & 0x40 };
        statusAttack.push_back(infliction);
    }
    srLogWrite("Intialized %i statuses for attack %i", attack.statusAttack.size(), attack.attackID);
}

void initAttacks(SrKernelStream* stream) {
    gContext.attacks = SrAttackRegistry(stream);
}

u16 getDefaultMagicUseMulti(u16 actionID) {
     switch (actionID) {
     case 0x1D:
     case 0x20:
     case 0x23:
     case 0x29:
     case 0x2C: {
         return 1;
         break;
     }
     default: {
         return 0;
         break;
     }
     }
}

 SpellEffect getDefaultMagicMultiEffects(u16 actionID) {
     auto multiEffect = SpellEffect();
     switch (actionID) {
     case 0x1D: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x56D910;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x56D880;
         break;
     }
     case 0x20: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x49B0D0;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x49B040;
         break;
     }
     case 0x23: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5B6930;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5B68A0;
         break;
     }
     case 0x29: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5A2650;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5A2570;
         break;
     }
     case 0x2C: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5A7E60;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5A7DD0;
     }
     default: {
         break;
     }
     }
     return multiEffect;
 }

 SISTERRAY_API void addSrAction(SrActionData data, u16 modActionIdx, const char* modName) {
     auto name = std::string(modName) + std::to_string(modActionIdx);
     auto srAttack = SrAttack();
     srAttack.attackData = data.baseData;
     // srAttack.auxData = data.auxData;
     srAttack.attackName = EncodedString::from_unicode(data.attackName);
     srAttack.attackDescription = EncodedString::from_unicode(data.attackName);
     gContext.attacks.addElement(name, srAttack);
     srLogWrite("Mod %s added action with true idx: %i ", modName, gContext.attacks.getResourceIndex(name));
 }


 SISTERRAY_API SrActionData getSrAction(u16 attackIdx, const char* modName) {
     auto srAttack = gContext.attacks.getElement(std::string(modName) + std::to_string(attackIdx));
     SrActionData ret{ srAttack.attackData, srAttack.attackName.str(), srAttack.attackDescription.str() };
     return ret;
 }


 SISTERRAY_API void addElementToAction(const  char* modName, u16 modActionIdx, const char* elementName) {
     auto actionID = std::string(modName) + std::to_string(modActionIdx);
     
     auto& elements = gContext.attacks.getElement(actionID).attackElements;
     if (std::find(elements.begin(), elements.end(), elementName) != elements.end()) {
         elements.push_back(elementName);
     }
 }

 SISTERRAY_API void addStatusToAction(const char* modName, u16 modActionIdx, const char* statusName, u8 inflictionChance, u8 doesRemove, u8 doesToggle, u16 duration) {
     auto actionID = std::string(modName) + std::to_string(modActionIdx);

     auto& status = gContext.attacks.getElement(actionID).statusAttack;
     StatusInfliction infliction{ statusName, inflictionChance, doesRemove, doesToggle, duration };
     if (std::find_if(status.begin(), status.end(), [&](StatusInfliction infliction) {return infliction.statusName == statusName; }) != status.end()) {
         status.push_back(infliction);
     }
 }


 SISTERRAY_API SrActionData getSrCommandAction(const char* modName, u8 modCmdIdx, u16 cmdAtkIdx){
     u16 actionIdx = gContext.commands.getElement(std::string(modName) + std::to_string(modCmdIdx)).commandActions[cmdAtkIdx];
     auto& srAction = gContext.attacks.getResource(actionIdx);
     SrActionData ret{srAction.attackData, srAction.attackName.str(), srAction.attackDescription.str()};
     return ret;
 }

 SISTERRAY_API SrActionData getSrCommandSwapAction(const char* modName, u8 modCmdIdx, u16 cmdAtkIdx) {
     u16 actionIdx = gContext.commands.getElement(std::string(modName) + std::to_string(modCmdIdx)).swapActions[cmdAtkIdx];
     auto& srAction = gContext.attacks.getResource(actionIdx);
     SrActionData ret{ srAction.attackData, srAction.attackName.str(), srAction.attackDescription.str() };
     return ret;
 }

 SISTERRAY_API void setActionAnimationScript(const char* modName, u16 modActionID, const char* modelName, const char* scriptModName, u16 modScriptID) {
     auto name = std::string(modName) + std::to_string(modActionID);
     auto& attack = gContext.attacks.getElement(name);
     auto animScriptName = std::string(scriptModName) + std::to_string(modScriptID);
     attack.animScriptIndex = gContext.battleAnimationScripts.getElement(modelName).modelAnimScripts.getResourceIndex(animScriptName);
     srLogWrite("modelName: %s, animScriptName: %s", modelName, animScriptName.c_str());
     srLogWrite("Initializing Animation Script for Action: %s to value %i", name.c_str(), gContext.battleAnimationScripts.getElement(modelName).modelAnimScripts.getResourceIndex(animScriptName));
 }

 SISTERRAY_API void setActionEffectType(const char* modName, u16 modActionID, SrAnimationType effectType) {
     auto name = std::string(modName) + std::to_string(modActionID);
     auto& attack = gContext.attacks.getElement(name);
     attack.animationType = effectType;
 }

 SISTERRAY_API void setEffectCallback(const char* modName, u16 modActionID, SRPFNSPELLEFFECTCALLBACK callback) {
     auto name = std::string(modName) + std::to_string(modActionID);
     auto& attack = gContext.attacks.getElement(name);
     attack.overrideEffect.dispatchCallback = callback;
     attack.useOverride = true;
 }
