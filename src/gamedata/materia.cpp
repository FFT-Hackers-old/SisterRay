#include <stdlib.h>
#include <string.h>
#include "../impl.h"

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*x))
#define LEVEL_INDEX_LIMIT  3

SrMateriaRegistry::SrMateriaRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<MateriaData, std::string>(stream) {
    /*Adjust the indexes referenced in kernel.bin to be relative to the action type*/
    for (auto it = begin(_resource_registry); it != end(_resource_registry); ++it) {
        auto materiaidx = distance(_resource_registry.begin(), it);
        auto& materia = *it;
        auto materiaType = materia.type & 0x0F;
        /*Update the references based on materia type to the proper index in the new type specific registries used within sister ray*/
        switch (materiaType) {
            case 11: {
                materia.data[0] -= 56;
                break;
            }
        }
    }
}

SISTERRAY_API SrMateriaData getSrMateria(u16 modMateriaID, const char* modName) {
    SrMateriaData srMateria = SrMateriaData();
    auto name = std::string(modName) + std::to_string(modMateriaID);
    srMateria.baseData = gContext.materias.getElement(name);
    srMateria.auxData = gContext.auxMaterias.getElement(name);

    auto absoluteIndex = gContext.materias.getResourceIndex(name);
    srMateria.materiaName = gContext.gameStrings.materia_names.get_string(absoluteIndex);
    srMateria.materiaDesc = gContext.gameStrings.materia_descriptions.get_string(absoluteIndex);

    return srMateria;
}

SISTERRAY_API void setSrMateriaData(SrMateriaData data, u16 modMateriaID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modMateriaID);
    gContext.materias.updateElement(name, data.baseData);
    gContext.auxMaterias.updateElement(name, data.auxData);

    auto absoluteIndex = gContext.materias.getResourceIndex(name);
    gContext.gameStrings.materia_names.updateResource(absoluteIndex, EncodedString::from_unicode(data.materiaName));
    gContext.gameStrings.materia_descriptions.updateResource(absoluteIndex, EncodedString::from_unicode(data.materiaDesc));
}

SISTERRAY_API void addSrMateria(SrMateriaData data, u16 modMateriaID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modMateriaID);
    gContext.materias.addElement(name, data.baseData);
    gContext.auxMaterias.addElement(name, data.auxData);

    gContext.gameStrings.materia_names.addResource(EncodedString::from_unicode(data.materiaName));
    gContext.gameStrings.materia_descriptions.addResource(EncodedString::from_unicode(data.materiaDesc));
}


static const u32 kPatchApLevel[] = {
    0x005ce2fb, 0x005ce31d, 0x005ce36c, 0x005dfce4,
    0x005dfcfe,
};

static const u32 kPatchEquipEffect[] = {
    0x005cd299, 0x005cd2b8, 0x005cd2e3, 0x005cd30e,
    0x005cd339, 0x005cd364, 0x005cd38f, 0x005cd3ba,
    0x005cd3e5, 0x005cd415, 0x005cd43d, 0x005cd470,
    0x005cd495, 0x005cd4b9, 0x005cd4de, 0x005cd503,
    0x005cd527, 0x005cd54c,
};

static const u32 kPatchStatusEffect[] = {
    0x005cbd94,
};

static const u32 kPatchStatusEffect1[] = {
    0x005cbda7,
};

static const u32 kPatchStatusEffect2[] = {
    0x005cbdbf,
};

static const u32 kPatchStatusEffect3[] = {
    0x005cbc9e, 0x005cbcbb, 0x0070b0c7, 0x0070bc9e,
    0x0071e44a, 0x0071efc1,
};

static const u32 kPatchType[] = {
    0x005c6e04, 0x005c6e4d, 0x005cb969, 0x005cb98e,
    0x005cbe2e, 0x005cbead, 0x005cbf76, 0x005cc2ee,
    0x005cc347, 0x005cc454, 0x005cc524, 0x005cc6f8,
    0x005cc79e, 0x005cc879, 0x005ccac4, 0x005ccb2c,
    0x005ccbf6, 0x005ccd68, 0x005cce41, 0x005ccf2e,
    0x005cd10a, 0x005cd129, 0x005d0274, 0x006cbd03,
};

static const u32 kPatchData[] = {
    0x005cbab5, 0x005cc0fb, 0x005cc135, 0x005cc191,
    0x005cc3eb, 0x005cc90a, 0x005cc91f, 0x005cccb6,
    0x005cd5e4, 0x005cd605, 0x005cd64c, 0x005cd6dd,
    0x005cd7c3, 0x005cd7ff, 0x005cd88f, 0x005cd8ac,
    0x005cd927, 0x005cd949, 0x005cdd12, 0x005cdd3f,
    0x005cdd70, 0x005cdd8f, 0x005cddf8, 0x005cde0d,
    0x005cde2f, 0x005cde56, 0x005cde71, 0x005cdee8,
    0x005cdf4d, 0x005cdf6c, 0x005cdf94, 0x005cdfb4,
    0x005cdfdc, 0x005ce078, 0x005ce097, 0x005ce0bf,
    0x005ce0df, 0x005ce136, 0x005ce1a2, 0x005ce1c2,
    0x005ce1fa, 0x005ce219, 0x005ce261, 0x005ce281,
    0x005ce2a8, 0x005ce9bd, 0x005ceb16, 0x005ceb34,
    0x005ceb9a, 0x005cec36, 0x005cec58, 0x005ceca0,
};

