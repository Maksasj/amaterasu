#ifndef _COLOR_H_
#define _COLOR_H_

#include "types.h"

namespace amts {
    class Color {
        private:
            Vec4f m_values;

        public:
            Color(const Vec4f& values) 
                : m_values(values) {

            }

            Color(const f32& r, const f32& g, const f32& b) 
                : m_values(r, g, b, 1.0f) {

            }

            Color(const f32& r, const f32& g, const f32& b, const f32& a) 
                : m_values(r, g, b, a) {

            }

            Color operator+(const Color& col) const {
                return Color(this->m_values + col.m_values);
            }

            Color operator-(const Color& col) const {
                return Color(this->m_values - col.m_values);
            }

            Color operator*(const Color& col) const {
                return Color(this->m_values * col.m_values);
            }

            Color operator/(const Color& col) const {
                return Color(this->m_values / col.m_values);
            }

            u32 to_u32() const {
                const u8 r = static_cast<u8>(m_values.x * 255.0f);
                const u8 g = static_cast<u8>(m_values.y * 255.0f);
                const u8 b = static_cast<u8>(m_values.z * 255.0f);
                const u8 a = static_cast<u8>(m_values.w * 255.0f);

                return (a << 24) | (b << 16) | (g << 8) | (r << 0);
            }
    };
}

#endif