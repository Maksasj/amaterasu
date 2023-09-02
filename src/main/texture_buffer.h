#ifndef _TEXTURE_BUFFER_H_
#define _TEXTURE_BUFFER_H_

#include <memory>

#include "common/common.h"

namespace amts {
    template<typename pixelDataType>
    class TextureBuffer {
        private:
            void* m_pixelData;

            const u64 m_width;
            const u64 m_height;

        public:
            TextureBuffer(const u64& width, const u64& height) 
                : m_pixelData(nullptr),
                  m_width(width),
                  m_height(height) {

            }

            virtual ~TextureBuffer() {
                if(m_pixelData != nullptr)
                    free(m_pixelData);
            }

            void allocate() {
                m_pixelData = static_cast<pixelDataType*>(malloc(sizeof(pixelDataType) * m_width * m_height));
            }

            void** get_pixel_data_ptr() {
                return &m_pixelData;
            }

            pixelDataType& get_pixel_at(const u64& i) {
                return static_cast<pixelDataType*>(m_pixelData)[i];
            }

            pixelDataType& get_pixel_at(const u64& x, const u64& y) {
                return static_cast<pixelDataType*>(m_pixelData)[x + y * m_width];
            }

            const u64& get_width() const {
                return m_width;
            }

            const u64& get_height() const {
                return m_height;
            }

            u64 get_area() const {
                return m_width * m_height;
            }

    };
}

#endif