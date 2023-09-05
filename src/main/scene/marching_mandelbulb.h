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

        template<typename T>
        static int sgn(T val) {
            return (T(0) < val) - (val < T(0));
        }

        static f32 atan_f(const f32& x, const f32& y) {
            return -sgn(x*y)*atan((abs(x)-abs(y))/(abs(x)+abs(y)));
        }
 
        f32 get_distance(const Vec3f& point) const {
            const static u64 iterations = 6;
            const static f32 power = 8.0f;

            Vec3f w = (point - m_position);
            float m = w.dot(w);

            Vec4f trap = Vec4f(abs(w.x), abs(w.y), abs(w.z), m);
            float dz = 1.0;

            for(u64 i = 0; i < iterations; ++i) {
                // trigonometric version

                // dz = 8*z^7*dz
                dz = 8.0 * pow(m, 3.5) * dz + 1.0;
                // dz = 8.0*pow(sqrt(m),7.0)*dz + 1.0;

                // z = z^8+z
                float r = w.length();
                float b = power * acos(w.y / r);
                float a = power * atan_f(w.x, w.z);
                w = (point - m_position) + Vec3f(sin(b) * sin(a), cos(b), sin(b) * cos(a)) * pow(r, 8.0);

                trap = min_f(trap, Vec4f(abs(w.x), abs(w.y), abs(w.z), m));

                m = w.dot(w);
                if (m > 256.0) break;
            }

            // res_color = vec4(m, trap.yzw);

            // distance estimation (through the Hubbard-Douady potential)
            return 0.25 * log(m) * sqrt(m) / dz;
        }

        RayResult hit(const Ray& ray) override {
            f32 d = 0.0f;

            const static u64 maxSteps = 1000;
            const static u64 maxDist = 10;
            const static f32 epsilon = 0.000001f;

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
            return std::make_unique<MarchingMandelbulb>(*this);
        }
    };
}

#endif