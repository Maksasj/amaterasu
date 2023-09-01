#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common/common.h"

#include "renderer/ray.h"
#include "renderer/ray_result.h"

namespace amts {
    struct Object {
        Vec3f m_position;
        u64 m_materialId;

        Object(const Vec3f _pos, const u64& materialId) : m_position(_pos), m_materialId(materialId) {

        }

        virtual ~Object() {}

        virtual RayResult hit(const Ray& ray) = 0;
    };

    struct SphereObject : public Object {
        f32 m_radius;

        SphereObject(const Vec3f _pos, const f32 _radius, const u64& materialId) : Object(_pos, materialId), m_radius(_radius) {

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
    };

    struct PlaneObject : public Object {
        Vec3f normal;

        PlaneObject(const Vec3f _pos, const u64 materialId) : Object(_pos, materialId) {
            normal = Vec3f(0.0f, -1.0f, 0.0f);
        } 

        RayResult hit(const Ray& ray) override {
            f32 denom = m_position.dot(normal);

            if (abs(denom) <= 1e-4f)
                return RayResult::invalid;

            f32 t = -(ray.m_origin.dot(normal) + denom) / ray.m_direction.dot(normal);

            if (t <= 1e-4)
                return RayResult::invalid;

            const Vec3f collisionPoint = ray.m_origin + ray.m_direction * t;

            return RayResult{t, collisionPoint, normal, 0};
        }
    };
}

#endif