#ifndef ACTION_TARGETING_H
#define ACTION_TARGETING_H

#include <SisterRay/SisterRay.h>

void setTargetingFromFlags(u8 flags, bool reverseRow);
void setTargetingDataGlobals(bool reverseRow);
u8 getTargetRow();
void setValidTargetMask();
void storeFirstWCommandIssued();

#endif 
