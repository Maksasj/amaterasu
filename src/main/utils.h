#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>

#include "common/common.h"

#define OMNI_TYPES_PI 3.14159265f

namespace amts {
    struct Utils {
        static f32 random_float() {
            return static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);
        }

        static Vec3f random_in_unit_sphere() {
            const f32 x = random_float() * 2.0f - 1.0f;
            const f32 y = random_float() * 2.0f - 1.0f;
            const f32 z = random_float() * 2.0f - 1.0f;

            return Vec3f(x, y, z).normalize();
        }

        // Todo, move this function to omni::types
        template<typename T>
        static int sgn(T val) {
            return (T(0) < val) - (val < T(0));
        }

        static f32 atan_f(const f32& x, const f32& y) {
            return -sgn(x*y)*atan((abs(x)-abs(y))/(abs(x)+abs(y)));
        }
    };
}

#endif