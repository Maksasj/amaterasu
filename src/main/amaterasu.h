#ifndef _AMATERASU_H_
#define _AMATERASU_H_

#include <memory>

#include "common.h"
#include "renderer.h"

namespace amts {
    class Amaterasu {
        private:
            bool m_close;

            std::unique_ptr<Window> m_window;
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RenderingTarget> m_target;

        public:
            Amaterasu() 
                : m_close(false) {

            }

            ~Amaterasu() {

            }

            void preinit() {
                SDL_Init(SDL_INIT_VIDEO);
            }

            void init() {
                SDL_Window* win = SDL_CreateWindow("Hello World", WINDOW_WIDTH, WINDOW_HEIGHT, 0);

            
                m_window = std::make_unique<Window>("Amaterasu", 800, 600);

                m_renderer = std::make_unique<Renderer>(m_window);
                m_target = std::make_unique<RendereringTarget>(m_renderer);
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
                    
                    const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
                    const SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

                    m_target.lock();
                    m_renderer->render(m_target);
                    m_target.unlock();

                    for(i32 xW = 0; xW < WINDOW_WIDTH; ++xW) {
                        for(i32 yH = 0; yH < WINDOW_HEIGHT; ++yH) {
                            
                        }
                    }

                    SDL_UnlockTexture(texture);
                    
                    SDL_RenderTexture(renderer, texture, &window_rect_f, &window_rect_f);
                    SDL_RenderPresent(renderer);
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