#ifndef MATERIA_INPUT_CALLBACKS_H
#define MATERIA_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"

void checkArrangeChoiceHandler(const MateriaInputEvent* event);
void enterMateriaViewHandler(const MateriaInputEvent* event);
void equipMateriaHandler(const MateriaInputEvent* event);
void selectCheckViewHandler(const MateriaInputEvent* event);
void arrangeChoiceHandler(const MateriaInputEvent* event);
void trashMateriaHandler(const MateriaInputEvent* event);
void confirmTrashHandler(const MateriaInputEvent* event);

void exitMenuHandler(const MateriaInputEvent* event);
void exitMateriaView(const MateriaInputEvent* event);
void exitCheckArrangeView(const MateriaInputEvent* event);
void exitSpellCheckView(const MateriaInputEvent* event);
void exitTrashMateriaView(const MateriaInputEvent* event);
void exitTrashConfirmView(const MateriaInputEvent* event);

void removeMateriaHandler(const MateriaInputEvent* event);


#endif
