#ifndef INVENTORY_REGISTRY_TEMPLATE_H
#define INVENTORY_REGISTRY_TEMPLATE_H

#include <SisterRay/SisterRay.h>
#include <unordered_map>
#include "../sr_registry_template.h"

/*This template requires that quantity/materiaID are in the struct
  Could metaprogram this away but not worth the effort right now*/
template<class T> class SrInventory : public SrResourceRegistry<T> {
public:
    SrInventory(u16 allocation_size) : SrResourceRegistry<T>(allocation_size) {};
    SrInventory(): SrResourceRegistry() {};

    void decrementInventoryEntry(u16 inventory_index, u8 stepSize) {
        if (_resource_registry[inventory_index].quantity + stepSize > 1) {
            _resource_registry[inventory_index].quantity = _resource_registry[inventory_index].quantity - stepSize;
        }
        else {
            _resource_registry[inventory_index].materiaID = 0xFFFF;
            _resource_registry[inventory_index].quantity = 0;
        }
    }

    void incrementInventoryEntry(u16 inventory_index, u8 stepSize) {
        if (_resource_registry[inventory_index].quantity + stepSize < 99) {
            _resource_registry[inventory_index].quantity = _resource_registry[inventory_index].quantity + stepSize;
        }
        else {
            _resource_registry[inventory_index].quantity = 99;
        }
    }

    void addResourceAtIndex(u16 inventory_index, u16 itemID) {
        if (_resource_registry[inventory_index].materiaID == 0xFFFF) {
            _resource_registry[inventory_index].materiaID = itemID;
            _resource_registry[inventory_index].quantity = 1;
        }
    }

    u16 findItemInventoryIndex(u16 itemID) {
        for (auto it = std::begin(_resource_registry); it != std::end(_resource_registry); ++it) {
            if ((*it).materiaID == itemID) {
                return distance(_resource_registry.begin(), it);
            }
        }
        return 0xFFFF;
    }

    //Utility function for incrementing an item from its absolute item ID
    bool incrementInventoryByItemID(u16 absoluteID, u8 stepSize) {
        u16 inventoryIndex = findItemInventoryIndex(absoluteID);
        if (inventoryIndex != 0xFFFF) {
            incrementInventoryEntry(inventoryIndex, stepSize);
            return true;
        }
        return false;
    }

    bool decrementInventoryByItemID(u16 absoluteID, u8 stepSize) {
        u16 inventoryIndex = findItemInventoryIndex(absoluteID);
        if (inventoryIndex != 0xFFFF) {
            decrementInventoryEntry(inventoryIndex, stepSize);
            return true;
        }
        return false;
    }
};

#endif
