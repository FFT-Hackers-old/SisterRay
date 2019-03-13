#ifndef GUI_DRAW_EVENT_H
#define GUI_DRAW_EVENT_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menus/menu.h"
#include "../sr_event_base.h"
#include "draw_instruction.h"
#include "equip_menu_events.h"
#include <vector>
#include <memory>

/*This class holds an std::vector of DrawInstructions corresponding to a particular event/widget
  An instance of it with an associated enum event Type is mutated by callbacks in order to render menu widgets*/
class MenuDrawEvent: public SrEvent<EquipMenuEvent> {
public:
    std::vector<std::unique_ptr<DrawInstruction>> instructions;
    MenuDrawEvent(EquipMenuEvent event_type);
    void clearDrawInstructions();
    void addDrawInstruction(drawTextParams params);
    void addDrawInstruction(drawBoxParams params);
    void addDrawInstruction(drawNumberParams params);
    void execute();
};
#endif
