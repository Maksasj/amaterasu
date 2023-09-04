#include "ray_renderer.h"
#include "rendering_target.h"

amts::RayRenderer::RayRenderer::RayRenderer() 
    : m_frame(1) {

}

amts::RayRenderer::RayRenderer::~RayRenderer() {

}

amts::RayResult amts::RayRenderer::trace_ray(const Ray& ray, const Scene& scene) const {
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

amts::Color amts::RayRenderer::per_pixel(const u64& x, const u64& y, const u64& width, const u64& height, const Scene& scene, const Camera& camera, const MaterialCollection& materialPool) const {
    const f32 aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);

    Vec3f rayDirection = Vec3f(
        (x / static_cast<f32>(width) - 0.5f),
        (y / static_cast<f32>(height) - 0.5f) / aspectRatio, 
        -1.0f);
    Vec4f rotated = camera.get_rotation_matrix() * Vec4f(rayDirection.x, rayDirection.y, rayDirection.z, 1.0);
    rayDirection = Vec3f(rotated.x, rotated.y, rotated.z);

    Ray ray{camera.get_position(), rayDirection};

    Vec3f light = Vec3f::splat(0.0f);
    Vec3f contribution = Vec3f::splat(1.0f);

    const u64 maxBounces = 10;
    for(u64 b = 0; b < maxBounces; ++b) {
        const auto result = trace_ray(ray, scene);

        if(result.hitDistance < 0.0f) {
            const Vec3f skyColor = Vec3f(1.0f, 1.0f, 1.0f);
            light += skyColor * contribution;
            break;
        }

        const std::unique_ptr<Object>& object = scene.m_objects[result.objectId];
        const std::unique_ptr<Material>& material = materialPool.m_materials[object->m_materialId];  

        contribution *= material->m_albedo.to_vec3f();
        light += material->m_emissionColor.to_vec3f() * material->m_emissionStrength;

        ray.m_origin = result.hitPoint + result.hitNormal * 0.0001f;

        const Vec3f reflection = ray.m_direction - result.hitNormal * 2.0f * ray.m_direction.dot(result.hitNormal); 
        ray.m_direction = (reflection + Utils::random_in_unit_sphere() * material->m_metallic).normalize();
    }

    return Color(light.x, light.y, light.z);
}

void amts::RayRenderer::render(RenderingTarget* target, const std::unique_ptr<Scene>& scene, const std::unique_ptr<Camera>& camera, const std::unique_ptr<MaterialCollection>& materialPool) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to render" << std::endl);

    const auto targetWidth = target->get_width();
    const auto targetHeight = target->get_height();

    TextureBuffer<Color>& accumulatedColor = target->get_accumulated_color();

    for(u64 x = 0; x < targetWidth; ++x) {
        for(u64 y = 0; y < targetHeight; ++y) {
            const Color color = per_pixel(x, y, targetWidth, targetHeight, *scene, *camera, *materialPool).clamp(0.0f, 1.0f);

            Color& accumulatedPixel = accumulatedColor.get_pixel_at(x, y);
            accumulatedPixel += color;

            target->get_pixel_at(x, y) = (accumulatedPixel / static_cast<f32>(m_frame)).to_u32();
        }
    }

    ++m_frame;
}

void amts::RayRenderer::reset_accumulation() {
    m_frame = 1;
}

