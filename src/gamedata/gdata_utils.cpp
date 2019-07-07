#include "gdata_utils.h"


std::string assembleGDataKey(u16 resourceIdx) {
    return std::string(BASE_PREFIX) + std::to_string(resourceIdx);
}
