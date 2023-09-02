#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common/common.h"

#include "renderer/ray.h"
#include "renderer/ray_result.h"

namespace amts {
    struct Object {
        std::string m_name;
        Vec3f m_position;
        u64 m_materialId;

        Object(const std::string& name, const Vec3f position, const u64& materialId) 
            : m_name(name),
              m_position(position), 
              m_materialId(materialId) {

        }

        virtual ~Object() {}

        virtual RayResult hit(const Ray& ray) = 0;
    };
}

#endif