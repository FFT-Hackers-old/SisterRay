#include "command_sr_callbacks.h"
#include "damage_callbacks.h"
#include "../battle/scene_globals.h"
#include "../battle/engine/damage_events.h"
#include "../impl.h"
#include "status_names.h"
#include "element_names.h"
#include "damage_callback_utils.h"
#include "../battle/battle_utils.h"
#include <algorithm>

void srLoadAbilityData() {
    CommandSetupEvent srEvent = { gDamageContextPtr };
    srSetupAction(srEvent);
}

void srSetupWeapon(CommandSetupEvent& srDamageEvent) {
    auto damageContext = srDamageEvent.damageContext;
    auto weaponCtx = srDamageEvent.srDamageContext->attackerState.weaponCtx;
    auto srWeapon = srDamageEvent.srDamageContext->attackerState.srWeapon;
    damageContext->abilityPower = 16;
    if (damageContext->attackerID < 3) {
        damageContext->animationEffectID = weaponCtx->attackEffectID;
        srDamageEvent.srDamageContext->damageFormula = srWeapon->damageFormula;
        srDamageEvent.srDamageContext->dmgFormulaModifiers = srWeapon->dmgFormulaModifiers;
        srDamageEvent.srDamageContext->damageType = srWeapon->damageType;
        srDamageEvent.srDamageContext->hitFormula = srWeapon->hitFormula;
        srDamageEvent.srDamageContext->hitFormulaModifiers = srWeapon->hitFormulaModifiers;
        damageContext->abilityHitRate = srWeapon->gameWeapon.weaponHitRate;
        damageContext->impactEffectID = srWeapon->gameWeapon.impactEffectID;
        srDamageEvent.srDamageContext->attackElements = srWeapon->attackElements;
        srDamageEvent.srDamageContext->statusToInflict = srWeapon->statusAttack;
        if (damageContext->cameraDataSingle == 0xFFFF)
            damageContext->cameraDataSingle = srWeapon->gameWeapon.cameraMovement;
        if (damageContext->cameraDataMultiple == 0xFFFF)
            damageContext->cameraDataMultiple = srWeapon->gameWeapon.cameraMovement;
        damageContext->specialAbilityFlags = srWeapon->gameWeapon.specialAttackFlags;
        if (damageContext->abilityTargetingFlags == 0xFF) {
            damageContext->abilityTargetingFlags = srWeapon->gameWeapon.targetFlags;
        }

        damageContext->impactSound = srWeapon->gameWeapon.normalSoundID;
        damageContext->critAtkSound = srWeapon->gameWeapon.critSoundID;
        damageContext->missAtkSound = srWeapon->gameWeapon.missSoundID;
        //Elemental Materia will have to go here
    }

    if (srActorHasStatus(srDamageEvent.srDamageContext->attackerState, StatusNames::BLIND))
        damageContext->abilityHitRate >>= 1;

    if (srActorHasStatus(srDamageEvent.srDamageContext->attackerState, StatusNames::FROG)) {
        damageContext->impactSound = 363;
        damageContext->critAtkSound = 364;
        damageContext->missAtkSound = 5;
        damageContext->impactEffectID = 0;
    }
    if (srDamageEvent.srDamageContext->attackerState.actorTimers->currentHP  == 7777)    {

    }
}

