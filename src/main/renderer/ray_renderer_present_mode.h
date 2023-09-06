#ifndef _RAY_RENDERER_PRESENT_MODE_H_
#define _RAY_RENDERER_PRESENT_MODE_H_

namespace amts {
    enum RayRendererPresentMode {
        IMMEDIATE_ACCUMULATION,
        TILED_ACCUMULATION
    };

    static std::string present_mode_to_string(const RayRendererPresentMode& mode) {
        switch (mode) {
            case IMMEDIATE_ACCUMULATION: return "Immediate Accumulation";
            case TILED_ACCUMULATION: return "Tiled Accumulation";
        }

        return "Undefined";    
    }
}

#endif