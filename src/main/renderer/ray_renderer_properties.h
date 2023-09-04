#ifndef _RAY_RENDERER_PROPERTIES_H_
#define _RAY_RENDERER_PROPERTIES_H_

namespace amts {
    struct RayRendererProperties {
        bool m_enableAntiAliasing = true;
        f32 m_antiAliasingFactor = 0.00125f;
    };
}

#endif