#define handleCantReach ((PFNSR_VOIDSUB)0x5DA278)
#define createFinalTargetMask ((PFNSR_VOIDSUB)0x5DC390)
#define gameCopyEventQueuePositions ((PFNSR_VOIDSUB)0x436C30)
void srApplyDamage(CommandSetupEvent& srSetupEvent) {
    srLogWrite("INSIDE SR_APPLY_DAMAGE Callback");
    auto damageContext = srSetupEvent.damageContext;
    printAnimationEventQueue();
    srLogWrite("target mask: %x", damageContext->targetMask);
    auto& aiContext = *AI_BATTLE_CONTEXT;
    u32* G_ACTION_STATE_FLAGS = (u32*)0xC3F364;
    u16* word_C3F37C = (u16*)0xC3F37C;
    u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
    u16* G_TEMP_ATTACKER_TARGETS = (u16*)0xC3F368;
    u8* byte_C3F358 = (u8*)0xC3F358;

    *G_ACTION_STATE_FLAGS = 0;
    u32 localActionCount;
    if (!(damageContext->animationScriptID != -1))
        return;

    if (!(damageContext->displayString == -1)) {
        handleFailedAction(srSetupEvent, damageContext->displayString);
        return;
    }

    if (!(damageContext->commandIndexCopy > 32 || !didActionFail(srSetupEvent)))
        return;

    if (damageContext->supportMatFlags)
        setSupportMateriaFlags(srSetupEvent);

    damageContext->usedTargetMask = damageContext->targetMask;
    damageContext->abilityPowerCopy = damageContext->abilityPower;

    // Execute all follow up actions
    while (1) {
        srLogWrite("Entering Damage Calculation While Loop");
        localActionCount = damageContext->actionCount--;// do this doublecut param times
        if (!localActionCount)
            break;

        for (u32 followUpIdx = 0; followUpIdx < 8; ++followUpIdx) {
            if (damageContext->followUpActions[followUpIdx] != 255) {
                srLogWrite("Handling follow up action with idx: %i", followUpIdx);
                auto followUpID = damageContext->followUpActions[followUpIdx];
                damageContext->followUpActions[followUpIdx] = -1;
                damageContext->absAttackIndex = followUpID;
                damageContext->sceneAbilityIndex = damageContext->absAttackIndex;
                damageContext->abilityTargetingFlags = 0xFF;
                srSetupAction(srSetupEvent);
                //some special flag setting, investigate and replace bit packing trick
                /*if (followUpID & 0x80) {
                    damageContext->specialAbilityFlags &=0xDFu;
                }*/
                break;
            }
        }
        word_C3F37C[0] = 0;
        damageContext->targetMask = damageContext->usedTargetMask;
        damageContext->hitCountCopy = damageContext->attackHitCount;
        srLogWrite("Handling Targeting");
        handleTargeting(srSetupEvent); //Reverse/redo in the future
        srLogWrite("Targeting Handled");
        handleCantReach(); //Reverse/redo in the future
        srLogWrite("Handling Action Success");
        srLogWrite("target mask: %x", damageContext->targetMask);
        if (damageContext->miscActionFlags & 0x80000) {
            for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                if ((1 << actorIdx) & damageContext->targetMask) {
                    if (aiContext.actorAIStates[actorIdx].statusMask & 1)
                        damageContext->targetMask &= ~(1 << actorIdx); // unflag dead targets
                }
            }
            srLogWrite("Handled untarget any dead targets");
            if (!damageContext->targetMask) {
                srLogWrite("Pushing Special null target Anim Event");
                srPushSpecialAnimEvent(damageContext->animationScriptID, srSetupEvent);
                break;
            }
        }
        if (!damageContext->targetMask) {
            srLogWrite("No targets in mask, triggering failed action");
            handleFailedAction(srSetupEvent, damageContext->displayString);
            break;
        }
        srLogWrite("Action succceeded");
        // Why do this lol?
        for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
            G_TEMP_ATTACKER_TARGETS[actorIdx] = 0;
            byte_C3F358[actorIdx] = -1;
        }
        G_TEMP_ATTACKER_TARGETS[damageContext->attackerID] = damageContext->targetMask;
        *G_ACTION_STATE_FLAGS |= 9u;
        while (*G_ACTION_STATE_FLAGS & 1) {
            srLogWrite("Entering Target Damage Event While Loop");
            if (*G_ACTION_STATE_FLAGS & 6) {
                *G_ACTION_STATE_FLAGS &= 0xFFFFFFFD;
                if (damageContext->commandIndexCopy != 3)
                    damageContext->activeAllies = 3;
            }

            // I doubt this needs to be done this way
            for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                G_ATTACKER_TARGETS[actorIdx] = G_TEMP_ATTACKER_TARGETS[actorIdx];
                G_TEMP_ATTACKER_TARGETS[actorIdx] = 0;
            }

            for (u8 attackerActorID = 0; attackerActorID < 10; ++attackerActorID) {
                auto targetMask = G_ATTACKER_TARGETS[attackerActorID];
                if (targetMask) {
                    damageContext->currentUnitIDTempt = attackerActorID;
                    damageContext->incOnDamageDealt = 0;
                    damageContext->attackHitCount = damageContext->hitCountCopy;
                    gameCopyEventQueuePositions();
                    setupAttackerAnimEvent(srSetupEvent, targetMask);
                    do {
                        bool shouldRandomTarget = false;
                        damageContext->targetMask = targetMask;
                        if (damageContext->attackHitCount)
                            shouldRandomTarget = true;
                        if (damageContext->quadCount && (damageContext->abilityTargetingFlags & 0xC) != 4)
                            shouldRandomTarget = true;
                        if (damageContext->abilityTargetingFlags & 0x80)
                            shouldRandomTarget = true;
                        if (damageContext->commandIndexCopy == 3)
                            shouldRandomTarget = true;

                        if (shouldRandomTarget)
                            damageContext->targetMask = getRandomMaskBit(damageContext->targetMask);

                        for (u8 targetActorID = 0; targetActorID < 10; ++targetActorID) {
                            if ((1 << targetActorID) & damageContext->targetMask) {
                                srSetupEvent.srDamageContext->targetRow = srSetupEvent.aiContext->actorAIStates[targetActorID].actorRow;
       
                                DamageCalculationEvent srDamageEvent{ srSetupEvent.damageContext, srSetupEvent.srDamageContext, srSetupEvent.aiContext, newDamageEvent() };
                                srDamageEvent.srDamageContext->attackerState = gContext.battleActors.getActiveBattleActor(attackerActorID);
                                printDamageQueueState();
                                srLogWrite("Creating Damage Event with for attacker: %i, target: %i", attackerActorID, targetActorID);
                                calculateDamage(&srDamageEvent, attackerActorID, targetActorID);
                                srLogWrite("Damage Event created at %p", &srDamageEvent.gameDamageEvent);
                            }
                        }
                        if (damageContext->attackHitCount)
                            --damageContext->attackHitCount;

                    } while (damageContext->attackHitCount);// for every attackHitCount
                    newDamageEvent()->targetID = 0xFF;
                    gContext.eventBus.dispatch(POST_ACTION_DAMAGE_CALC, &srSetupEvent);
                }
            }
            srLogWrite("Exiting Damage Event Creation Loop");
            printDamageQueueState();
            handleDrainEffects(srSetupEvent);
            handleQuadraMagic(srSetupEvent);

            if (damageContext->miscActionFlags & 0x1000)
                damageContext->cameraDataSingle = -4;
            else
                damageContext->cameraDataSingle = -1;
            damageContext->cameraDataMultiple = damageContext->cameraDataSingle;
        }
        //Handles updating animation scripts, should be command level perhaps
        switch (damageContext->animationScriptID) {
        case 0x18:
            damageContext->animationScriptID = 25;
            break;
        case 0x19:
            damageContext->animationScriptID = 26;
            break;
        case 0x1A:
            damageContext->animationScriptID = 27;
            break;
        case 0x16:
            damageContext->animationScriptID = 23;
            damageContext->usedTargetMask = damageContext->targetMask;
            break;
        }
    }
    if (damageContext->miscActionFlags & 0x10000)
        srPushSpecialAnimEvent(71, srSetupEvent);

    if (damageContext->unkDword8 != -1) {
        auto animEvent = createAnimEvent(10, 2, 1, 0xFF, damageContext->unkDword8, 0xFF, 0, 0xFFFF);
        animEvent->damageEventQueueIdx = 0xFFFF;
    }
    srLogWrite("EXITING SRAPPLYDAMAGE CALLBACK");
    printDamageQueueState();
    printAnimationEventQueue();
 }

 void setupAttackerAnimEvent(CommandSetupEvent& setupEvent, u16 targetMask) {
     auto damageContext = setupEvent.damageContext;
     auto& aiContext = *setupEvent.aiContext;
     auto& attackerState = setupEvent.srDamageContext->attackerState;
     u8* byte_8FF1E8 = (u8*)0x8FF1E8;
     u8* byte_8FF1E9 = (u8*)0x8FF1E9;

     auto animEvent = newAnimEvent();
     printAnimationEvent(animEvent);
     printAnimationEventQueue();
     damageContext->targetCount = countTargets(targetMask);
     animEvent->attackerID = damageContext->attackerID;
     animEvent->activeAllies = damageContext->activeAllies;
     animEvent->animationScriptID = damageContext->animationScriptID;
     if (damageContext->attackerID >= 4 && srActorHasStatus(attackerState, StatusNames::FROG) && animEvent->animationScriptID != 52) {
         if (damageContext->commandIndexCopy != 32 || damageContext->animationEffectID != 0xFF) {
             if (damageContext->commandIndexCopy <= 32)
                 animEvent->animationScriptID = 29;
         }
         else {
             animEvent->animationScriptID = 20;
         }
     }
     if (damageContext->attackerID < 3 && (damageContext->targetCount > 1 || damageContext->miscActionFlags & 0x20000)) {
         for (u8 i = 0; i < 7; ++i) {
             if (animEvent->animationScriptID == byte_8FF1E8[2 * i]) {
                 animEvent->animationScriptID = byte_8FF1E9[2 * i];
                 break;
             }
         }
     }
     animEvent->commandIndex = damageContext->commandIndexCopy;
     animEvent->spellEffectID = damageContext->animationEffectID;
     if (damageContext->targetCount <= 1)
         animEvent->cameraData = damageContext->cameraDataSingle;
     else
         animEvent->cameraData = damageContext->cameraDataMultiple;
     animEvent->actionIndex = damageContext->sceneAbilityIndex;
     animEvent->specialDamageFlags = 0;
     printAnimationEvent(animEvent);
     printAnimationEventQueue();
}



 bool didActionFail(CommandSetupEvent& setupEvent) {
     bool cannotPerformAction; // [esp+Ch] [ebp-8h]
     bool didActionFail; // [esp+10h] [ebp-4h]

     auto damageContext = setupEvent.damageContext;
     auto& aiContext = *setupEvent.aiContext;
     auto& attackerState = setupEvent.srDamageContext->attackerState;

     didActionFail = false;
     if (!(srActorHasStatus(attackerState, StatusNames::STOP)) && damageContext->animationScriptID != 52) {
         cannotPerformAction = false;
         if (srActorHasStatus(attackerState, StatusNames::SILENCE)) {
             switch (damageContext->commandIndexCopy) {
             case 2:
             case 3:
             case 0xD:
             case 0x15:
             case 0x16:
                 cannotPerformAction = true;
                 break;
             case 0x20:
                 if (damageContext->MPCost)
                     cannotPerformAction = true;
                 break;
             default:
                 break;
             }
         }
         if (srActorHasStatus(attackerState, StatusNames::FROG)) {
             switch (damageContext->commandIndexCopy)
             {
             case 1:
             case 4:
                 break;
             case 2:
             case 0x15:
                 if (damageContext->absAttackIndex != 10)
                     cannotPerformAction = true;
                 break;
             case 0x20:
                 if (damageContext->MPCost)
                     cannotPerformAction = true;
                 break;
             default:
                 cannotPerformAction = true;
                 break;
             }
         }
         if (cannotPerformAction) {
             handleFailedAction(setupEvent, -1);
             didActionFail = true;
         }
         else if (attackerState.actorBattleVars->currentMP < damageContext->MPCost) {
             handleFailedAction(setupEvent, (damageContext->attackerID >= 3) + 91);
             didActionFail = true;
         }
         else {
             attackerState.actorBattleVars->currentMP -= damageContext->MPCost;
         }
     }
     damageContext->MPCost = 0;
     return didActionFail;
 }

 void setSupportMateriaFlags(CommandSetupEvent& srSetupEvent) {
     auto damageContext = srSetupEvent.damageContext;
     u8 supportMateriaFlags = srSetupEvent.damageContext->supportMatFlags;
     if (supportMateriaFlags & 1) {
         damageContext->miscActionFlags |= 0x80;
     }
     if (supportMateriaFlags & 2) {
         damageContext->miscActionFlags |= 0x40;
     }
     if (supportMateriaFlags & 8)
         damageContext->miscActionFlags |= 4u;
     if (supportMateriaFlags & 0x10) {
         damageContext->miscActionFlags |= 8u;
     }
     if (supportMateriaFlags & 0xE0)
         damageContext->MPTurboBoost = 10 * (supportMateriaFlags >> 5);
 }


 void handleFailedAction(CommandSetupEvent& srSetupEvent, u16 strIdx) {
     auto damageContext = srSetupEvent.damageContext;
     auto& aiContext = *srSetupEvent.aiContext;
     u16 stringArgs[2]; // [esp+4h] [ebp-8h]
     int animScriptID; // [esp+8h] [ebp-4h]
     srLogWrite("Handling Failed Action, Insite Callback");
     //TODO modularize this as part of the command object
     animScriptID = 3;
     switch (damageContext->commandIndexCopy) {
     case 2:
         animScriptID = 56;
         break;
     case 3:
         animScriptID = 54;
         break;
     case 0xD:
         animScriptID = 55;
         break;
     case 0x14:
         animScriptID = 53;
         break;
     default:
         break;
     }

     const auto& attackerState = srSetupEvent.srDamageContext->attackerState;
     if (damageContext->attackerID >= 3) {
         srLogWrite("Failed case: NON-PLAYER Actor");
         if (strIdx != 0xFFFF) {
             stringArgs[0] = damageContext->attackerID;
             stringArgs[1] = -1;
             if (attackerState.actorTimers->field_F != 0xFF)
                 stringArgs[1] = attackerState.actorTimers->field_F;
             srLogWrite("Pushing Display string event with arguments: %x, %x, stringIdx: %i", stringArgs[0], stringArgs[1], strIdx);
             pushDisplayString(damageContext->attackerID, strIdx, 1, (u32*)&stringArgs);
         }
     }
     else {
         srLogWrite("Failed case: PLAYER Actor");
         srPushSpecialAnimEvent(animScriptID, srSetupEvent);
         if (strIdx != -1) {
             auto animEvent = createAnimEvent(10, 2, 1, 0xFF, strIdx, 0xFF, 0, 0xFFFF);
             animEvent->damageEventQueueIdx = 0xFFFF;
             srPushSpecialAnimEvent(59, srSetupEvent);
             srLogWrite("Special Event Pushed");
         }
         if (animScriptID == 3) {
             srPushSpecialAnimEvent(4, srSetupEvent);
         }
         
     }
     srLogWrite("returning from callback");
 }

 void srPushSpecialAnimEvent(u16 animationScriptID, CommandSetupEvent& srSetupEvent) {
     auto damageContext = srSetupEvent.damageContext;
     auto animEvent = createAnimEvent(
         damageContext->attackerID,
         1,
         animationScriptID,
         damageContext->commandIndexCopy,
         damageContext->sceneAbilityIndex,
         damageContext->animationEffectID,
         0,
         0xFFFF
     );
     auto damageEvent = newDamageEvent();
     damageEvent->targetID = damageContext->attackerID;
     damageEvent->attackerID = damageContext->attackerID;
     damageEvent->damagedAnimScriptIdx = 51;
     damageEvent->specialDamageFlags = 0;
     damageEvent->targetStatusMask = 0;
     auto terminatorEvent = newDamageEvent();
     terminatorEvent->targetID = 0xFF;
 }

 void handleTargeting(CommandSetupEvent& srSetupEvent) {
     u16 validRowTargets; 
     u16 validCanTargetMask; 
     bool targetOppositeRow; 
     bool pickValidTarget; 
     signed int v6; 
     u16 possibleTargets; 
     int v9; 
     u16 validTargetsMask; 
     u16* word_9A8894 = (u16*)0x9A8894;
     u16* word_9A88A2 = (u16*)0x9A88A2;
     u16* word_9A8896 = (u16*)0x9A8896;
     u16* word_9A88AA = (u16*)0x9A88AA;
     u16* word_9A88AE = (u16*)0x9A88AE;
     u16* word_9A8898 = (u16*)0x9A8898;
     u16* G_ESCAPED_ACTORS_MASK = (u16*)0x9AAD24;

     auto damageContext = srSetupEvent.damageContext;
     auto& aiContext = *srSetupEvent.aiContext;

     u8 targetFlags = damageContext->abilityTargetingFlags;
     if (damageContext->miscActionFlags & 0x1000000) {
         possibleTargets = *word_9A88A2 | *word_9A8896;
     }
     else {
         possibleTargets = (*word_9A8894 & *word_9A8896);
         if (!(damageContext->specialAbilityFlags & 0x100) || !(damageContext->specialAbilityFlags & 0x800))
             possibleTargets |= (~*G_ESCAPED_ACTORS_MASK & *word_9A88A2);
     }
     validTargetsMask = damageContext->targetMask & (possibleTargets ^ damageContext->targetMask);
     if (validTargetsMask) {
         for ( u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
             if ((1 << actorIdx) & validTargetsMask) {
                 auto actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
                 if (actorState.actorTimers->field_C != 0xFF)
                     damageContext->targetMask |= (1 << actorState.actorTimers->field_C);
             }
         }
         srLogWrite("calculated valid targetMask: %x", damageContext->targetMask);
     }
     if (targetFlags) {
         bool confusionActive = srActorHasStatus(srSetupEvent.srDamageContext->attackerState, StatusNames::CONFUSION);
         bool manipulateActive = srActorHasStatus(srSetupEvent.srDamageContext->attackerState, StatusNames::MANIPULATE);
         if (targetFlags & TGT_FLAG_ALL_ROWS) {
             damageContext->targetMask = (~(*word_9A88A2) & *word_9A8894);
             srLogWrite("calculated all row targetMask: %x", damageContext->targetMask);
         }
         else {
             v6 = 0;
             if (targetFlags & TGT_FLAG_START_ENEMIES) {
                 if (damageContext->attackerID < 4u)
                     validRowTargets = 15u;
                 else
                     validRowTargets = 1008u;
             }
             else {
                 if (damageContext->attackerID < 4u)
                     validRowTargets = 1008u;
                 else
                     validRowTargets = 15u;
             }

             u8 cmdIdx = damageContext->commandIndexCopy;
             if (cmdIdx != CMD_SUMMON && cmdIdx != CMD_LIMIT) {
                 if (confusionActive && manipulateActive)
                     validRowTargets = (~validRowTargets) & 0x3FF;
                 if (confusionActive) {
                     damageContext->targetMask = validRowTargets;
                     if (!(damageContext->miscActionFlags & 0x200))
                         damageContext->targetMask = getRandomMaskBit(damageContext->targetMask);
                 }
             }

             if ((targetFlags & TGT_FLAG_TOGGLE_MULTIPLE & TGT_FLAG_START_MULTIPLE) == 4)
                 v6 = 1;
             if (damageContext->quadCount) {
                 v6 = 1;
                 targetFlags = targetFlags | TGT_FLAG_START_MULTIPLE;
             }
             if (targetFlags & TGT_FLAG_ONE_ROW) {
                 possibleTargets &= validRowTargets;
             }
             else if (targetFlags & TGT_SHORT_RANGE) {
                 possibleTargets = (*word_9A8898 & possibleTargets);
             }
             damageContext->targetMask &= possibleTargets;
             srLogWrite("Target Mask after short range/one row restrictions: %x", damageContext->targetMask);
             validCanTargetMask = possibleTargets & validRowTargets;
             if (!(damageContext->miscActionFlags & 0x200000)) {
                 if (damageContext->targetMask && countTargets(damageContext->finalTargetMask) > 1 || v6) {
                     v9 = ((damageContext->targetMask & 0xF) != 0 ? 15 : 1008) & possibleTargets;
                     if (!(*word_9A88AA & damageContext->targetMask))
                         v9 &= ~(*word_9A88AA);
                     if (!(*word_9A88AE & damageContext->targetMask))
                         v9 &= ~(*word_9A88AE);
                     damageContext->targetMask = v9;
                 }
                 if (!damageContext->targetMask) {
                     if (targetFlags & TGT_FLAG_START_MULTIPLE && targetFlags & TGT_FLAG_TOGGLE_MULTIPLE && !(damageContext->miscActionFlags & 0x200))
                         validCanTargetMask = getRandomMaskBit(validCanTargetMask);
                     damageContext->targetMask = validCanTargetMask;
                 }
                 if (!(targetFlags & TGT_FLAG_START_MULTIPLE) || (damageContext->miscActionFlags & 0x100200) == 1049088)
                     damageContext->targetMask = getRandomMaskBit(damageContext->targetMask);
             }

             if (*word_9A88AA & damageContext->targetMask && *word_9A88AE & damageContext->targetMask) {
                 u8 v0 = getRandomNumber();
                 damageContext->targetMask &= word_9A88AA[v0 & 2];
                 damageContext->usedTargetMask &= word_9A88AA[v0 & 2];
             }
             srLogWrite("calculated targetFlags targetMask: %x", damageContext->targetMask);
         }
     }
     else {
         damageContext->targetMask = 1 << damageContext->attackerID;
     }
     if (!damageContext->finalTargetMask)
         damageContext->finalTargetMask = damageContext->targetMask;
 }

 void handleQuadraMagic(CommandSetupEvent& srSetupEvent) {
     auto damageContext = srSetupEvent.damageContext;
     auto& aiContext = *srSetupEvent.aiContext;
     u32* G_ACTION_STATE_FLAGS = (u32*)0xC3F364;
     u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
     u16* G_TEMP_ATTACKER_TARGETS = (u16*)0xC3F368;
     u8* byte_C3F358 = (u8*)0xC3F358;
     u16* word_9A8894 = (u16*)0x9A8894;

     if (!(*G_ACTION_STATE_FLAGS & 2)) {
         if (damageContext->quadCount)
             --damageContext->quadCount;

         if (damageContext->quadCount) {
             u16 targetMask = damageContext->targetMask;
             if (damageContext->commandIndexCopy == 3)
                 targetMask = *word_9A8894 & 0x3F0;
             // Don't target anything that is dead
             if (targetMask) {
                 for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                     auto actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
                     auto battleVars = srSetupEvent.aiContext->actorAIStates[actorIdx];
                     if (srActorHasStatus(actorState, StatusNames::DEATH));
                         targetMask &= ~(1 << actorIdx);
                 }
                 G_TEMP_ATTACKER_TARGETS[damageContext->attackerID] = targetMask;
                 if (!targetMask)
                     damageContext->quadCount = 0;
             }
         }
         if (damageContext->quadCount) {
             *G_ACTION_STATE_FLAGS |= 4;
         }
         else {
             *G_ACTION_STATE_FLAGS &= 0xFFFFFFFE;
         }
     }
     // End Quadra Magic
 }

 void handleDrainEffects(CommandSetupEvent& srSetupEvent) {
     for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
         const auto& actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
         if (actorState.actorTimers->drainedHP)
             handleActorDrain(srSetupEvent, actorIdx, false);
         if (actorState.actorTimers->drainedMP)
             handleActorDrain(srSetupEvent, actorIdx, true);
     }
 }

 void handleActorDrain(CommandSetupEvent& srSetupEvent, u8 actorIdx, bool drainMP) {
     auto& aiContext = *srSetupEvent.aiContext;
     auto& damageCtx = *srSetupEvent.damageContext;
     auto& actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
     bool isDead = srActorHasStatus(actorState, StatusNames::DEATH);
     if (!isDead) {
         auto animationEvent = createAnimEvent(actorIdx, 1, 46, 0, 0, 0, 0, 0xFFFF);
         auto damageFlagLocal = 1;
         auto drainedHP = 0;
         if (!drainMP) {
             drainedHP = actorState.actorTimers->drainedHP;
             actorState.actorTimers->drainedHP = 0;
         }
         else{
             drainedHP = actorState.actorTimers->drainedMP;
             actorState.actorTimers->drainedMP = 0;
         }
         damageCtx.targetID = actorIdx;
         damageCtx.abilityFlags2 = drainMP != false ? 4 : 0;
         if (drainedHP < 0) {
             drainedHP = -drainedHP;
             damageCtx.abilityFlags2 |= 1;
         }
         bool isMagic = damageCtx.specialAbilityFlags & 4;
         if (actorIsDamageImmune(aiContext.actorAIStates[actorIdx], actorState, isMagic))
             drainedHP = 0;
         damageCtx.currentDamage = drainedHP;

         auto damageEvent = newDamageEvent();
         DamageCalculationEvent srDamageEvent{srSetupEvent.damageContext, srSetupEvent.srDamageContext, srSetupEvent.aiContext, damageEvent};
         handleDamage(&srDamageEvent);
         if (srActorHasStatus(actorState, StatusNames::DEATH)) {
             damageFlagLocal = 5;
             damageCtx.wasKilledMask |= 1 << actorIdx;
         }
         damageEvent->targetID = actorIdx;
         damageEvent->attackerID = actorIdx;
         damageEvent->damagedAnimScriptIdx = 46;
         damageEvent->specialDamageFlags = damageFlagLocal;
         damageEvent->targetStatusMask = aiContext.actorAIStates[actorIdx].statusMask;
         createImpactEvent(damageEvent, damageCtx.currentDamage, damageCtx.abilityFlags2, -1, -1,
             aiContext.actorAIStates[actorIdx].currentHP, aiContext.actorAIStates[actorIdx].currentHP);
         damageEvent->targetID = 0xFF;
     }
 }


