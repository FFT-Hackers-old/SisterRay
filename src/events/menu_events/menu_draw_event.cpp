#include "menu_draw_event.h"


MenuDrawEvent::MenuDrawEvent(EquipMenuEvent event_type): SrEvent(event_type) {}

void MenuDrawEvent::clearDrawInstructions() {
    instructions.clear();
}

void MenuDrawEvent::addDrawInstruction(drawBoxParams params) {
    instructions.emplace_back(std::make_unique<DrawBoxInstruction>(params));
}

void MenuDrawEvent::addDrawInstruction(drawTextParams params) {
    instructions.emplace_back(std::make_unique<DrawTextInstruction>(params));
}

void MenuDrawEvent::addDrawInstruction(drawNumberParams params) {
    instructions.emplace_back(std::make_unique<DrawNumberInstruction>(params));
}

void MenuDrawEvent::execute() {
    for (auto instruction = instructions.begin(); instruction != instructions.end(); instruction++) {
        (*instruction)->draw();
    }
}
