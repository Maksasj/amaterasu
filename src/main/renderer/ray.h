#ifndef _RAY_H_
#define _RAY_H_

#include "types.h"

namespace amts {
    struct Ray {
        Vec3f m_origin;
        Vec3f m_direction;
    };
}

#endif