#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common/common.h"

#include "object_type.h"
#include "renderer/ray.h"
#include "renderer/ray_result.h"
#include "renderer/ray_renderer_profile.h"

namespace amts {
    struct Object {
        const ObjectType m_type;
        
        std::string m_name;
        Vec3f m_position;
        u64 m_materialId;

        Object(const ObjectType type, const std::string& name, const Vec3f position, const u64& materialId) 
            : m_type(type),
              m_name(name),
              m_position(position), 
              m_materialId(materialId) {

        }

        virtual ~Object() {}

        virtual RayResult hit(const RayRendererProfile& profile, const Ray& ray) = 0;
        virtual std::unique_ptr<Object> clone() = 0;
    };
}

#endif