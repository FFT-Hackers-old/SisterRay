#include "action_spell_effects.h"
#include "../../impl.h"

#define gameBeatRushEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E1627)
#define gameSomersaultEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E163E)
#define gameWaterkickEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E1655)
#define gameMeteodriveEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E166C)
#define gameDolphinBlowEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E1683)
#define gameMeteorStrikeEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E169A)
#define gameFinalHeavenEffect ((SRPFNSPELLEFFECTCALLBACK)0x4E16B1)

typedef void(*SRPFNLOADANIMATIONEFFECT)(SRPFNSPELLLOADERCALLBACK);
#define gameLoadAnimationEffect          ((SRPFNLOADANIMATIONEFFECT)0x4284A7)

void srLoadAnimationEffect(SRPFNSPELLLOADERCALLBACK loader) {
    gameLoadAnimationEffect(loader);
}

void srBeatRushSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameBeatRushEffect(currentTargetMask, actorID);
}

void srBeatSomersaultEffect(u32 currentTargetMask, u32 actorID) {
    gameSomersaultEffect(currentTargetMask, actorID);
}

void srWaterkickSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameWaterkickEffect(currentTargetMask, actorID);
}

void srMeteodriveSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameMeteodriveEffect(currentTargetMask, actorID);
}

void srDoplhinBlowSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameDolphinBlowEffect(currentTargetMask, actorID);
}

void srMeteorStrikeSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameMeteorStrikeEffect(currentTargetMask, actorID);
}

void srFinalHeavenSpellEffect(u32 currentTargetMask, u32 actorID) {
    gameFinalHeavenEffect(currentTargetMask, actorID);
}

