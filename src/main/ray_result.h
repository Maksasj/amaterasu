#ifndef _RAY_RESULT_H_
#define _RAY_RESULT_H_

#include "types.h"

namespace amts {
    struct RayResult {
        f32 hitDistance;
        Vec3f hitPoint;
        Vec3f hitNormal;

        u64 objectId;

        const static RayResult invalid;

        bool is_valid() const {
            return hitDistance > 0.0f;
        }
    };
}


#endif