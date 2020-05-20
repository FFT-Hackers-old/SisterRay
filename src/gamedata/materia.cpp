#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "materia.h"

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*x))
#define LEVEL_INDEX_LIMIT  3

SrMateriaRegistry::SrMateriaRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrMateria, std::string>() {
    size_t read_size;
    MateriaData baseMateria;
    std::string srAttackID;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseMateria, sizeof(baseMateria));
        if (read_size != sizeof(baseMateria))
            break;
        SrMateria materia;
        materia.gameMateria = baseMateria;
        auto materiaType = materia.gameMateria.type & 0x0F;
        /*Update the references based on materia type to the proper index in the new type specific registries used within sister ray*/
        switch (materiaType) {
        case 11: {
            materia.gameMateria.data[0] -= 56;
            break;
        }
        }
        materia.materiaName = gContext.gameStrings.materia_names.get_string(idx);
        materia.materiaDescription = gContext.gameStrings.materia_descriptions.get_string(idx);
        setEquipEffects(materia.gameMateria.equipEffect, materia, idx);
        addElement(assembleGDataKey(idx), materia);
        ++idx;
    }
}

SISTERRAY_API SrMateriaData getSrMateria(u16 modMateriaID, const char* modName) {
    SrMateriaData srMateria = SrMateriaData();
    auto name = std::string(modName) + std::to_string(modMateriaID);
    auto& materia = gContext.materias.getElement(name);
    srMateria.baseData = materia.gameMateria;
    srMateria.materiaName = materia.materiaName.str();
    srMateria.materiaDesc = materia.materiaDescription.str();

    return srMateria;
}

SISTERRAY_API void setSrMateriaData(SrMateriaData data, u16 modMateriaID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modMateriaID);
    auto materia = SrMateria();
    materia.gameMateria = data.baseData;
    materia.auxData = data.auxData;
    materia.materiaName = EncodedString::from_unicode(data.materiaName);
    materia.materiaDescription = EncodedString::from_unicode(data.materiaDesc);
    gContext.materias.updateElement(name, materia);
}

SISTERRAY_API void addSrMateria(SrMateriaData data, u16 modMateriaID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modMateriaID);
    auto materia = SrMateria();
    materia.gameMateria = data.baseData;
    materia.auxData = data.auxData;
    materia.materiaName = EncodedString::from_unicode(data.materiaName);
    materia.materiaDescription = EncodedString::from_unicode(data.materiaDesc);
    gContext.materias.addElement(name, materia);
}


void setEquipEffects(u16 equipEffect, SrMateria& srMateria, u16 materiaIdx) {
    auto& srEquipEffects = srMateria.equipEffects;
    auto& hpBoosts = srEquipEffects["HP"];
    auto& mpBoosts = srEquipEffects["MP"];
    auto& strBoosts = srEquipEffects["STR"];
    auto& vitBoosts = srEquipEffects["VIT"];
    auto& magBoosts = srEquipEffects["MAG"];
    auto& sprBoosts = srEquipEffects["SPR"];
    auto& dexBoosts = srEquipEffects["DEX"];
    auto& lckBoosts = srEquipEffects["LCK"];

    auto statBoost = StatBoost();
    statBoost.tags.insert("MATERIA");
    statBoost.tags.insert(assembleGDataKey(materiaIdx));
    switch (equipEffect) {
        case 1: {
            statBoost.amount = 2;
            statBoost.sign = true;
            strBoosts.push_back(statBoost);

            statBoost.amount = 2;
            vitBoosts.push_back(statBoost);

            statBoost.amount = 5;
            statBoost.isPercent = true;
            hpBoosts.push_back(statBoost);

            statBoost.amount = 2;
            statBoost.sign = false;
            statBoost.isPercent = false;
            magBoosts.push_back(statBoost);

            statBoost.amount = 1;
            sprBoosts.push_back(statBoost);

            statBoost.amount = 5;
            statBoost.isPercent = true;
            mpBoosts.push_back(statBoost);
            break;
        }
        case 2: {
            statBoost.amount = 4;
            statBoost.sign = true;
            strBoosts.push_back(statBoost);

            statBoost.amount = 2;
            vitBoosts.push_back(statBoost);

            statBoost.amount = 10;
            statBoost.isPercent = true;
            hpBoosts.push_back(statBoost);

            statBoost.amount = 4;
            statBoost.sign = false;
            statBoost.isPercent = false;
            magBoosts.push_back(statBoost);

            statBoost.amount = 2;
            sprBoosts.push_back(statBoost);

            statBoost.amount = 10;
            statBoost.isPercent = true;
            mpBoosts.push_back(statBoost);
            break;
        }
        case 3: {
            statBoost.amount = 2;
            dexBoosts.push_back(statBoost);
            statBoost.amount = 2;
            statBoost.sign = true;
            lckBoosts.push_back(statBoost);
            break;
        }
        case 4: {
            statBoost.amount = 1;
            statBoost.sign = true;
            strBoosts.push_back(statBoost);
            statBoost.amount = 1;
            vitBoosts.push_back(statBoost);

            statBoost.sign = false;
            statBoost.amount = 1;
            magBoosts.push_back(statBoost);
            statBoost.amount = 1;
            sprBoosts.push_back(statBoost);
            break;
        }
    }
}

