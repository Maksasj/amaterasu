#ifndef _MARCHING_MANDELBULB_OBJECT_H_
#define _MARCHING_MANDELBULB_OBJECT_H_

#include <cmath>

#include "object.h"

namespace amts {
    struct MarchingMandelbulb : public Object {
        f32 m_radius;

        MarchingMandelbulb(const std::string& name, const Vec3f position, const u64& materialId, const f32 radius) 
            : Object(MARCHING_SPHERE, name, position, materialId), 
              m_radius(radius) {

        } 

        static Vec4f min_f(const Vec4f& a, const Vec4f& b) {
            return Vec4f(
                std::min(a.x, b.x),
                std::min(a.y, b.y),
                std::min(a.z, b.z),
                std::min(a.w, b.w)
            );
        }
 
        f32 get_distance(const Vec3f& point) const {
            const static u64 iterations = 12;
            const static f32 power = 8.0f;

            Vec3f w = (point - m_position);
            f32 m = w.dot(w);

            Vec4f trap = Vec4f(abs(w.x), abs(w.y), abs(w.z), m);
            f32 dz = 1.0f;

            for(u64 i = 0; i < iterations; ++i) {
                // trigonometric version

                // dz = 8*z^7*dz
                dz = 8.0f * pow(m, 3.5) * dz + 1.0f;
                // dz = 8.0*pow(sqrt(m),7.0)*dz + 1.0;

                // z = z^8+z
                f32 r = w.length();
                f32 b = power * acos(w.y / r);
                f32 a = power * atan2f(w.x, w.z);
                w = (point - m_position) + Vec3f(sin(b) * sin(a), cos(b), sin(b) * cos(a)) * pow(r, 8.0f);

                trap = min_f(trap, Vec4f(abs(w.x), abs(w.y), abs(w.z), m));

                m = w.dot(w);
                if (m > 256.0f) break;
            }

            // res_color = vec4(m, trap.yzw);

            // distance estimation (through the Hubbard-Douady potential)
            return 0.25f * log(m) * sqrt(m) / dz;
        }

        RayResult hit(const RayRendererProfile& profile, const Ray& ray) override {
            f32 d = 0.0f;

            for(u64 i = 0; i < profile.m_rayMarchingMaxSteps; ++i) {
                Vec3f p = ray.m_origin + ray.m_direction.normalize() * d;
                f32 ds = get_distance(p);
                d += ds;

                if(d > profile.m_rayMarchingMaxDist)
                    return RayResult::invalid;

                if(ds < profile.m_rayMarchingEpsilon)
                    return RayResult{d, p, (p - m_position).normalize(), 0};
            }

            return RayResult::invalid;
        }

        std::unique_ptr<Object> clone() override {
            return std::make_unique<MarchingMandelbulb>(*this);
        }
    };
}

#endif