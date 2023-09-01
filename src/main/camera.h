#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <unordered_map>
#include "common/common.h"

#include "window.h"

namespace amts {
    class Camera {
        private:
            Vec3f m_position;
            Vec3f m_direction;
            Vec3f m_rotation;
            Mat4f m_rotationMatrix;

        public:
            friend class CameraController;

            Camera() 
                : m_position(Vec3f::splat(0.0f)), 
                  m_direction(0.0f, 0.0f, -1.0f),
                  m_rotation(Vec3f::splat(0.0f)),
                  m_rotationMatrix(Mat4f::identity()) {

            }

            const Vec3f& get_position() const {
                return m_position;
            }

            const Mat4f& get_rotation_matrix() const {
                return m_rotationMatrix;
            }
    };
}

#endif