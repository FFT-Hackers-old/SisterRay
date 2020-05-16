#ifndef ANIMATION_SCRIPT_OPC_HANDLERS_H
#define ANIMATION_SCRIPT_OPC_HANDLERS_H

#include <SisterRay/SisterRay.h>
#include "animation_script_opcode.h"

extern bool invertDirection;
extern bool stallBeforeRetreat[10];
extern u16 stallFrames[10];
extern u16 framesStalled[10];

OpCodeControlSequence nopCode(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCode8E(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode8F(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode90(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode91(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode92(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode93(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode94(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode95(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode96(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode97(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode98(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode99(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9A(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9B(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9C(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9D(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9E(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCode9F(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCodeA0(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA5(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeA9(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAB(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAD(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAE(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeAF(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCodeB0(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeB9(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeBA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeBC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeBD(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeBE(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeBF(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCodeC1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC2(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC3(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC5(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeC8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeCA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeCC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeCE(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeCF(AnimScriptEvent* srEvent);


OpCodeControlSequence OpCodeD0(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD2(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD3(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD5(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeD8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDB(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDD(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDE(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeDF(AnimScriptEvent* srEvent);


OpCodeControlSequence OpCodeE0(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE2(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE3(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE5(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeE9(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeEA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeEB(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeED(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeEC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeEE(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCodeF0(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF1(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF3(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF4(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF5(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF6(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF7(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF8(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeF9(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeFA(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeFC(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeFD(AnimScriptEvent* srEvent);
OpCodeControlSequence OpCodeFE(AnimScriptEvent* srEvent);

OpCodeControlSequence OpCodePHSChange(AnimScriptEvent* srEvent);

#endif 
