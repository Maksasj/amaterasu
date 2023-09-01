#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include <memory>
#include <unordered_map>

#include "common.h"
#include "camera.h"

namespace amts {
    class CameraController {
        private:
            std::unordered_map<i32, bool> m_keys;
            bool m_mouseLocked;

            bool m_moved;

        public:  
            CameraController() {
                m_mouseLocked = false;
                m_moved = false;
            }

            ~CameraController() {

            }
            
            void update_input(SDL_Event& event) {
                switch (event.type) {
                    case SDL_EVENT_KEY_DOWN:
                        m_keys[event.key.keysym.sym] = true;
                        break;
                    case SDL_EVENT_KEY_UP:
                        m_keys[event.key.keysym.sym] = false;
                        break;
                    default:
                        break;
                }
            }

            void move_camera(std::unique_ptr<Camera>& cameraPtr, std::unique_ptr<Window>& window) {
                auto& camera = *cameraPtr;

                if (m_keys[SDLK_w])
                    camera.m_position -= Vec3f(camera.m_direction.z, 0.0f, camera.m_direction.x).normalize() * 0.02f;

                if (m_keys[SDLK_s])
                    camera.m_position += Vec3f(camera.m_direction.z, 0.0f, camera.m_direction.x).normalize() * 0.02f;

                if (m_keys[SDLK_a])
                    camera.m_position -= Vec3f(1.0f, 0.0f, -1.0f) * camera.m_direction.normalize() * 0.02f;

                if (m_keys[SDLK_d])
                    camera.m_position += Vec3f(1.0f, 0.0f, -1.0f) * camera.m_direction.normalize() * 0.02f;

                if (m_keys[SDLK_SPACE])
                    camera.m_position.y += 0.1f;

                if (m_keys[SDLK_LSHIFT])
                    camera.m_position.y -= 0.1f;

                bool oldMouseLockState = m_moved;
                static bool buttonPressed = false;
                const bool isDebugButtonPressed = m_keys[SDLK_t];
                m_moved = (isDebugButtonPressed && !buttonPressed) ? !m_moved : m_moved;  
                buttonPressed = !isDebugButtonPressed ? false : true;
                
                const f64 centerXPos = window->get_width() / 2.0f;
                const f64 centerYPos = window->get_height() / 2.0f;

                if(oldMouseLockState != m_moved) {
                    SDL_WarpMouseInWindow(window->get_sdl_window(), centerXPos, centerYPos);
                }

                if(m_moved == true) {
                    f32 xPos, yPos;
                    SDL_GetMouseState(&xPos, &yPos);
                    
                    const f32 deltaX = floor(centerXPos) - xPos;
                    const f32 deltaY = floor(centerYPos) - yPos;

                    camera.m_rotation.x += deltaY * 0.005f;
                    camera.m_rotation.y += deltaX * 0.005f;

                    SDL_WarpMouseInWindow(window->get_sdl_window(), centerXPos, centerYPos);
                    m_moved = true;
                }

                camera.m_direction.x = cos(camera.m_rotation.y) * cos(camera.m_rotation.x);
                camera.m_direction.y = sin(camera.m_rotation.x);
                camera.m_direction.z = sin(camera.m_rotation.y) * cos(camera.m_rotation.x);
                camera.m_direction.normalize();

                camera.m_rotationMatrix = trait_bryan_angle_yxz(camera.m_rotation);
            }

            const bool& is_moved() const {
                return m_moved;
            }

            void reset_move_flag() {
                m_moved = false;
            }
    };
}

#endif