void srSetupAction(CommandSetupEvent& srSetupEvent) {
    i32 sceneAbilityIndex;
    i32 enemyActionIndex;
    u16 elementMask;
    u8 characterID;

    auto deathSentenceFlag = 0;
    u16 cameraOverrideData = 0xFFFF;
    auto damageContext = srSetupEvent.damageContext;
    const SrAttack& action = getCommandAction(damageContext->commandIndex, damageContext->relAttackIndex);
    const AttackData& abilityData = action.attackData;
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);
    AttackData* currentSceneAbilities = (AttackData*)(0x9A90C4);
    char* attackNamesPtr = (char*)(0x9A9484);
    u16* currentSceneAbilityIDs = (u16*)(0x9A9444);

    damageContext->MPCost = -1;
    srSetupEvent.srDamageContext->attackStats = action.attackStats;
    srSetupEvent.srDamageContext->statusToInflict = action.statusAttack;
    srLogWrite("action has %i statuses to inflict", action.statusAttack.size());
    for (const auto& infliction : action.statusAttack) {
        srLogWrite("ACTION WILL ATTEMPT TO INFLICT STATUS: %s", infliction.statusName);
    }
    srSetupEvent.srDamageContext->attackElements = action.attackElements;
    srSetupEvent.srDamageContext->useActionDamageLimits = action.useOverrideLimits;
    srSetupEvent.srDamageContext->hpDamageLimit = action.hpDamageLimit;
    srSetupEvent.srDamageContext->mpDamageLimit = action.mpDamageLimit;

    switch (damageContext->commandIndexCopy) {
        case 20: {
            if (damageContext->attackerID < 3)
                initializeLimitContext(damageContext);
            break;
        }
        case 32: {
            currentSceneAbilities[0] = abilityData;
            *currentSceneAbilityIDs = action.attackID;
            damageContext->sceneAbilityIndex = action.attackID;
            break;
        }
        default: {
            break;
        }
    }
    if (damageContext->commandIndex == CMD_MAGIC && damageContext->relAttackIndex == 54) //death sentence case hardcoded
        deathSentenceFlag = 1;

    if (!((damageContext->miscActionFlags) & 0x400000)) {
        if (damageContext->attackerID < 3) {
            EnabledSpell* spellData = getSpellSlot(damageContext->attackerID, damageContext->commandIndexCopy, damageContext->relAttackIndex);
            if (spellData)
                updatePlayerSpellData(damageContext, spellData, abilityData);
        }
    }

    if (deathSentenceFlag) {
        damageContext->animationScriptID = 52;
    }
    else if (damageContext->attackerID >= 4) {
        const auto& actingEnemyRecord = *getInBattleActorEnemyData(damageContext->attackerID - 4); //Finds the "formation data" for the current actor
        for (enemyActionIndex = 0; enemyActionIndex < 16; ++enemyActionIndex) {
            if (damageContext->absAttackIndex == actingEnemyRecord.attackSceneIDs[enemyActionIndex]) { //offset into the actual actor IDs
                damageContext->animationScriptID = actingEnemyRecord.attackAnimScripts[enemyActionIndex]; //offset into the animation script indexes for this model
                cameraOverrideData = actingEnemyRecord.attackCameraIDs[enemyActionIndex];
                break;
            }
        }
    }

    if (damageContext->MPCost = -1)
        damageContext->MPCost = abilityData.MPCost;

    if (executingAction.entryPriority == 3 || damageContext->miscActionFlags & 0x400000) //This is triggered by mime
        damageContext->MPCost = 0;

    damageContext->abilityHitRate = abilityData.abilityHitRate;
    damageContext->damageFormulaID = abilityData.damageFormula;

    srSetupEvent.srDamageContext->damageFormula = action.damageFormula;
    srSetupEvent.srDamageContext->dmgFormulaModifiers = action.dmgFormulaModifiers;
    srSetupEvent.srDamageContext->hitFormula = action.hitFormula;
    srSetupEvent.srDamageContext->hitFormulaModifiers = action.hitFormulaModifiers;
    srSetupEvent.srDamageContext->damageType = action.damageType;

    if (abilityData.elementMask == 0xFFFF)
        elementMask = 0;
    else
        elementMask = abilityData.elementMask;

    damageContext->attackElementsMask = elementMask;
    damageContext->abilityPower = abilityData.attackPower;
    damageContext->targetReactionAnimation = abilityData.targetReactionID;

    damageContext->abilityTargetingFlags = abilityData.targetingFlags;

    if (damageContext->commandIndexCopy == CMD_LIMIT)
        cameraOverrideData = (((*(u16*)0x9AB0CC) & 8) != 0) ? abilityData.cameraMovementMultiple : abilityData.cameraMovementSingle;

    if (cameraOverrideData == 0xFFFF) {
        damageContext->cameraDataSingle = abilityData.cameraMovementSingle;
        damageContext->cameraDataMultiple = abilityData.cameraMovementMultiple;
    }
    else {
        damageContext->cameraDataSingle = cameraOverrideData;
        damageContext->cameraDataMultiple = cameraOverrideData;
    }

    if (abilityData.impactEffectID != 255)
        damageContext->impactEffectID = abilityData.impactEffectID;
    damageContext->animationEffectID = abilityData.animationEffectID;
    damageContext->specialAbilityFlags = abilityData.specialAttackFlags;
    damageContext->impactSound = abilityData.impactSoundID;
    damageContext->critAtkSound = abilityData.impactSoundID;
    damageContext->missAtkSound = abilityData.impactSoundID;

    if (!((damageContext->specialAbilityFlags) & 4) && gAiActorVariables[damageContext->attackerID].statusMask & 0x4000000)
        damageContext->abilityHitRate >>= 1; //(half hit chance)
    setStatusInflictionData(damageContext, abilityData.statusInflictType, abilityData.statusMask);

    if (deathSentenceFlag)
        gAiActorVariables[damageContext->attackerID].statusMask &= 0xFFDFFFFF;
    copyAdditionalEffects(abilityData.additionalEffect, abilityData.additionalEffectModifier);
}

