#include "renderer.h"
#include "rendering_target.h"

amts::Renderer::Renderer::Renderer(const std::unique_ptr<Window>& window) : m_sdlRenderer(nullptr) {
    m_sdlRenderer = SDL_CreateRenderer(window->get_sdl_window(), NULL, SDL_RENDERER_ACCELERATED);
}

amts::Renderer::Renderer::~Renderer() {
    SDL_DestroyRenderer(m_sdlRenderer);
}

amts::RayResult amts::Renderer::trace_ray(const Ray& ray, const Scene& scene) const {
    f32 depth = std::numeric_limits<f32>::max();
    RayResult rayResult = RayResult::invalid;
    bool validCollision = false;

    for(u64 i = 0; i < scene.m_objects.size(); ++i) {
        const RayResult result = scene.m_objects[i]->hit(ray);

        if(!result.is_valid())
            continue;

        if(result.hitDistance > depth)
            continue;

        depth = result.hitDistance;
        rayResult = result;
        rayResult.objectId = i;
        validCollision = true;
    }

    if(validCollision)
        return rayResult;

    return RayResult::invalid;
}

amts::Color amts::Renderer::per_pixel(const u64& x, const u64& y, const u64& width, const u64& height, const Scene& scene, const std::unique_ptr<Camera>& camera) const {
    const f32 aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);

    Vec3f rayDirection = Vec3f(
        (x / static_cast<f32>(width) - 0.5f),
        (y / static_cast<f32>(height) - 0.5f) / aspectRatio, 
        -1.0f);
    Vec4f rotated = camera->get_rotation_matrix() * Vec4f(rayDirection.x, rayDirection.y, rayDirection.z, 1.0);
    rayDirection = Vec3f(rotated.x, rotated.y, rotated.z);

    Ray ray{camera->get_position(), rayDirection};

    Vec3f light = Vec3f::splat(0.0f);
    Vec3f contribution = Vec3f::splat(1.0f);

    const u64 maxBounces = 3;
    for(u64 b = 0; b < maxBounces; ++b) {
        const auto result = trace_ray(ray, scene);

        if(result.hitDistance < 0.0f) {
            const Vec3f skyColor = Vec3f(0.6f, 0.6f, 0.9f);
            light += skyColor * contribution;
            break;
        }

        contribution *= scene.m_objects[result.objectId]->get_contribution();

        ray.m_origin = result.hitPoint + result.hitNormal * 0.0001f;
        ray.m_direction = result.hitNormal.normalize();
    }

    return Color(light.x, light.y, light.z);
}

void amts::Renderer::render(const std::unique_ptr<RenderingTarget>& target, const std::unique_ptr<Scene>& scene, const std::unique_ptr<Camera>& camera) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to render" << std::endl);

    const auto targetWidth = target->get_width();
    const auto targetHeight = target->get_height();

    u32* pixels = target->get_pixel_data();

    for(u64 x = 0; x < targetWidth; ++x) {
        for(u64 y = 0; y < targetHeight; ++y) {
            const Color color = per_pixel(x, y, targetWidth, targetHeight, *scene, camera).clamp(0.0f, 1.0f);

            pixels[x + y * targetWidth] = color.to_u32();
        }
    }
}

void amts::Renderer::present_target(const std::unique_ptr<RenderingTarget>& target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to do present" << std::endl);

    SDL_RenderTexture(m_sdlRenderer, target->get_sdl_texture(), &target->get_texture_rect_f(), &target->get_texture_rect_f());
    SDL_RenderPresent(m_sdlRenderer);
}

SDL_Renderer* amts::Renderer::get_sdl_renderer() {
    return m_sdlRenderer;
}
