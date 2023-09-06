#ifndef _PLANE_OBJECT_H_
#define _PLANE_OBJECT_H_

#include "object.h"

namespace amts {
    struct PlaneObject : public Object {
        Vec3f m_normal;

        PlaneObject(const std::string& name, const Vec3f position, const u64 materialId, const Vec3f& normal) 
            : Object(PLANE, name, position, materialId),
              m_normal(normal) {
            
        }

        RayResult hit(const RayRendererProfile& profile, const Ray& ray) override {
            f32 denom = m_position.dot(m_normal);

            if(abs(denom) <= 1e-4f)
                return RayResult::invalid;

            f32 t = -(ray.m_origin.dot(m_normal) + denom) / ray.m_direction.dot(m_normal);

            if(t <= 1e-4)
                return RayResult::invalid;

            const Vec3f collisionPoint = ray.m_origin + ray.m_direction * t;

            return RayResult{t, collisionPoint, m_normal, 0};
        }

        std::unique_ptr<Object> clone() override {
            return std::make_unique<PlaneObject>(*this);
        }
    };
}

#endif