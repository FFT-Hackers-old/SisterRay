#include "command_callbacks.h"
#include "command_sr_callbacks.h"

#define ff7ApplyDamage   ((FF7PFNCOMMANDSETUP)0x5D9940)
#define ff7weaponSetup   ((FF7PFNCOMMANDSETUP)0x5C9C64)

//The following callbacks are shared between many commands
void applyDamage(CommandSetupEvent event) {
    ff7ApplyDamage();
}

void weaponSetup(CommandSetupEvent event) {
    ff7weaponSetup();
}

void loadAbility(CommandSetupEvent event) {
    srSetupAction(event);
}

//The following callbacks are primarily used to setup specific commands


#define ff7setupItem            ((FF7PFNCOMMANDSETUP)0x05C9B6C)
#define ff7SetItemThrowType    ((FF7PFNCOMMANDSETUP)0x05C8FDF)

void setupItem(CommandSetupEvent event) {
    ff7SetItemThrowType();
    ff7setupItem();
}


#define ff7ForceSelfTarget     ((FF7PFNCOMMANDSETUP)0x5C94AB)
#define ff7SetupRowChange      ((FF7PFNCOMMANDSETUP)0x5C92A7)
#define ff7PushMoveEvents      ((FF7PFNCOMMANDSETUP)0x5C9E8D)
void setupMove(CommandSetupEvent event) {
    ff7SetupRowChange();
    ff7ForceSelfTarget();
    ff7PushMoveEvents();

}

#define ff7SetupLimit          ((FF7PFNCOMMANDSETUP)0x5C930F)
void setupLimit(CommandSetupEvent event) {
    ff7SetupLimit();
}

#define ff7SetupSense          ((FF7PFNCOMMANDSETUP)0x5C94A1)
#define ff7PushSenseStrings    ((FF7PFNCOMMANDSETUP)0x5CA093)
void setupSense(CommandSetupEvent event) {
    ff7SetupSense();
    ff7PushSenseStrings();
}

#define ff7SetupMime (((FF7PFNCOMMANDSETUP)0x5CA465)
void setupMime(CommandSetupEvent event) {
    ff7SetupMime();
}

#define ff7SetupFlash ((FF7PFNCOMMANDSETUP)0x5CA651)
void setupFlash(CommandSetupEvent event) {
    ff7SetupFlash();
}

#define ff7SetupThrow ((FF7PFNCOMMANDSETUP)0x5C904D)
void setupThrow(CommandSetupEvent event) {
    ff7SetupThrow();
}

#define ff7SetupManipulate ((FF7PFNCOMMANDSETUP)0x5C9467)
void setupManipulate(CommandSetupEvent event) {
    ff7SetupManipulate();
}

#define ff7SetupDeathblow  ((FF7PFNCOMMANDSETUP)0x5CA6A0)
void setupDeathblow(CommandSetupEvent event) {
    ff7SetupDeathblow();
}

#define ff7SetupMorph      ((FF7PFNCOMMANDSETUP)0x5CA6D2)
void setupMorph(CommandSetupEvent event) {
    ff7SetupMorph();
}

#define ff7SetupCoin       ((FF7PFNCOMMANDSETUP)0x5C9150)
void setupCoin(CommandSetupEvent event) {
    ff7SetupCoin();
}

#define ff7SetupSteal      ((FF7PFNCOMMANDSETUP)0x5CA6E3)
void setupSteal(CommandSetupEvent event) {
    ff7SetupSteal();
}

#define ff7SetupDoubleCut  ((FF7PFNCOMMANDSETUP)0x5C9FAC)
void setupDoubleCut(CommandSetupEvent event) {
    ff7SetupDoubleCut();
}

#define ff7SetupQuadCut    ((FF7PFNCOMMANDSETUP)0x5C9F46)
void setupQuadCut(CommandSetupEvent event) {
    ff7SetupQuadCut();
}

#define ff7SetupPoison     ((FF7PFNCOMMANDSETUP)0x5C9FC0)
void setupPoison(CommandSetupEvent event) {
    ff7SetupPoison();
}