SRPFNSPELLLOADERCALLBACK srGetLoaderCallback(u8 dispatchCommandIdx, u8 relativeSpellEffectID) {
    SRPFNSPELLLOADERCALLBACK* gameLoaders = (SRPFNSPELLLOADERCALLBACK*)0x7C1208;
    u16* magicLoaderIdxMap = (u16*)0x7C17A0;
    u16* summonLoaderIdxMap = (u16*)0x7C1840;
    u16* itemLoaderIdxMap =(u16*)0x7C1710;
    u16* eskillLoaderIdxMap = (u16*)0x7C1810;
    u16* limitLoaderIdxMap = (u16*)0x7C19A8;
    u16* enemyAttackLoaderIdxMap = (u16*)0x7C1868;
    u16* throwLoaderIdxMap = (u16*)0x7C1A48;
    u16* coinLoaderIdxMap = (u16*)0x7C157C;
    switch (dispatchCommandIdx) {
    case 2: {
        return gameLoaders[magicLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 3: {
        return gameLoaders[summonLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 4: {
        return gameLoaders[itemLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 7: {
        return gameLoaders[coinLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 8: {
        return gameLoaders[throwLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 13: {
        return gameLoaders[eskillLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 20: {
        return gameLoaders[limitLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case 32: {
        return gameLoaders[enemyAttackLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    default: {
        return gameLoaders[magicLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    }
}

void srLoadActionSpellEffects(u8 actorID, u8 commandIdx, u16 actionIdx) {
    u8* byte_9ADEFC = (u8*)0x9ADEFC;
    u8* byte_9ADF00 = (u8*)0x9ADF00;
    u32* dword_7C10D8 = (u32*)0x7C10D8;
    u8* G_TARGETING_ALL = (u8*)0xBF2E1C;
    if (*dword_7C10D8) {
        *byte_9ADF00 = actorID;
        *byte_9ADEFC = 0;
        u8 animationType = CMD_ENEMY_ACTION;
        bool useMulti = false;
        u8 animEffectID = 0xFF;
        SpellEffect multiCallback = { nullptr, nullptr };

        if (commandIdx == CMD_SUMMON)
            *byte_9ADEFC = 1;

        auto command = getCommand(commandIdx);
        if (command.auxData.hasActions) {
            auto action = getCommandAction(commandIdx, actionIdx);
            if (action.useOverride) {
                auto overrrideLoader = action.overrideEffect.loadCallback;
                srLoadAnimationEffect(overrrideLoader);
            }
            animEffectID = action.attackData.animationEffectID;
            animationType = action.animationType;
            useMulti = (action.useMulti != 0) ? true : false;
            auto multiCallback = action.multiEffect;
        }
        else {
            if (command.auxData.useOverride) {
                auto overrrideLoader = command.auxData.override.loadCallback;
                srLoadAnimationEffect(overrrideLoader);
            }
            animEffectID = command.auxData.animationEffectID;
            animationType = command.auxData.animationType;
            useMulti = (command.auxData.useMulti != 0) ? true : false;
            auto multiCallback = command.auxData.useMulti;
        }

        if (animationType == 0xFF || animEffectID == 0xFF) {
            srLogWrite("Animation type or animEffectID are 0xFF, and no override was specified, skipping animation load");
            return;
        }

        if (useMulti && *G_TARGETING_ALL) {
            auto loadCallback = multiCallback.loadCallback;
            srLoadAnimationEffect(loadCallback);
            return;

        }
        srLoadAnimationEffect(srGetLoaderCallback(animationType, animEffectID));
    }
    *byte_9ADEFC = 0;
}

SRPFNSPELLEFFECTCALLBACK srGetDispatchCallback(u8 animationType, u8 animEffectID) {
    SRPFNSPELLEFFECTCALLBACK* magicEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE4B0;
    SRPFNSPELLEFFECTCALLBACK* summonLoaderIdxMap = (SRPFNSPELLEFFECTCALLBACK*)0x7C1840;
    SRPFNSPELLEFFECTCALLBACK* itemEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE398;
    SRPFNSPELLEFFECTCALLBACK* eskillEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE588;
    SRPFNSPELLEFFECTCALLBACK* limitEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE860;
    SRPFNSPELLEFFECTCALLBACK* enemyAtkEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE5E8;
    SRPFNSPELLEFFECTCALLBACK* throwEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE9A0;
    SRPFNSPELLEFFECTCALLBACK coinLoaderCallback = (SRPFNSPELLEFFECTCALLBACK)0x4CE3BE;
    switch (animationType) {
    case 2: {
        return magicEffectCallbackTable[animEffectID];
        break;
    }
    case 3: {
        return summonLoaderIdxMap[animEffectID];
        break;
    }
    case 4: {
        return itemEffectCallbackTable[animEffectID];
        break;
    }
    case 7: {
        return coinLoaderCallback;
        break;
    }
    case 8: {
        return throwEffectCallbackTable[animEffectID];
        break;
    }
    case 13: {
        return eskillEffectCallbackTable[animEffectID];
        break;

    }
    case 20: {
        return limitEffectCallbackTable[animEffectID];
        break;
    }
    case 32: {
        return enemyAtkEffectCallbackTable[animEffectID];
        break;
    }
    default: {
        return magicEffectCallbackTable[animEffectID];
        break;
    }
    }
}

typedef void(*PFNSRSUB429322)();
#define sub_429322         ((PFNSRSUB429322)0x429322)
void srDispatchActionSpellEffects(u8 actorID, u8 commandIdx, u16 actionIdx) {
    u8* byte_9ADEFC = (u8*)0x9ADEFC;
    u8* byte_BF23BC = (u8*)0xBF23BC;
    u8* G_TARGETING_ALL = (u8*)0xBF2E1C;
    u16* targetMask = (u16*)0xBFD0F8;
    u32* dword_7C10D8 = (u32*)0x7C10D8;
    if (!*dword_7C10D8) {
        sub_429322();
    }

    *byte_BF23BC = 0;
    u8 animationType = CMD_ENEMY_ACTION;
    bool useMulti = false;
    SpellEffect multiCallback = { nullptr, nullptr };
    u8 animEffectID = 0xFF;

    auto command = getCommand(commandIdx);
    if (command.auxData.hasActions) {
        auto action = getCommandAction(commandIdx, actionIdx);
        animEffectID = action.attackData.animationEffectID;
        animationType = action.animationType;
        if (action.useOverride) {
            AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, *targetMask };
            gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
            action.overrideEffect.dispatchCallback(*targetMask, actorID);
        }
        useMulti = (action.useMulti != 0) ? true : false;
        auto multiCallback = action.multiEffect;
    }
    else {
        if (command.auxData.useOverride) {
            animEffectID = command.auxData.animationEffectID;
            animationType = command.auxData.animationType;
            AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, *targetMask };
            gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
            command.auxData.override.dispatchCallback(*targetMask, actorID);
        }
        useMulti = (command.auxData.useMulti != 0) ? true : false;
        auto multiCallback = command.auxData.spellEffectMulti;
    }
    if (animationType == 0xFF || animEffectID == 0xFF) {
        srLogWrite("Animation type or animEffectID are 0xFF, and no override was specified, skipping animation load");
        return;
    }

    if (useMulti && *G_TARGETING_ALL) {
        AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, *targetMask };
        gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
        multiCallback.dispatchCallback(*targetMask, actorID);
        return;
    }
    AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, *targetMask };
    gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
    auto effectCallback = srGetDispatchCallback(animationType, animEffectID);
    effectCallback(*targetMask, actorID);
}
