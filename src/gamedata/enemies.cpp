#include "enemies.h"

/*Using this computation to avoid annoying template specialization issues, and avoid wrapping addElement and changing the API for this registry
  Pre-computing this may be logical, but we can't overwrite addElement so there's not a great way to implement that and keep the shared registry interface*/
std::unordered_set<u16> SrEnemyRegistry::getUniqueModelIDs() {
    auto size = resourceCount();
    auto modelIDs = std::unordered_set<u16>();
    for (auto i = 0; i < size; ++i) {
        if (getResource(i).modelID != 0xFFFF)
            modelIDs.insert(getResource(i).modelID);
    }
    return modelIDs;
}
