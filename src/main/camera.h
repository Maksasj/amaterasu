#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <unordered_map>
#include "common.h"

#include "window.h"

namespace amts {
    class Camera {
        private:
            Vec3f m_position;
            Vec3f m_direction;
            Vec3f m_rotation;
            Mat4f m_rotationMatrix;

            std::unordered_map<i32, bool> KEYS;
            bool mouseLocked;

        public:
            Camera() 
                : m_position(Vec3f::splat(0.0f)), 
                  m_direction(0.0f, 0.0f, -1.0f),
                  m_rotation(Vec3f::splat(0.0f)),
                  m_rotationMatrix(Mat4f::identity()) {
                
                mouseLocked = false;
            }

            const Vec3f& get_position() const {
                return m_position;
            }

            const Mat4f& get_rotation_matrix() const {
                return m_rotationMatrix;
            }

            void update_input(SDL_Event& event) {
                switch (event.type) {
                    case SDL_EVENT_KEY_DOWN:
                        KEYS[event.key.keysym.sym] = true;
                        break;
                    case SDL_EVENT_KEY_UP:
                        KEYS[event.key.keysym.sym] = false;
                        break;
                    default:
                        break;
                }
            }

            bool update_state(std::unique_ptr<Window>& window) {
                bool moved = false;

                if (KEYS[SDLK_w]) {
                    m_position -= Vec3f(m_direction.z, 0.0f, m_direction.x).normalize() * 0.02f;
                    moved = true;
                }

                if (KEYS[SDLK_s]) {
                    m_position += Vec3f(m_direction.z, 0.0f, m_direction.x).normalize() * 0.02f;
                    moved = true;
                }

                if (KEYS[SDLK_a]) {
                    m_position -= Vec3f(1.0f, 0.0f, -1.0f) * m_direction.normalize() * 0.02f;
                    moved = true;
                }

                if (KEYS[SDLK_d]) {
                    m_position += Vec3f(1.0f, 0.0f, -1.0f) * m_direction.normalize() * 0.02f;
                    moved = true;
                }

                if (KEYS[SDLK_SPACE]) {
                    m_position.y += 0.1f;
                }

                if (KEYS[SDLK_LSHIFT]) {
                    m_position.y -= 0.1f;
                }

                bool oldMouseLockState = mouseLocked;
                static bool buttonPressed = false;
                const bool isDebugButtonPressed = KEYS[SDLK_t];
                mouseLocked = (isDebugButtonPressed && !buttonPressed) ? !mouseLocked : mouseLocked;  
                buttonPressed = !isDebugButtonPressed ? false : true;
                
                const f64 centerXPos = window->get_width() / 2.0f;
                const f64 centerYPos = window->get_height() / 2.0f;

                if(oldMouseLockState != mouseLocked) {
                    SDL_WarpMouseInWindow(window->get_sdl_window(), centerXPos, centerYPos);
                }

                if(mouseLocked == true) {
                    f32 xPos, yPos;
                    SDL_GetMouseState(&xPos, &yPos);
                    
                    const f32 deltaX = floor(centerXPos) - xPos;
                    const f32 deltaY = floor(centerYPos) - yPos;

                    m_rotation.x += deltaY * 0.005f;
                    m_rotation.y += deltaX * 0.005f;

                    SDL_WarpMouseInWindow(window->get_sdl_window(), centerXPos, centerYPos);
                    moved = true;
                }

                m_direction.x = cos(m_rotation.y) * cos(m_rotation.x);
                m_direction.y = sin(m_rotation.x);
                m_direction.z = sin(m_rotation.y) * cos(m_rotation.x);
                m_direction.normalize();

                m_rotationMatrix = trait_bryan_angle_yxz(m_rotation);

                return moved;
            }
    };
}

#endif