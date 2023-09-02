#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "plane_object.h"
#include "sphere_object.h"

namespace amts {
    struct Scene {
        std::vector<std::unique_ptr<Object>> m_objects;
    };
}

#endif