#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "plane_object.h"
#include "sphere_object.h"

namespace amts {
    class Scene {
        private:
            bool m_modified;
            bool m_isRendering;
        
        public:
            std::vector<std::unique_ptr<Object>> m_objects;

            const bool& is_modified() const {
                return m_modified;
            }

            const bool& is_rendering() const {
                return m_isRendering;
            }

            void mark_as_modified() {
                m_modified = true;
            }

            void reset_modified_flag() {
                m_modified = false;
            }

            void mark_as_rendering() {
                m_isRendering = true;
            }

            void reset_rendering_flag() {
                m_isRendering = false;
            }
    };
}

#endif