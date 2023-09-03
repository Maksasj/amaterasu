#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "plane_object.h"
#include "sphere_object.h"

namespace amts {
    class Scene {
        private:
            bool m_modified;
        
        public:
            std::vector<std::unique_ptr<Object>> m_objects;

            const bool& is_modified() const {
                return m_modified;
            }

            void mark_as_modified() {
                m_modified = true;
            }

            void reset_modified_flag() {
                m_modified = false;
            }
    };
}

#endif