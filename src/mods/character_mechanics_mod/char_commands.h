#ifndef CHARACTER_MOD_H
#define CHARACTER_MOD_H

#include <SisterRay/SisterRay.h>
#include <string>
#include <vector>
struct InputBuffer_ {
    std::vector<u32> inputBuffer;
};

extern bool punisherActive;
extern bool doSwitchStance;
extern bool mashinNoob;
extern bool isCombohandled;
extern bool doCounter;
extern u8 punisherCounterTarget;

namespace CloudCommands{
    const u16 STANCE_SWITCH = 14;
}

namespace SoldierWidgetNames {
    const std::string SOLDIER_WIDGET_NAME = "SOLDIER_WIDGET";
    const std::string SOLDIER_BOX_NAME = "SOLDIER_BOX";
    const std::string SOLDIER_GRID_NAME = "SOLDIER_GRID";
}

struct ComboContext {
    u8 hitsSoFar;
    u16 lastMainTarget; //Mask of the actor attacked on the last action not including cleave;
    u16 initialTargetMask;
    u16 lastTargetMask;
    u8 lastCommandIdx;
    u16 lastActionIdx;
};

void enableSoldier(const EnableDefaultAbilitiesEvent* const event);
void loadCharMod();
void initCloudMenu();
void loadCloudAnimations();
void loadCloudActions();
void initializeLimits();
void initSoldierCommandViewWidget(const MenuInitEvent* event);
Widget* allocateSoldierRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void soldierCommandUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void drawSoldierViewWidget(const MenuDrawEvent* event);
void cmdSoldierelectHandler(SelectCommandEvent* event);
void handleSelectSoldierAction(const MenuInputEvent* event);
void cmdSwordplayelectHandler(SelectCommandEvent* menuEvent);
void limitSwapUpdater(const MenuInputEvent* event);
void stanceSwapCallback(const ModelAnimationEvent* modelAnimEvent);
void handlePunisherSwap(const ModelAnimationEvent* modelAnimEvent);
OpCodeControlSequence playStanceAnimation(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeCombo(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeSetComboWaitFrames(AnimScriptEvent* srEvent);
void useStanceHurtScript(DamageCalculationEvent* initActorEvent);
void setActorWait(const AnimScriptEvent* modelAnimEvent);
void triggerCounter(const ModelAnimationEvent* modelAnimEvent);
void executeCounter(const ModelAnimationEvent* modelAnimEvent);
void handlePunisherNoEvade(DamageCalculationEvent* srDmgCalcEvent);
OpCodeControlSequence handlePunisherSwap(AnimScriptEvent* modelAnimEvent);
void HandleCloudCombo(ComboEvent* srComboEvent);
OpCodeControlSequence ComboJumpOpcode(AnimScriptEvent* srAnimEvent);
OpCodeControlSequence ComboIdle(AnimScriptEvent* srEvent);
void smoothCombo(ModelAnimationEvent* animEvent);
void setLingeringAnims(AnimScriptEvent* animEvent);
void swapStance(CommandSetupEvent& event);
bool handleStanceChange(ComboEvent* srComboEvent);
bool handleNormalSwordplay(ComboEvent* srComboEvent, bool wasHeld = false);
void cloudInitActors(InitBattleActorEvent* initActorEvent);
void setActionInitialTargets(void* nullEvent);
bool srParseInputHoldInBuffer(InputBuffer* buffer, u32 comboMask);
void unsetCombOnIdle(AnimScriptEvent* srAnimEvent);

#endif
