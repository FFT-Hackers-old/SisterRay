#ifndef ITEMS_H
#define ITEMS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include <vector>

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T> class SrResourceRegistry {
public:
    T get_resource(resource index) {
        return SrResourceRegistry::resource_registry[index];
    }

    void add_resource(T resource) {
        SrResourceRegistry::resource_registry.push_back(resource);
    }

    int resource_count() {
        return resource_registry.size();
    }

private:
    std::vector<T> resource_registry;
};

#endif