SISTERRAY_API void initMateria(SrKernelStream* stream) {
    gContext.materias = SrMateriaRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu Materias", (unsigned long)gContext.materias.resourceCount());
}

u8 getMateriaTopType(u16 materiaID) {
    return gContext.materias.getResource(materiaID).gameMateria.type & 0x0F;
}

u8 getMateriaSubType(u16 materiaID) {
    return (gContext.materias.getResource(materiaID).gameMateria.type & 0xF0) >> 4;
}

u32 getMateriaColorType(u16 materiaID) {
    u8* greaterTypeArray = (u8*)(0x91ABF8);
    u8* materiaAssetType = (u8*)(0x91ABF0);
    auto materiaType = materiaAssetType[greaterTypeArray[gContext.materias.getResource(materiaID).gameMateria.type & 0xF]];
    return materiaType;
}

u8 getMateriaLevel(const MateriaInventoryEntry& materia, u8* maxLevelPtr) {
    if (materia.materiaID == 0xFFFF) {
        *maxLevelPtr = 0;
        return 0;
    }
    auto levels = gContext.materias.getResource(materia.materiaID).gameMateria.apLevel;

    u8 level = 1;
    u8 maxLevel = 1;
    for (u16 apLevelIndex = 0; levels[apLevelIndex] != 0xFFFF; ++apLevelIndex) {
        if (materia.materiaAP > (levels[apLevelIndex] * 100))
            ++level;
        maxLevel++;

        if (apLevelIndex == LEVEL_INDEX_LIMIT)
            break;
    }
    *maxLevelPtr = maxLevel;
    return level;
}

SrEventContext getTopKey(u8 materiaType) {
    switch (materiaType) {
        case 0:
            return MAT_HIGH_TYPE_0;
        case 1:
            return MAT_HIGH_TYPE_1;
        case 2:
            return MAT_HIGH_TYPE_2;
        case 3:
            return MAT_HIGH_TYPE_3;
        case 4:
            return MAT_HIGH_TYPE_4;
        case 5:
            return MAT_HIGH_TYPE_5;
        case 6:
            return MAT_HIGH_TYPE_6;
        case 7:
            return MAT_HIGH_TYPE_7;
        case 8:
            return MAT_HIGH_TYPE_8;
        case 9:
            return MAT_HIGH_TYPE_9;
        case 10:
            return MAT_HIGH_TYPE_10;
        case 11:
            return MAT_HIGH_TYPE_11;
        case 12:
            return MAT_HIGH_TYPE_12;
        case 13:
            return MAT_HIGH_TYPE_13;
        case 14:
            return MAT_HIGH_TYPE_14;
        case 15:
            return MAT_HIGH_TYPE_15;
        default:
            return MAT_HIGH_TYPE_0;
    }
}

SrEventContext getSubKey(u8 materiaType) {
    switch (materiaType) {
        case 0:
            return MAT_SUB_TYPE_0;
        case 1:
            return MAT_SUB_TYPE_1;
        case 2:
            return MAT_SUB_TYPE_2;
        case 3:
            return MAT_SUB_TYPE_3;
        case 4:
            return MAT_SUB_TYPE_4;
        case 5:
            return MAT_SUB_TYPE_5;
        case 6:
            return MAT_SUB_TYPE_6;
        case 7:
            return MAT_SUB_TYPE_7;
        case 8:
            return MAT_SUB_TYPE_8;
        case 9:
            return MAT_SUB_TYPE_9;
        case 10:
            return MAT_SUB_TYPE_10;
        case 11:
            return MAT_SUB_TYPE_11;
        case 12:
            return MAT_SUB_TYPE_12;
        case 13:
            return MAT_SUB_TYPE_13;
        case 14:
            return MAT_SUB_TYPE_14;
        case 15:
            return MAT_SUB_TYPE_15;
        default:
            return MAT_SUB_TYPE_0;
    }
}
