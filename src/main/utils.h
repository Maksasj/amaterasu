#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>

#include "common.h"

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
    };
}

#endif