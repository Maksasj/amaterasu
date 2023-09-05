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

            const f32& r() const { return m_values.x; }
            const f32& g() const { return m_values.y; }
            const f32& b() const { return m_values.z; }
            const f32& a() const { return m_values.w; }

            Color& clamp(const f32& min, const f32& max) {
                m_values.clamp(min, max);

                return *this;
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

            Color operator+(const f32& value) const {
                return Color(this->m_values + value);
            }

            Color operator-(const f32& value) const {
                return Color(this->m_values - value);
            }

            Color operator*(const f32& value) const {
                return Color(this->m_values * value);
            }

            Color operator/(const f32& value) const {
                return Color(this->m_values / value);
            }

            inline Color& operator+=(const Color& other) {
                m_values += other.m_values;
                return *this;
            }

            inline Color& operator-=(const Color& other) {
                m_values -= other.m_values;
                return *this;
            }

            inline Color& operator*=(const Color& other) {
                m_values *= other.m_values;
                return *this;
            }

            inline Color& operator/=(const Color& other) {
                m_values /= other.m_values;
                return *this;
            }

            inline bool operator==(const Color& other) {
                return m_values == other.m_values;
            }

            inline bool operator!=(const Color& other) {
                return m_values != other.m_values;
            }

            u32 to_u32() const {
                const u8 r = static_cast<u8>(m_values.x * 255.0f);
                const u8 g = static_cast<u8>(m_values.y * 255.0f);
                const u8 b = static_cast<u8>(m_values.z * 255.0f);
                const u8 a = static_cast<u8>(m_values.w * 255.0f);

                return (a << 24) | (b << 16) | (g << 8) | (r << 0);
            }

            Vec3f to_vec3f() const {
                return Vec3f(m_values.x, m_values.y, m_values.z);
            }

            static Color from_vec4f(const u32& color) {
                return Color(
                    (color & 0xFF)          / 255.0f,
                    ((color >> 8) & 0xFF)   / 255.0f,
                    ((color >> 16) & 0xFF)  / 255.0f,
                    ((color >> 24) & 0xFF)  / 255.0f);
            }
    };
}

#endif