#include "model_file_utils.h"
#include "../../impl.h"

void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

/*Used when loading b file for summons and other models that do not need to be SR'd yet*/
void* srGameLoadABFile(LGPContext* context, char* baseFileName) {
    i32 bytesRead = 0;
    char abFileNameBuffer[204];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    return ff7LoadModelFile(context, &bytesRead, abFileNameBuffer);
}


void srGameInitAnimations(u32 lgpIndex, u32 unkint, void** srAnimsTable, LGPContext* context, char* filename) {
    ff7InitAnimations(lgpIndex, unkint, srAnimsTable, context, filename);
}

LGPArchiveFile srOpenDAFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer, bool doMangle) {
    i32 bytesRead = 0;
    char daFileNameBuffer[204];
    char mangledName[32];
    u32 archiveIndex = 0;

    if (doMangle) {
        createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
        context->mangler(&(daFileNameBuffer[0]), &(mangledName[0]));
        srLogWrite("attempting to open file with mangled name: %s", &(mangledName[0]));
        return lgpArchiveRead((u8*)battleLGPBuffer, lowerCaseStr(mangledName).c_str());
    }
    else {
        createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
        srLogWrite("attempting to open file with mangled name: %s", &(daFileNameBuffer[0]));
        return lgpArchiveRead((u8*)battleLGPBuffer, daFileNameBuffer);
    }
}

LGPArchiveFile srOpenABFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer) {
    i32 bytesRead = 0;
    char abFileNameBuffer[204];
    char mangledName[32];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    context->mangler(&(abFileNameBuffer[0]), &(mangledName[0]));

    //srLogWrite("mangled name: %s", mangledName);
    LGPArchiveFile archiveFile = lgpArchiveRead((u8*)battleLGPBuffer, mangledName);
    return archiveFile;
}

std::vector<std::unordered_map<std::string, SrAnimation>> loadModelAnimationFromDAFile(const char* modelName, void* daFileBuffer, bool hasWeapon) {
    u32* daFilePtr = (u32*)(daFileBuffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %x, %x, %x, %x, %x", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4]);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    std::vector<std::unordered_map<std::string, SrAnimation>> parsedAnimations;
    for (u32 animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
        srLogWrite("Loading animation with bones: %i, frame count: %i", animHeader->bonesCount, animHeader->framesCount);
        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, (u32*)frameDataPtr);
        u32 animSize = ((12 * (animHeader->bonesCount - 1)) + 24) * animHeader->framesCount;
        SrAnimation srAnim = { animSize, currentAnimation };
        std::unordered_map<std::string, SrAnimation> animMap;
        if (hasWeapon) {
            if (animationIdx < 8) {
                animMap["BASE"] = srAnim;
                parsedAnimations.push_back(animMap);
            }
            else {
                parsedAnimations[animationIdx - 8]["WPN"] = srAnim;
            }
        }
        else {
            animMap["BASE"] = srAnim;
            parsedAnimations.push_back(animMap);
        }
        animDataStartPtr = (u32*)&(frameDataPtr[animHeader->compressedSize]);
    }
    return parsedAnimations;
}