static const u32 kPatchData1[] = {
    0x005cba99, 0x005cc1d5, 0x005cc1fa, 0x005cc256,
    0x005cc961, 0x005cc983,
};

static void patch_materia(void)
{
    srPatchAddresses((void**)kPatchApLevel, ARRAY_SIZE(kPatchApLevel), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, apLevel));
    srPatchAddresses((void**)kPatchEquipEffect, ARRAY_SIZE(kPatchEquipEffect), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, equipEffect));
    srPatchAddresses((void**)kPatchStatusEffect, ARRAY_SIZE(kPatchStatusEffect), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, statusEffects));
    srPatchAddresses((void**)kPatchStatusEffect1, ARRAY_SIZE(kPatchStatusEffect1), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, statusEffects) + 1);
    srPatchAddresses((void**)kPatchStatusEffect2, ARRAY_SIZE(kPatchStatusEffect2), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, statusEffects) + 2);
    srPatchAddresses((void**)kPatchStatusEffect3, ARRAY_SIZE(kPatchStatusEffect3), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, elementType));
    srPatchAddresses((void**)kPatchType, ARRAY_SIZE(kPatchType), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, type));
    srPatchAddresses((void**)kPatchData, ARRAY_SIZE(kPatchData), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, data));
    srPatchAddresses((void**)kPatchData1, ARRAY_SIZE(kPatchData1), (void*)0x00dbdf60, gContext.materias.getData(), offsetof(MateriaData, data) + 1);
}

void initializeAuxMateriaRegistry() {
    for (auto materiaIdx = 0; materiaIdx < KERNEL_MATERIA_COUNT; ++materiaIdx) {
        auto name = std::string(BASE_PREFIX) + std::to_string(materiaIdx);
        auto& kernelMateria = gContext.materias.getElement(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        populateEquipEffects(kernelMateria, boosts);
        AuxMateriaData auxWeapon = { boosts };
        gContext.auxMaterias.addElement(name, auxWeapon);
    }
}


void populateEquipEffects(const MateriaData& kernelMateria, ActorStatBoosts& boosts) {
    auto equipEffect = kernelMateria.equipEffect;
    switch (equipEffect) {
        case 1: {
            boosts.strBoost.amount = 2;
            boosts.strBoost.sign = true;
            boosts.vitBoost.amount = 2;
            boosts.vitBoost.sign = true;
            boosts.maxHPBoost.percentAmount = 5;
            boosts.maxHPBoost.sign = true;

            boosts.magBoost.amount = 2;
            boosts.magDefBoost.amount = 1;
            boosts.maxMPBoost.percentAmount = 5;
            break;
        }
        case 2: {
            boosts.strBoost.amount = 4;
            boosts.strBoost.sign = true;
            boosts.vitBoost.amount = 2;
            boosts.vitBoost.sign = true;
            boosts.maxHPBoost.percentAmount = 10;
            boosts.maxHPBoost.sign = true;

            boosts.magBoost.amount = 4;
            boosts.magDefBoost.amount = 2;
            boosts.maxMPBoost.percentAmount = 10;
            break;
        }
        case 3: {
            boosts.dexBoost.amount = 2;
            boosts.luckBoost.amount = 2;
            boosts.luckBoost.sign = true;
            break;
        }
        case 4: {
            boosts.strBoost.amount = 1;
            boosts.strBoost.sign = true;
            boosts.vitBoost.amount = 1;
            boosts.vitBoost.sign = true;

            boosts.magBoost.amount = 1;
            boosts.magDefBoost.amount = 1;
            break;
        }
    }
}

SISTERRAY_API void init_materia(SrKernelStream* stream) {
    gContext.materias = SrMateriaRegistry(stream);
    gContext.auxMaterias = SrAuxMateriaRegistry();
    void initializeAuxMateriaRegistry();
    srLogWrite("kernel.bin: Loaded %lu Materias", (unsigned long)gContext.materias.resourceCount());
    patch_materia();
}

u8 getMateriaTopType(u16 materiaID) {
    return gContext.materias.getResource(materiaID).type & 0x0F;
}

u8 getMateriaSubType(u16 materiaID) {
    return (gContext.materias.getResource(materiaID).type & 0xF0) >> 4;
}

u32 getMateriaColorType(u16 materiaID) {
    u8* greaterTypeArray = (u8*)(0x91ABF8);
    u8* materiaAssetType = (u8*)(0x91ABF0);
    auto materiaType = materiaAssetType[greaterTypeArray[gContext.materias.getResource(materiaID).type & 0xF]];
    return materiaType;
}

u8 getMateriaLevel(const MateriaInventoryEntry& materia, u8* maxLevelPtr) {
    if (materia.item_id == 0xFFFF) {
        *maxLevelPtr = 0;
        return 0;
    }
    auto levels = gContext.materias.getResource(materia.item_id).apLevel;

    u8 level = 1;
    u8 maxLevel = 1;
    for (u16 apLevelIndex = 0; levels[apLevelIndex] != 0xFFFF; ++apLevelIndex) {
        if (materia.materia_ap > (levels[apLevelIndex] * 100))
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
