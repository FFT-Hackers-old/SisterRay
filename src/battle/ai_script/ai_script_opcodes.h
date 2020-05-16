#ifndef AI_SCRIPT_OPCODES_H
#define AI_SCRIPT_OPCODES_H

#include <SisterRay/SisterRay.h>

void pushScriptValues(AIScriptEvent* srEvent);
void pushScriptAddresses(AIScriptEvent* srEvent);
void handleMathOperations(AIScriptEvent* srEvent);
void handleCompareOperations(AIScriptEvent* srEvent);
void handlePushConstants(AIScriptEvent* srEvent);
void handleJumps(AIScriptEvent* srEvent);
void enqueueScriptAction(u8 actorID, u8 commandIndex, u16 relAttackIndex);

OpCodeControlSequence nopCode(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode00(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode01(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode02(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode03(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode10(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode11(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode12(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode13(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode30(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode31(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode32(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode33(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode34(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode35(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode36(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode37(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode40(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode41(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode42(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode43(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode44(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode45(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode50(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode51(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode52(AIScriptEvent* srEvent);


OpCodeControlSequence OpCode60(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode61(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode62(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode70(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode71(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode72(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode73(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode74(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode75(AIScriptEvent* srEvent);

OpCodeControlSequence OpCode80(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode81(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode82(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode83(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode84(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode85(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode86(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode87(AIScriptEvent* srEvent);


OpCodeControlSequence OpCode90(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode91(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode92(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode93(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode94(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode95(AIScriptEvent* srEvent);
OpCodeControlSequence OpCode96(AIScriptEvent* srEvent);


#endif
