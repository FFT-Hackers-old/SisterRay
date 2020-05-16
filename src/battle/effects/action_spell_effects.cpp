#include "action_spell_effects.h"
#include "../battle_utils.h"
#include "summon_effects.h"
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

u8 isEffectLoading() {
    u8* G_EFFECT_LOADING = (u8*)0xBF2A30;
    return *G_EFFECT_LOADING;
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

SRPFNSPELLLOADERCALLBACK srGetLoaderCallback(SrAnimationType animationType, u8 relativeSpellEffectID) {
    SRPFNSPELLLOADERCALLBACK* gameLoaders = (SRPFNSPELLLOADERCALLBACK*)0x7C1208;
    u16* magicLoaderIdxMap = (u16*)0x7C17A0;
    u16* summonLoaderIdxMap = (u16*)0x7C1840;
    u16* itemLoaderIdxMap =(u16*)0x7C1710;
    u16* eskillLoaderIdxMap = (u16*)0x7C1810;
    u16* limitLoaderIdxMap = (u16*)0x7C19A8;
    u16* enemyAttackLoaderIdxMap = (u16*)0x7C1868;
    u16* throwLoaderIdxMap = (u16*)0x7C1A48;
    u16* coinLoaderIdxMap = (u16*)0x7C157C;
    switch (animationType) {
    case MAGIC: {
        return gameLoaders[magicLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case SUMMON: {
        return gameLoaders[summonLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case ITEM: {
        return gameLoaders[itemLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case COIN: {
        return gameLoaders[coinLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case THROW: {
        return gameLoaders[throwLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case ENEMY_SKILL: {
        return gameLoaders[eskillLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case LIMIT: {
        return gameLoaders[limitLoaderIdxMap[relativeSpellEffectID]];
        break;
    }
    case ENEMY_ATTACK: {
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
    if (*dword_7C10D8) {
        *byte_9ADF00 = actorID;
        *byte_9ADEFC = 0;
        auto animationType = ENEMY_ATTACK;
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
                if (overrrideLoader != nullptr) {
                    srLoadAnimationEffect(overrrideLoader);
                }
            }
            animEffectID = command.auxData.animationEffectID;
            animationType = command.auxData.animationType;
            useMulti = (command.auxData.useMulti != 0) ? true : false;
            auto multiCallback = command.auxData.useMulti;
        }

        if (animationType == NO_EFFECT || animEffectID == 0xFF) {
            srLogWrite("Animation type or animEffectID are 0xFF, and no override was specified, skipping animation load");
            return;
        }

        if (useMulti && getTargetAllActive()) {
            auto loadCallback = multiCallback.loadCallback;
            srLoadAnimationEffect(loadCallback);
            return;

        }
        srLoadAnimationEffect(srGetLoaderCallback(animationType, animEffectID));
    }
    *byte_9ADEFC = 0;
}

SRPFNSPELLEFFECTCALLBACK srGetDispatchCallback(SrAnimationType animationType, u8 animEffectID) {
    SRPFNSPELLEFFECTCALLBACK* magicEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE4B0;
    SRPFNSPELLEFFECTCALLBACK* summonLoaderIdxMap = (SRPFNSPELLEFFECTCALLBACK*)0x7C1840;
    SRPFNSPELLEFFECTCALLBACK* itemEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE398;
    SRPFNSPELLEFFECTCALLBACK* eskillEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE588;
    SRPFNSPELLEFFECTCALLBACK* limitEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE860;
    SRPFNSPELLEFFECTCALLBACK* enemyAtkEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE5E8;
    SRPFNSPELLEFFECTCALLBACK* throwEffectCallbackTable = (SRPFNSPELLEFFECTCALLBACK*)0x8FE9A0;
    SRPFNSPELLEFFECTCALLBACK coinLoaderCallback = (SRPFNSPELLEFFECTCALLBACK)0x4CE3BE;
    srLogWrite("Fetching effect with animation type %d, effect ID: %d", animationType, animEffectID);
    switch (animationType) {
    case MAGIC: {
        return magicEffectCallbackTable[animEffectID];
        break;
    }
    case SUMMON: {
        return (SRPFNSPELLEFFECTCALLBACK)summonEffectMainDispatcher;;
        break;
    }
    case ITEM: {
        return itemEffectCallbackTable[animEffectID];
        break;
    }
    case COIN: {
        return coinLoaderCallback;
        break;
    }
    case THROW: {
        return throwEffectCallbackTable[animEffectID];
        break;
    }
    case ENEMY_SKILL: {
        return eskillEffectCallbackTable[animEffectID];
        break;

    }
    case LIMIT: {
        return limitEffectCallbackTable[animEffectID];
        break;
    }
    case ENEMY_ATTACK: {
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
    u32* dword_7C10D8 = (u32*)0x7C10D8;
    if (!*dword_7C10D8) {
        sub_429322();
    }

    *byte_BF23BC = 0;
    auto animationType = ENEMY_ATTACK;
    bool useMulti = false;
    SpellEffect multiCallback = { nullptr, nullptr };
    u8 animEffectID = 0xFF;
    srLogWrite("Entering effect dispatch with actor: %d, cmdIdx: %d, actionIdx: %d", actorID, commandIdx, actionIdx);
    auto command = getCommand(commandIdx);
    if (command.auxData.hasActions) {
        auto action = getCommandAction(commandIdx, actionIdx);
        animEffectID = action.attackData.animationEffectID;
        srLogWrite("Animation Effect Idx: %i", animEffectID);
        animationType = action.animationType;
        srLogWrite("Effect Dispatch type: %i", animationType);
        if (action.useOverride) {
            AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, getAnimatingActionTargetMask() };
            gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
            action.overrideEffect.dispatchCallback(getAnimatingActionTargetMask(), actorID);
        }
        useMulti = (action.useMulti != 0) ? true : false;
        auto multiCallback = action.multiEffect;
    }
    else {
        if (command.auxData.useOverride) {
            animEffectID = command.auxData.animationEffectID;
            animationType = command.auxData.animationType;
            AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, getAnimatingActionTargetMask() };
            gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
            command.auxData.override.dispatchCallback(getAnimatingActionTargetMask(), actorID);
        }
        useMulti = (command.auxData.useMulti != 0) ? true : false;
        auto multiCallback = command.auxData.spellEffectMulti;
    }
    if (animationType == NO_EFFECT || animEffectID == 0xFF) {
        srLogWrite("Animation type or animEffectID are 0xFF, and no override was specified, skipping animation load");
        return;
    }

    if (useMulti && getTargetAllActive()) {
        AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, getAnimatingActionTargetMask() };
        gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
        multiCallback.dispatchCallback(getAnimatingActionTargetMask(), actorID);
        return;
    }
    srLogWrite("dispatching spell anim effect with type: %d and animID %d", animationType, animEffectID);
    AnimEffectEvent srEvent = { actorID, animationType, animEffectID, commandIdx, actionIdx, getAnimatingActionTargetMask() };
    gContext.eventBus.dispatch(ON_DISPATCH_ANIMAMTION_EFFECT, (void*)&srEvent);
    auto effectCallback = srGetDispatchCallback(animationType, animEffectID);
    effectCallback(getAnimatingActionTargetMask(), actorID);
}
