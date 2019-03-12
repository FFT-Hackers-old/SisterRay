#ifndef DRAW_INSTRUCTION_H
#define DRAW_INSTRUCTION_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menus/menu.h"

/*This is an abstract class is a wrapper around the FFVII engines various "Draw" commands enabling */
class DrawInstruction {
public:
    virtual void draw() = 0;
};

class DrawTextInstruction: DrawInstruction {
public:
    drawTextParams instructionParams;
    void DrawTextInstruction(drawTextParams);
    void draw();
};

class DrawNumberInstruction : DrawInstruction {
public:
    drawNumberParams instructionParams;
    void DrawNumberInstruction(drawNumberParams);
    void draw();
};

class DrawBoxInstruction: DrawInstruction {
public:
    drawBoxParams instructionParams;
    void DrawBoxInstruction(drawBoxParams);
    void draw();
};
#endif
