#ifndef DRAW_INSTRUCTION_H
#define DRAW_INSTRUCTION_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menus/menu.h"

/*This is an abstract class is a wrapper around the FFVII engines various "Draw" commands enabling
  Delayed execution of them. A sequence of these commands define an FFVII menu widget*/
class DrawInstruction {
public:
    virtual void draw() = 0;
};

class DrawTextInstruction: public DrawInstruction {
public:
    drawTextParams instructionParams;
    DrawTextInstruction(drawTextParams params);
    void draw();
};

class DrawNumberInstruction : public DrawInstruction {
public:
    drawNumberParams instructionParams;
    DrawNumberInstruction(drawNumberParams params);
    void draw();
};

class DrawBoxInstruction: public DrawInstruction {
public:
    drawBoxParams instructionParams;
    DrawBoxInstruction(drawBoxParams params);
    void draw();
};
#endif
