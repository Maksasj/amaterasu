#ifndef _RENDERERING_TARGET_H_
#define _RENDERERING_TARGET_H_

#include <memory>

#include "common/common.h"
#include "texture_buffer.h"

namespace amts {
    class RenderingTarget : public TextureBuffer<u32> {
        private:
            bool m_locked;
            TextureBuffer<Color> m_accumulatedColor;

        public:
            RenderingTarget(const u64& width, const u64& height) 
                : TextureBuffer(width, height), 
                  m_locked(false),
                  m_accumulatedColor(width, height) {
                
                m_accumulatedColor.allocate();
            }

            ~RenderingTarget() {

            }

            virtual void lock() {
                m_locked = true;
            }

            virtual void unlock() {
                m_locked = false;
            }

            TextureBuffer<Color>& get_accumulated_color() {
                return m_accumulatedColor;
            }

            void reset_accumulation(const Color& resetColor = Color(0.0f, 0.0f, 0.0f)) {
                for(u64 i = 0; i < get_area(); ++i)
                    m_accumulatedColor.get_pixel_at(i) = resetColor;
            }

            const bool& is_locked() const {
                return m_locked;
            }
    };
}

#endif