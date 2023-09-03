#ifndef _SPHERE_OBJECT_H_
#define _SPHERE_OBJECT_H_

#include "object.h"

namespace amts {
    struct SphereObject : public Object {
        f32 m_radius;

        SphereObject(const std::string& name, const Vec3f position, const u64& materialId, const f32 radius) 
            : Object(SPHERE, name, position, materialId), 
              m_radius(radius) {

        } 

        RayResult hit(const Ray& ray) override {
            const auto oc = ray.m_origin - m_position;

            const f32 a = ray.m_direction.dot(ray.m_direction);
            const f32 b = 2.0f * ray.m_direction.dot(oc);
            const f32 c = oc.dot(oc) - m_radius*m_radius;

            const f32 discriminant = b*b - 4*a*c;

            if(discriminant >= 0) {
                f32 d0 = (-b + sqrt(discriminant)) / (2.0f * a);
                f32 d1 = (-b - sqrt(discriminant)) / (2.0f * a);

                if(d0 > d1) std::swap(d0, d1);

                if(d0 < 0.0f) {
                    d0 = d1;
                    if(d0 < 0) return RayResult::invalid;
                }

                const f32 d = d0;

                const Vec3f collisionPoint = ray.m_origin + ray.m_direction*(d * 0.99f);
                const Vec3f normal = (collisionPoint - m_position).normalize();

                return RayResult{d, collisionPoint, normal, 0};
            }

            return RayResult::invalid;
        }

        std::unique_ptr<Object> clone() override {
            return std::make_unique<SphereObject>(*this);
        }
    };
}

#endif