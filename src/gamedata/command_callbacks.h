#ifndef COMMAND_CALLBACKS_H
#define COMMAND_CALLBACKS_H

#include <SisterRay/SisterRay.h>

typedef void(*SRPFNCOMMANDSETUP)(CommandSetupEvent&);
typedef void(*FF7PFNCOMMANDSETUP)();

void applyDamage(CommandSetupEvent& event);
void weaponSetup(CommandSetupEvent& event);
void loadAbility(CommandSetupEvent& event);
void setupItem(CommandSetupEvent& event);
void setupMove(CommandSetupEvent& event);
void setupLimit(CommandSetupEvent& event);
void setupSense(CommandSetupEvent& event);
void setupMime(CommandSetupEvent& event);
void setupFlash(CommandSetupEvent& event);
void setupThrow(CommandSetupEvent& event);
void setupManipulate(CommandSetupEvent& event);
void setupDeathblow(CommandSetupEvent& event);
void setupMorph(CommandSetupEvent& event);
void setupCoin(CommandSetupEvent& event);
void setupSteal(CommandSetupEvent& event);
void setupDoubleCut(CommandSetupEvent& event);
void setupQuadCut(CommandSetupEvent& event);
void setupPoison(CommandSetupEvent& event);
void setupEnemyAttack(CommandSetupEvent& setupEvent);
void createStringEvent(CommandSetupEvent& setupEvent);

#endif // !COMMAND_CALLBACKS_H
