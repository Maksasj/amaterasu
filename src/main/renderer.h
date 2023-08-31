#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cassert>
#include <memory>

#include "common.h"
#include "window.h"
#include "ray.h"
#include "ray_result.h"
#include "scene.h"
#include "camera.h"

namespace amts {
    class RenderingTarget;

    class Renderer {
        private:
            SDL_Renderer* m_sdlRenderer; 

        public:
            Renderer(const std::unique_ptr<Window>& window);

            ~Renderer();

            RayResult trace_ray(const Ray& ray, const Scene& scene) const;
            Color per_pixel(const u64& x, const u64& y, const u64& width, const u64& height, const Scene& scene, const std::unique_ptr<Camera>& camera) const;

            void render(const std::unique_ptr<RenderingTarget>& target, const std::unique_ptr<Scene>& scene, const std::unique_ptr<Camera>& camera);

            void present_target(const std::unique_ptr<RenderingTarget>& target);

            SDL_Renderer* get_sdl_renderer();
    };
}

#endif