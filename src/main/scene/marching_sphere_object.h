#ifndef _MARCHING_SPHERE_OBJECT_H_
#define _MARCHING_SPHERE_OBJECT_H_

#include "object.h"

namespace amts {
    struct MarchingSphereObject : public Object {
        f32 m_radius;

        MarchingSphereObject(const std::string& name, const Vec3f position, const u64& materialId, const f32 radius) 
            : Object(MARCHING_SPHERE, name, position, materialId), 
              m_radius(radius) {

        } 

        f32 get_distance(const Vec3f& point) const {
            const f32 sphere_dist = (m_position - point).length() - m_radius;
            return sphere_dist;
        }

        RayResult hit(const Ray& ray) override {
            f32 d = 0.0f;

            const static u64 maxSteps = 100;
            const static u64 maxDist = 10;
            const static f32 epsilon = 0.00001f;

            for(u64 i = 0; i < maxSteps; ++i) {
                Vec3f p = ray.m_origin + ray.m_direction.normalize() * d;
                f32 ds = get_distance(p);
                d += ds;

                if(d > maxDist) {
                    return RayResult::invalid;
                }

                if(ds < epsilon) {
                    return RayResult{d, p, (p - m_position).normalize(), 0};
                }
            }

            return RayResult::invalid;
        }

        std::unique_ptr<Object> clone() override {
            return std::make_unique<MarchingSphereObject>(*this);
        }
    };
}

#endif