//Kill this during Limit Break reimplementation
void initializeLimitContext(DamageCalcStruct* damageContext) {
    u8 characterID;
    u8* activeLimitIDs;
    u8* kernelLimitScriptIndexes = (u8*)(0x7B76A0);
    u8* unkPartyStructPtr = (u8*)(0x9A87F4);

    characterID = unkPartyStructPtr[16 * damageContext->attackerID];
    activeLimitIDs = getGamePartyMember(damageContext->attackerID)->enabledLimitBytes;
    if (damageContext->absAttackIndex >= 96 && damageContext->absAttackIndex <= 128) {
        auto scriptIndex = damageContext->absAttackIndex - 96;
        if (kernelLimitScriptIndexes[scriptIndex] != 255)
            damageContext->animationScriptID = kernelLimitScriptIndexes[scriptIndex];
        damageContext->enabledMagicsIndex = -1;
        return;
    }
    for (i32 limitIndex = 0; limitIndex < 3; ++limitIndex) {
        auto relativeLimitIndex = (i8)activeLimitIDs[limitIndex];
        if (relativeLimitIndex + 128 == damageContext->absAttackIndex) {
            auto limitAnimationScriptIndex = 0;
            for (auto charLimitArrayIndex = 0; charLimitArrayIndex < 12; ++charLimitArrayIndex) {
                auto fetchedRelativeIndex = getLimitRelativeIndex(characterID, charLimitArrayIndex);
                if (fetchedRelativeIndex != 127) {
                    if (fetchedRelativeIndex == relativeLimitIndex)
                        break;
                    ++limitAnimationScriptIndex;
                }
            }
            damageContext->animationScriptID = limitAnimationScriptIndex + 60;
            break;
        }
    }
}

