#ifndef ACTION_TARGETING_H
#define ACTION_TARGETING_H

#include <SisterRay/SisterRay.h>

void setTargetingFromFlags(u8 flags, u32 reverseRow);
void setTargetingDataGlobals(u32 reverseRow);
u8 getTargetRow();
void setValidTargetMask();
void storeFirstWCommandIssued();

#endif 
