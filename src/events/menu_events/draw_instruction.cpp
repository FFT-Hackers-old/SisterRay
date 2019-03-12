#include "draw_instruction.h"

DrawTextInstruction::DrawTextInstruction(drawTextParams params) {
    instructionParams = params;
}

void DrawTextInstruction::draw() {
    gameDrawString(
        instructionParams.xCoordinate,
        instructionParams.yCoordinate,
        instructionParams.stringToDraw,
        instructionParams.textColor,
        instructionParams.textThickness
    );
}


DrawNumberInstruction::DrawNumberInstruction(drawNumberParams params) {
    instructionParams = params;
}

void DrawNumberInstruction::draw() {
    gameDrawNumbers(
        instructionParams.xCoordinate,
        instructionParams.yCoordinate,
        instructionParams.numberToDraw,
        instructionParams.charCount,
        instructionParams.numberColor,
        instructionParams.numberThickness
    );
}

DrawBoxInstruction::DrawBoxInstruction(drawBoxParams params) {
    instructionParams = params;
}

void DrawBoxInstruction::draw() {
    i16 drawDistances[4] = { instructionParams.drawDistance1, instructionParams.drawDistance2, instructionParams.drawDistance3, instructionParams.drawDistance4 };
    gameDrawBox(
        drawDistances,
        instructionParams.boxFloat
    );
}
