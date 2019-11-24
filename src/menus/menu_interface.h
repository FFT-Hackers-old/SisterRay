#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API void drawCursor(Cursor* cursor, float priority);
SISTERRAY_API void drawFlashingCursor(Cursor* cursor, u32 stateMask, float priority);
SISTERRAY_API void moveCursor(Cursor* cursor, i32 xCoordinate, i32 yCoordinate);
SISTERRAY_API void scaleCursor(Cursor* cursor, i32 rowSpacing, i32 columnSpacing);

//Menu manipulation API
SISTERRAY_API Menu* getMenu(char* menuName);
SISTERRAY_API void addState(Menu* menu, Cursor* context);
SISTERRAY_API Cursor* getStateCursor(Menu* menu, u32 menuState, u32 cursorIdx=0);
SISTERRAY_API void setStateCursor(Menu* menu, u32 menuState, Cursor context, u32 cursorIdx=0);
SISTERRAY_API u32 getMenuState(Menu* menu);
SISTERRAY_API void setMenuState(Menu* menu, u32 value);
SISTERRAY_API Widget* getWidget(Menu* menu);
SISTERRAY_API u32 getActiveCursorIndex(Menu* menu, u32 menuState);
SISTERRAY_API void setActiveCursorIndex(Menu* menu, u32 menuState, u32 cursorIndex);
SISTERRAY_API void setTransitionData(Menu* menu, u32 menuState, TransitionData transition);
SISTERRAY_API TransitionData* getTransitionData(Menu* menu, u32 menuState);
SISTERRAY_API void setOpeningState(Menu* menu, u32 menuState);
SISTERRAY_API void setClosingState(Menu* menu, u32 menuState);
SISTERRAY_API void setNoTransitionState(Menu* menu, u32 menuState);

#endif
