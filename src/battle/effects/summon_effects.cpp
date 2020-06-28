#include "summon_effects.h"
#include "action_effects.h"
#include "../battle_models/model_names.h"
#include "../battle_utils.h"
#include "../battle_models/battle_models_api.h"
#include "../battle_models/model_setup_routines.h"
#include "../animation_script/animation_script_api.h"
#include "sound_effects.h"
#include "../../mods/controllable_summons/ifrit.h"
#include "../../impl.h"

void endSummonHandler() {
    srLogWrite("RUNNING END SUMMON HANDLER");
    u8* G_SCRIPT_WAIT_FRAMES = (u8*)0xBFD0F0;
    u8* byte_BFB2EC = (u8*)0xBFB2EC;
    u8* byte_BE6262 = (u8*)0xBE6262;
    if (getEffect60QueueTop()->wordArray[0]) {
        if (getEffect60QueueTop()->wordArray[1]) {
            --getEffect60QueueTop()->wordArray[1];
        }
        else {
            srLogWrite("Exiting End Summon Handler");
            getEffect60QueueTop()->handlerIndex = 0xFFFF;
            *byte_BFB2EC = 0;
            *byte_BE6262 = 1;
        }
    }
    else if (!getEffect100QueueSize()) {
        if (getActionActorIdx() < 3) {
            srLogWrite("Handling respawn summon for party");
            getBattleModelState(1)->modelEffectFlags |= 4u;
            getBattleModelState(1)->field_25 &= 0xFDu;
            for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
                if (gContext.party.getActivePartyMember(partyIdx).srPartyMember->isActive) {
                    getBattleModelState(partyIdx)->modelEffectFlags |= 4u;
                    getBattleModelState(partyIdx)->field_25 &= 0xFDu;
                }
            }
        }
        getEffect60QueueTop()->wordArray[0] = 1;
        getEffect60QueueTop()->wordArray[1] = 14;
        *G_SCRIPT_WAIT_FRAMES = 14;
    }
}

void summonEffectMainDispatcher(u16 actionTargetMask, u8 casterIdx) {
    srPushEffect100(summonMainEffect);
}

typedef void(*SRPFNSUB_SETSUMMONMODEL)(BattleModel*, void*);
#define setSummonModel           ((SRPFNSUB_SETSUMMONMODEL)0x429398)
#define sub_429322               ((PFNSR_VOIDSUB)0x429322)
#define convertSummonBData       ((PFNSR_VOIDSUB)0x5C1DF0)
#define sub_748F8F               ((PFNSR_INTSUB)0x748F8f)
void summonMainEffect() {
    u8* byte_BFB2EC = (u8*)0xBFB2EC;
    u8* byte_BE2C89 = (u8*)0xBE2C89;
    u8* byte_BE4775 = (u8*)0xBE4775;
    u8* byte_BE6261 = (u8*)0xBE6261;
    u8* byte_BF1EB0 = (u8*)0xBF1EB0;
    u8* G_SCRIPT_WAIT_FRAMES = (u8*)0xBFD0F0;
    u32* dword_7C10D8 = (u32*)0x7C10D8;
    u32** dword_9ADFD8 = (u32**)0x9ADFD8;

    auto summonEffecCtx = getEffect100QueueTop();
    u16 currentFrame = summonEffecCtx->wordArray[0];

    *byte_BF1EB0 = 0;
    switch (currentFrame) {
    case 0: {
        ++summonEffecCtx->wordArray[0];
        summonEffecCtx->wordArray[1] = 14;
        *G_SCRIPT_WAIT_FRAMES = 14;
        sub_748F8F(41);
        u8 caster = getActionActorIdx();
        if (caster < 3) {
            for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
                setModelVanish(partyIdx);
        }
        else {
            for (u8 enemyIdx = 4; enemyIdx < 10; ++enemyIdx)
                setModelVanish(enemyIdx);
        }
        if (getBattleType() == 4) {
            if (caster < 3) {
                for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
                    setModelVanish(partyIdx);
            }
            else {
                for (u8 enemyIdx = 4; enemyIdx < 10; ++enemyIdx)
                    setModelVanish(enemyIdx);
            }
        }
        break;
    }
    case 15: {
        //gBigAnimBlock[0].field_25 |= 2u;
        *byte_BFB2EC = 1;
        *byte_BE2C89 |= 2u;
        *byte_BE4775 |= 2u;
        ++summonEffecCtx->wordArray[0];
        if (!*dword_7C10D8)
            sub_429322();
        auto spellEffectIdx = getBattleModelState(getActionActorIdx())->animationEffect;
        auto modelName = getSummonModelNameFromEffectIdx(spellEffectIdx);
        std::vector<char> chars(modelName.begin(), modelName.end());
        chars.push_back(0);
        char* cModelName = &(chars[0]);
        auto summonModel = srSetSummonModel(6, cModelName);
        srLogWrite("LOADED IFRIT SUMMON MODEL AT: %p", summonModel);
        setSummonModel(summonModel, *dword_9ADFD8);
        //activatePlayableSummon(getActionActorIdx(), modelName);
        pushSummonEffectCallback(spellEffectIdx, getAnimatingActionTargetMask(), getActionActorIdx());
        convertSummonBData();
        *byte_BE6261 |= 0x10u;
        summonEffecCtx->handlerIndex = 0xFFFF;
        gContext.battleActors.swapPartyToSummon(spellEffectIdx); //Will need to be actual summon IDX later
        gContext.party.activateSummon(spellEffectIdx);
        break;
    }
    }
    ++summonEffecCtx->wordArray[0];
}

void playSummonAnimationScript() {
    u32** off_9ADFD8 = (u32**)0x9ADFD8;
    u32* gModelHolder = *off_9ADFD8;
    u8* unk_BE6261 = (u8*)0xBE6261;
    BattleModel** unk_BE6DE4 = (BattleModel**)0xBE6DE4;
    if (!(*unk_BE6261 & 2) && gModelHolder) {
        BattleModel* summonModel = (BattleModel*)gModelHolder[20];
        if (summonModel) {
            u8** scriptTable = (u8**)summonModel->animScriptStruct + (104u/4);
            *unk_BE6DE4 = (BattleModel*)gModelHolder[20];
            runAnimationScript(3, scriptTable);
        }
    }
}

void pushSummonEffectCallback(u8 animationEffectIdx, u16 targetMask, u8 actorIdx) {
    SRPFNSPELLEFFECTCALLBACK callback = nullptr;
    switch (animationEffectIdx) {
    case 0: {
        break;
    }
    case 1: {
        break;
    }
    case 2: {
        prepareIfrit(targetMask, actorIdx);
        break;
    }
    }
}

std::string getSummonModelNameFromEffectIdx(u8 animationEffectIdx) {
    switch (animationEffectIdx) {
    case 0: {
        return BattleModelNames::CHOCOMOG;
        break;
    }
    case 1: {
        return BattleModelNames::SHIVA;
        break;
    }
    case 2: {
        return BattleModelNames::IFRIT;
        break;
    }
    }
}
