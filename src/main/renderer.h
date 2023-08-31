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
#include "utils.h"
#include "material_pool.h"

namespace amts {
    class RenderingTarget;

    class Renderer {
        private:
            SDL_Renderer* m_sdlRenderer; 
            u64 m_frame;

        public:
            Renderer(const std::unique_ptr<Window>& window);

            ~Renderer();

            RayResult trace_ray(const Ray& ray, const Scene& scene) const;
            Color per_pixel(const u64& x, const u64& y, const u64& width, const u64& height, const Scene& scene, const Camera& camera, const MaterialPool& materialPool) const;

            void render(
                const std::unique_ptr<RenderingTarget>& target,
                const std::unique_ptr<Scene>& scene,
                const std::unique_ptr<Camera>& camera,
                const std::unique_ptr<MaterialPool>& materialPool
            );

            void present_target(const std::unique_ptr<RenderingTarget>& target);
            void present();

            void clear(const Color& color);

            void reset_accumulation();

            SDL_Renderer* get_sdl_renderer();
    };
}

#endif