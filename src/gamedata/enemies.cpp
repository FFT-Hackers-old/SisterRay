#include "enemies.h"

/*Using this computation to avoid annoying template specialization issues, and avoid wrapping add_element and changing the API for this registry
  Pre-computing this may be logical, but we can't overwrite add_element so there's not a great way to implement that and keep the shared registry interface*/
std::unordered_set<u16> SrEnemyRegistry::getUniqueModelIDs() {
    auto size = resource_count();
    auto modelIDs = std::unordered_set<u16>();
    for (auto i = 0; i < size; ++i) {
        if (get_resource(i).modelID != 0xFFFF)
            modelIDs.insert(get_resource(i).modelID);
    }
    return modelIDs;
}

std::string assembleEnemyDataKey(std::string uniqueID) {
    return std::string(BASE_PREFIX) + uniqueID;
}
