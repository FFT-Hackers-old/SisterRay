#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <string>
#include "../EncodedString.h"
#include "../sr_named_registry.h"

typedef struct {
    EncodedString displayName;
    bool isGameElement;
    bool gameIndex;
    std::string affName;
    std::string resName;
    std::string penName;
}SrElement;

class SrElementRegistry : public SrNamedResourceRegistry<SrElement, std::string> {
public:
    SrElementRegistry() : SrNamedResourceRegistry<SrElement, std::string>() {}
    SrElementRegistry(bool useResistances = true);
};

void initElements();
std::string getElementIDFromIndex(u8 elementIdx);

#endif 
