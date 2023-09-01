#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "common.h"

namespace amts {
    struct Material {
        std::string m_name;

        Color m_albedo;

        Color m_emissionColor;
        f32 m_emissionStrength;

        f32 m_metallic;

        Material(const std::string& name, const Color& albedo, const Color& emissionColor, const f32& emissionStrength, const f32& metallic) 
            : m_name(name),
              m_albedo(albedo),
              m_emissionColor(emissionColor),
              m_emissionStrength(emissionStrength),
              m_metallic(metallic) {

        }
    };
}

#endif