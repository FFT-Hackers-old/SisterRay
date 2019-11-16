#ifndef ACTION_INTERFACE_H
#define ACTION_INTERFACE_H

#include <SisterRay/SisterRay.h>
SISTERRAY_API void setChosenCommandID(u8 commandID);
SISTERRAY_API u8 getChosenCommandID();
SISTERRAY_API void setChosenActionID(u16 actionID);
SISTERRAY_API u16 getChosenActionID();
SISTERRAY_API void setChosenActionMenuIndex(u16 menuIndex);
SISTERRAY_API u16 getChosenActionMenuIndex();

SISTERRAY_API void setChosenActionTargetData(u8 targetFlags);
SISTERRAY_API u8 getChosenActionTargetData();
SISTERRAY_API void setChosenActionTargetType(u8 targetType);
SISTERRAY_API u8 getChosenActionTargetType();
SISTERRAY_API void setChosenActionTargetIndex(u8 targetIndex);
SISTERRAY_API u8 getChosenActionTargetIndex();
SISTERRAY_API void setChosenActionTargetRow(u8 targetRow);
SISTERRAY_API u8 getChosenActionTargetRow();
SISTERRAY_API void setChosenActionValidTargetMask(u16 targetMask);
SISTERRAY_API u8 getChosenActionValidTargetMask()
#endif 
