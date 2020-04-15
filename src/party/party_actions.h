#ifndef PARTY_ACTIONS_H
#define PARTY_ACTIONS_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API void addAutoAction(u8 characterIdx, AutoActionType type, u8 commandIndex, u16 actionID, u8 activationChance, u8 counterCount);

/*Public API for getting and enabling stuff in your own enabling handlers*/
SISTERRAY_API void enableCommand(u8 characterIdx, u8 enabledIndex, u8 commandIndex);
SISTERRAY_API void voidCommand(u8 characterIdx, u8 enabledIndex);
SISTERRAY_API u8 getEnabledSlotIndex(u8 characterIdx, u8 commandIndex);
SISTERRAY_API void  insertEnabledCommand(u8 characterIdx, u8 commandIndex);
SISTERRAY_API EnabledCommand* getCommandSlot(u8 characterIdx, u8 commandIndex);

SISTERRAY_API EnabledSpell* getSpellSlot(u8 characterIdx, u8 commandIndex, u16 actionIndex);
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u8 characterIdx, u32 enabledSlotIndex);
SISTERRAY_API void enableMagic(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u8 characterIdx, u32 enabledSlotIndex);
SISTERRAY_API void enableSummon(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u8 characterIdx, u32 enabledSlotIndex);
SISTERRAY_API void enableESkill(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex);
#endif // !PARTY_ACTIONS_H
