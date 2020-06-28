#ifndef COMMON_WIDGETS_H
#define COMMON_WIDGETS_H

#include <SisterRay/SisterRay.h>
#include "../menus/menu_utils.h"
#include "../widgets/widget.h"

Widget* createCommandViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC commandNameViewUpdater, const char* menuName=nullptr, u8 cursorIdx=0);
Widget* allocateCommandRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u8* updatingActor);
void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u32* updatingActor);
void techniqueViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u32* updatingActor);
void techniqueViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u8* updatingActor);
void baseCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, const EnabledCommand*);
void updateCommandsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu=nullptr, u16 cursorState=0);
void resizeCommandBox(u8 actorID, Widget* boxWidget, Cursor* commandChoiceCursor=nullptr);

Widget* createActionViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC actionViewUpdater, const char* menuName, u16 cursorIdx);
Widget* allocateActionRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void updateActionsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu, u16 cursorState);
#endif // !COMMON_WIDGETS_H
