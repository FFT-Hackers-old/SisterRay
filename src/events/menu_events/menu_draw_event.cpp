#include "menu_draw_event.h"


MenuDrawEvent::MenuDrawEvent() {}

void MenuDrawEvent::clearDrawInstructions() {
    instructions.clear();
}

void MenuDrawEvent::addDrawInstruction(drawBoxParams params) {
    std::unique_ptr<DrawBoxInstruction> instruction_ptr = std::make_unique<DrawBoxInstruction>(params);
    instructions.emplace_back(instruction_ptr);
}

void MenuDrawEvent::addDrawInstruction(drawTextParams params) {
    std::unique_ptr<DrawTextInstruction> instruction_ptr = std::make_unique<DrawTextInstruction>(params);
    instructions.emplace_back(instruction_ptr);
}

void MenuDrawEvent::addDrawInstruction(drawNumberParams params) {
    std::unique_ptr<DrawNumberInstruction> instruction_ptr = std::make_unique<DrawNumberInstruction>(params);
    instructions.emplace_back(instruction_ptr);
}

void MenuDrawEvent::execute() {
    for (auto instruction = instructions.begin(); instruction != instructions.end(); instruction++) {
        (*instruction)->draw();
    }
}
