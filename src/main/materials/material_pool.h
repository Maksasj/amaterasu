#ifndef _MATERIAL_POOL_H_
#define _MATERIAL_POOL_H_

#include "material.h"

namespace amts {
    struct MaterialPool {
        std::vector<std::unique_ptr<Material>> m_materials;
    };
}

#endif