#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"

namespace amts {
    struct Scene {
        std::vector<std::unique_ptr<Object>> m_objects;
    };
}

#endif