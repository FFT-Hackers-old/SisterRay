#include "elements.h"
#include "../party/stat_names.h"
#include "element_names.h"

SrElementRegistry::SrElementRegistry(bool useResistances) {
    SrElement fire{ EncodedString::from_unicode(ElementNames::FIRE_NAME.c_str()), true, 0, StatNames::FIRE_AFF, StatNames::FIRE_RES };
    addElement(ElementNames::FIRE, fire);
    SrElement ice{ EncodedString::from_unicode(ElementNames::ICE_NAME.c_str()), true, 1, StatNames::ICE_AFF, StatNames::ICE_RES };
    addElement(ElementNames::ICE, ice);
    SrElement bolt{ EncodedString::from_unicode(ElementNames::LIGHT_NAME.c_str()), true, 2, StatNames::LIGHT_AFF, StatNames::LIGHT_RES };
    addElement(ElementNames::BOLT, bolt);
    SrElement earth{ EncodedString::from_unicode(ElementNames::EARTH_NAME.c_str()), true, 3, StatNames::EARTH_AFF, StatNames::EARTH_RES };
    addElement(ElementNames::EARTH, earth);
    SrElement poison{ EncodedString::from_unicode(ElementNames::POISON_NAME.c_str()), true, 4, StatNames::POISON_AFF, StatNames::POISON_RES };
    addElement(ElementNames::PSN, poison);
    SrElement gravity{ EncodedString::from_unicode(ElementNames::GRAV_NAME.c_str()), true, 5, StatNames::GRAVITY_AFF, StatNames::GRAVITY_RES };
    addElement(ElementNames::GRAV, gravity);
    SrElement water{ EncodedString::from_unicode(ElementNames::WATER_NAME.c_str()), true, 6, StatNames::WATER_AFF, StatNames::WATER_RES };
    addElement(ElementNames::WATER, water);
    SrElement wind{ EncodedString::from_unicode(ElementNames::WIND_NAME.c_str()), true, 7, StatNames::WIND_AFF, StatNames::WIND_RES };
    addElement(ElementNames::WIND, wind);
    SrElement holy{ EncodedString::from_unicode(ElementNames::HOLY_NAME.c_str()), true, 8, StatNames::HOLY_AFF, StatNames::HOLY_RES };
    addElement(ElementNames::HOLY, holy);
    SrElement restore{ EncodedString::from_unicode(ElementNames::RESTORE_NAME.c_str()), true, 9, StatNames::HOLY_AFF, StatNames::HOLY_RES };
    addElement(ElementNames::RESTORE, restore);

    SrElement slash{ EncodedString::from_unicode(ElementNames::SLASH_NAME.c_str()), true, 10, StatNames::SLASH_ARM, };
    addElement(ElementNames::SLASH, slash);
    SrElement blunt{ EncodedString::from_unicode(ElementNames::BLUNT_NAME.c_str()), true, 11, StatNames::BLUNT_ARM,  };
    addElement(ElementNames::BLUNT, blunt);
    SrElement pierce{ EncodedString::from_unicode(ElementNames::PIERCE_NAME.c_str()), true, 12, StatNames::PIERCE_ARM,  };
    addElement(ElementNames::PIERCE, pierce);
    SrElement shadow{ EncodedString::from_unicode(ElementNames::SHADOW_NAME.c_str()), true, 15, StatNames::SHADOW_AFF, StatNames::SHADOW_RES };
    addElement(ElementNames::DARK, shadow);
}

void initElements() {
    gContext.elements = SrElementRegistry(true);
    srLogWrite("kernel.bin: Loaded %lu elements", (unsigned long)gContext.elements.resourceCount());
}


std::string getElementIDFromIndex(u8 elementIdx) {
    switch (elementIdx) {
    case ELM_FIRE: {
        return ElementNames::FIRE;
    }
    case ELM_ICE: {
        return ElementNames::ICE;
    }
    case ELM_THUNDER: {
        return ElementNames::BOLT;
    }
    case ELM_WATER: {
        return ElementNames::WATER;
    }
    case ELM_WIND: {
        return ElementNames::WIND;
    }
    case ELM_EARTH: {
        return ElementNames::EARTH;
    }
    case ELM_POISON: {
        return ElementNames::PSN;
    }
    case ELM_GRAVITY: {
        return ElementNames::GRAV;
    }
    case ELM_HOLY: {
        return ElementNames::HOLY;
    }
    case ELM_SHADOW: {
        return ElementNames::DARK;
    }
    }
}
