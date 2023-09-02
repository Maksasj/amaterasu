#ifndef _MATERIAL_COLLECTION_H_
#define _MATERIAL_COLLECTION_H_

#include "material.h"

namespace amts {
    struct MaterialCollection {
        std::vector<std::unique_ptr<Material>> m_materials;
    };
}

#endif