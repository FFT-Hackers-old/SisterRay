#ifndef MATERIA_INPUT_CALLBACKS_H
#define MATERIA_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "materia_widget_names.h"

void checkArrangeChoiceHandler(const MenuInputEvent* event);
void enterMateriaViewHandler(const MenuInputEvent* event);
void equipMateriaHandler(const MenuInputEvent* event);
void selectCheckViewHandler(const MenuInputEvent* event);
void arrangeChoiceHandler(const MenuInputEvent* event);
void trashMateriaHandler(const MenuInputEvent* event);
void confirmTrashHandler(const MenuInputEvent* event);

void exitMenuHandler(const MenuInputEvent* event);
void exitMateriaView(const MenuInputEvent* event);
void exitCheckArrangeView(const MenuInputEvent* event);
void exitSpellCheckView(const MenuInputEvent* event);
void exitTrashMateriaView(const MenuInputEvent* event);
void exitTrashConfirmView(const MenuInputEvent* event);

void removeMateriaHandler(const MenuInputEvent* event);

void enterSlotView(const MenuInputEvent* event);
void enterOptionView(const MenuInputEvent* event);


#endif
