#ifndef CONTROLLABLE_SUMMON_H
#define CONTROLLABLE_SUMMON_H

#include <SisterRay/SisterRay.h>
#include <string>


namespace InfernoWidgetNames {
    const std::string INFERNO_WIDGET_NAME = "INFERNO_WIDGET";
    const std::string INFERNO_BOX_NAME = "INFERNO_BOX";
    const std::string INFERNO_GRID_NAME = "INFERNO_GRID";
}

void loadControllableSummon();
OpCodeControlSequence activatePlayableSummon(AnimScriptEvent* srEvent);
void initializeIfrit(InitSummonEvent* summEvent);
void loadPlayableSummons();
void cmdInfernoSelectHandler(SelectCommandEvent* menuEvent);
void cmdRavageSelectHandler(SelectCommandEvent* menuEvent);
void loadPlayableSummonAnimScripts();
void loadPlayableSummonAnimations();
void initIfritMenu();
void initInfernoViewWidget(const MenuInitEvent* menuEvent);
Widget* allocateInfernoRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void infernoCommandUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void drawInfernoViewWidget(const MenuDrawEvent* event);
void handleSelectInfernoAction(const MenuInputEvent* event);
#endif // !CONTROLLABLE_SUMMON_H
