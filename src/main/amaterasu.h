#ifndef _AMATERASU_H_
#define _AMATERASU_H_

#include <memory>

#include "common.h"
#include "renderer.h"
#include "window.h"
#include "rendering_target.h"
#include "material_pool.h"

namespace amts {
    class Amaterasu {
        private:
            bool m_close;

            std::unique_ptr<Window> m_window;
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RenderingTarget> m_target;
            std::unique_ptr<Scene> m_scene;
            std::unique_ptr<Camera> m_mainCamera;
            std::unique_ptr<MaterialPool> m_materialPool;

        public:
            Amaterasu() 
                : m_close(false),
                  m_window(nullptr),
                  m_renderer(nullptr),
                  m_target(nullptr),
                  m_scene(nullptr),
                  m_mainCamera(nullptr),
                  m_materialPool(nullptr) {

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
                
                m_scene = std::make_unique<Scene>();
                m_materialPool = std::make_unique<MaterialPool>();

                m_mainCamera = std::make_unique<Camera>();
            }
            
            void load() {
                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Red metallic",
                    Color(1.0f, 0.0f, 0.0f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    0.1f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Blue diffuse",
                    Color(0.0f, 0.0f, 1.0f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    1.0f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Sun",
                    Color(0.99f, 0.32f, 0.0f),
                    Color(0.99f, 0.32f, 0.0f),
                    1.0f,
                    0.8f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Gray floor",
                    Color(0.3f, 0.3f, 0.3f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    1.0f
                ));

                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(0.0f, 0.0f, -2.5f),  0.75f, 0));
                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(1.0f, 0.0f, -2.0f),  0.32f, 1));
                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(-1.0f, 0.0f, -2.5f), 0.32f, 2));
                m_scene->m_objects.emplace_back(std::make_unique<PlaneObject>(Vec3f(0.0f, -0.5f, 0.0f), 3));
            }

            void run() {
                while (!m_close) {
                    SDL_Event event;

                    while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                            case SDL_EVENT_QUIT:
                                m_close = true;
                                break;
                            default:
                                break;
                        }

                        m_mainCamera->update_input(event);
                    }

                    if(m_mainCamera->update_state(m_window)) {
                        m_target->reset_accumulation();
                        m_renderer->reset_accumulation();
                    }
                    
                    m_target->lock();
                    m_renderer->render(m_target, m_scene, m_mainCamera, m_materialPool);
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

                m_scene = nullptr;
                m_materialPool = nullptr;

                m_mainCamera = nullptr;

                SDL_Quit();
            }
    };
}

#endif