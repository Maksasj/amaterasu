#include "common_proxy.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

amts::CommonProxy::CommonProxy() 
    : m_rayRenderer(nullptr),
        m_scene(nullptr),
        m_mainCamera(nullptr),
        m_materialCollection(nullptr), 
        m_activeSkyTexture(nullptr) { 

}

amts::CommonProxy::~CommonProxy() {

}

void amts::CommonProxy::preinit() {

}

void amts::CommonProxy::init() {
    m_rayRenderer = std::make_unique<RayRenderer>();
    m_scene = std::make_unique<Scene>();
    m_mainCamera = std::make_unique<Camera>();
    m_materialCollection = std::make_unique<MaterialCollection>();

    m_workerPool = std::make_unique<WorkerPool>();
}

void amts::CommonProxy::load() {
    std::cout << "load started\n";
    // Materials
    m_materialCollection->m_materials.emplace_back(std::make_unique<Material>(
        "Red metallic",
        Color(1.0f, 0.0f, 0.0f),
        Color(0.0f, 0.0f, 0.0f),
        0.0f,
        0.1f
    ));

    m_materialCollection->m_materials.emplace_back(std::make_unique<Material>(
        "Blue diffuse",
        Color(0.0f, 0.0f, 1.0f),
        Color(0.0f, 0.0f, 0.0f),
        0.0f,
        1.0f
    ));

    m_materialCollection->m_materials.emplace_back(std::make_unique<Material>(
        "Sun",
        Color(0.99f, 0.32f, 0.0f),
        Color(0.99f, 0.32f, 0.0f),
        1.0f,
        0.8f
    ));

    m_materialCollection->m_materials.emplace_back(std::make_unique<Material>(
        "Gray floor",
        Color(0.3f, 0.3f, 0.3f),
        Color(0.0f, 0.0f, 0.0f),
        0.0f,
        1.0f
    ));

    // Scene objects
    /*
    m_scene->m_objects.emplace_back(std::make_unique<MarchingMandelbulb>(
        "Big red sphere", 
        Vec3f(0.0f, -1.5f, -2.5f), 
        1,
        0.75f
    ));
    */

    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Big red sphere", 
        Vec3f(0.0f, 0.0f, -2.5f), 
        1,
        0.75f
    ));

    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Blue sphere",
        Vec3f(1.0f, 0.0f, -2.0f),
        2,
        0.32f
    ));
    
    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Emissive sphere",
        Vec3f(-1.0f, 0.0f, -2.5f),
        3,
        0.32f
    ));
   
    m_scene->m_objects.emplace_back(std::make_unique<PlaneObject>(
        "Floor plane",
        Vec3f(0.0f, -0.5f, 0.0f),
        4,
        Vec3f(0.0f, -1.0f, 0.0f)
    ));

    m_activeSkyTexture = TextureBuffer<u32>::load_pixel_data_from_png_file("assets/images/fluela_wisshorn_spherical_panorama.png");
    std::cout << "load\n";
}

void amts::CommonProxy::run() {

}

void amts::CommonProxy::unload() {
    m_activeSkyTexture = nullptr;
}

void amts::CommonProxy::cleanup() {
    m_rayRenderer = nullptr;
    m_scene = nullptr;
    m_mainCamera = nullptr;
    m_materialCollection = nullptr;
}

void amts::CommonProxy::save_rendering_target_to_png(RenderingTarget* target, const std::string& fileName) {
    stbi_write_png(fileName.c_str(), target->get_width(), target->get_height(), 4, *target->get_pixel_data_ptr(), target->get_width() * sizeof(u32));
}
