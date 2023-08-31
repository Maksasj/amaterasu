#ifndef _AMATERASU_H_
#define _AMATERASU_H_

#include <memory>

#include "common.h"
#include "renderer.h"
#include "window.h"
#include "rendering_target.h"

namespace amts {
    class Amaterasu {
        private:
            bool m_close;

            std::unique_ptr<Window> m_window;
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RenderingTarget> m_target;

        public:
            Amaterasu() 
                : m_close(false),
                  m_window(nullptr),
                  m_renderer(nullptr),
                  m_target(nullptr) {

            }

            ~Amaterasu() {

            }

            void preinit() {
                SDL_Init(SDL_INIT_VIDEO);
            }

            void init() {
                m_window = std::make_unique<Window>("Amaterasu", 800, 600);
                m_renderer = std::make_unique<Renderer>(m_window);
                m_target = std::make_unique<RenderingTarget>(m_renderer, 800, 600);
            }
            
            void load() {

            }

            void run() {
                while (!m_close) {
                    SDL_Event event;

                    while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                            case SDL_EVENT_QUIT:
                                m_close = true;
                                break;
                            // case SDL_EVENT_KEY_DOWN:
                            //     KEYS[event.key.keysym.sym] = true;
                            //     break;
                            // case SDL_EVENT_KEY_UP:
                            //     KEYS[event.key.keysym.sym] = false;
                            //     break;
                            default:
                                break;
                        }
                    }
                    
                    m_target->lock();
                    m_renderer->render(m_target);
                    m_target->unlock();

                    m_renderer->present_target(m_target);
                }
            }

            void unload() {

            }

            void cleanup() {
                m_target = nullptr;
                m_renderer = nullptr;
                m_window = nullptr;

                SDL_Quit();
            }
    };
}

#endif