void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData) {
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);

    damageContext->MPCost = spellData->mpCost;
    if ((executingAction.entryPriority >= 5) && !(damageContext->miscActionFlags & 0x400000)) //priority 5 and 6 actions? what are those
        damageContext->supportMatFlags = spellData->supportEffectsMask;
    srLogWrite("Setting MP Cost for player spell to: %i", damageContext->MPCost);
    if (spellData->quadCount && spellData->quadEnabled) {
        spellData->quadCount = spellData->quadCount - 1;
        damageContext->quadEnabled = spellData->quadEnabled;// quadEnabled?
        damageContext->quadCount = damageContext->quadEnabled + 3;  // numberOfCasts?
        if (damageContext->quadCount > 8) {
            damageContext->quadCount = 8;
        }
        srCreateEvent(2, damageContext->attackerID, 21, 6);
    }
    else if (damageContext->commandIndexCopy == CMD_SUMMON) {
        if (spellData->allCount) { //if yo ucan still use the summon
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 4);
            }
        }
        else {
            damageContext->displayString = 121; //Probably displays "Summon Power is all used up"
        }
    }
    else if (damageContext->miscActionFlags & 0x200) { //Handle the all case
        if (spellData->allCount) {
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 2);
            }
        }
        else if (abilityData.targetingFlags & 8) {
            damageContext->miscActionFlags |= 0x100000u;
        }
    }
}

void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask) {
    u16* word_9AAD1E = (u16*)(0x9AAD1E);
    damageContext->addStatusMask = 0;
    damageContext->rmStatusMask = 0;
    damageContext->toggleStatusMask = 0;
    auto statusType = statusInflictionByte >> 6;
    if (statusType < 3) {
        if (inflictedStatusMask >= 0) {
            switch (statusType) {
                case 0:
                    damageContext->addStatusMask = inflictedStatusMask;
                    break;
                case 1:
                    damageContext->rmStatusMask = inflictedStatusMask;
                    break;
                case 2:
                    damageContext->toggleStatusMask = inflictedStatusMask;
                    break;
                default: {
                    break;
                }
            }
        }
        else {
            damageContext->addStatusMask = 2147483648; //Figure out why this gets set
            *word_9AAD1E = (i8)inflictedStatusMask & 3;
        }
    }
}
