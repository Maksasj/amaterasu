#ifndef _RAY_RENDERER_PROFILE_H_
#define _RAY_RENDERER_PROFILE_H_

#include "common/common.h"

#define HIGH_IMPACT
#define LOW_IMPACT
#define NO_IMPACT

namespace amts {
    struct RayRendererProfile {
        // Rendering
        HIGH_IMPACT u64 m_maxBounces; // Done
        NO_IMPACT   f32 m_reflectionNormalOffset; // Done
        HIGH_IMPACT bool m_enableReflection; // Done
        HIGH_IMPACT bool m_enableEmission; // Done
        LOW_IMPACT  bool m_enableSkybox; // Done
        HIGH_IMPACT bool m_enableRayDirectionCaching;

        // Ray marching
        HIGH_IMPACT u64 m_rayMarchingMaxSteps; // Done
        HIGH_IMPACT u64 m_rayMarchingMaxDist; // Done
        HIGH_IMPACT f32 m_rayMarchingEpsilon; // Done

        // Anti alliasing
        LOW_IMPACT  bool m_enableAntiAliasing; // Done
        LOW_IMPACT  f32 m_antiAliasingFactor; // Done

        // Present
        NO_IMPACT   u64 m_presentMode; // u64 should be changet to enum
        HIGH_IMPACT bool m_multiThreading;
        NO_IMPACT   u64 m_samplesPerPixel;

        static RayRendererProfile create_default_renderer_profile() {
            RayRendererProfile profile;

            // Rendering
            profile.m_maxBounces = 10;
            profile.m_reflectionNormalOffset = 0.0001f;
            profile.m_enableReflection = true;
            profile.m_enableEmission = true;
            profile.m_enableSkybox = true;
            profile.m_enableRayDirectionCaching = false;

            // Ray marching
            profile.m_rayMarchingMaxSteps = 1000;
            profile.m_rayMarchingMaxDist = 10;
            profile.m_rayMarchingEpsilon = 0.000001f;

            // Anti alliasing
            profile.m_enableAntiAliasing = true;
            profile.m_antiAliasingFactor = 0.00125f;

            // Present
            profile.m_presentMode = 0;
            profile.m_multiThreading = true;
            profile.m_samplesPerPixel = 0;

            return profile;
        }
    };
}

#endif