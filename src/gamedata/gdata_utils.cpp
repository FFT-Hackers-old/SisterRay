#include "gdata_utils.h"

const std::string assembleGDataKey(u16 resourceIdx) {
    return std::string(BASE_PREFIX) + std::to_string(resourceIdx);
}


