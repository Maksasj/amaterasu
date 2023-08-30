#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <functional>

#include <omni_types.tpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

using namespace omni::types;

struct RayCollisionData {
    bool collision;

    Vec3f point = Vec3f::splat(0.0f);
    Vec3f direction_after = Vec3f::splat(0.0f);
    u32 color = 0xFF000000;
};

struct Ray {
    Vec3f origin;
    Vec3f direction;
};

struct Object {
    Vec3f pos;

    Object(const Vec3f _pos) : pos(_pos) {

    }

    virtual ~Object() {}
    virtual RayCollisionData hit(const Ray& ray) = 0;
};

struct SphereObject : public Object {
    f32 radius;
    u32 color;

    SphereObject(const Vec3f _pos, u32 _color, const f32 _radius) : Object(_pos), radius(_radius) {
        color = _color;
    } 

    RayCollisionData hit(const Ray& ray) override {
        const auto oc = ray.origin - pos;

        const f32 a = ray.direction.dot(ray.direction);
        const f32 b = 2.0f * ray.direction.dot(oc);
        const f32 c = oc.dot(oc) - radius*radius;

        const f32 discriminant = b*b - 4*a*c;

        if(discriminant > 0) {
            const f32 d0 = (-b + sqrt(discriminant)) / (2.0f * a);
            const f32 d1 = (-b - sqrt(discriminant)) / (2.0f * a);

            const f32 d = std::min(d0, d1); // Closest intersaction point

            const Vec3f collisionPoint = ray.origin + ray.direction*d;
            const Vec3f normal = (collisionPoint - pos).normalize();

            return RayCollisionData{
                true,
                collisionPoint,
                normal,
                color
            };
        }

        return RayCollisionData{false};
    }
};

struct PlaneObject : public Object {
    PlaneObject(const Vec3f _pos) : Object(_pos) {

    } 

    RayCollisionData hit(const Ray& ray) override {
        /*
        if(ray.y > pos.y) {
            return RayCollisionData{
                true,
                ray,
                ray,
                0xFF00FF00
            };
        }
        */

        return RayCollisionData{false};
    }
};

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Hello World", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    std::vector<Object*> objects;

    objects.push_back(new SphereObject(Vec3f(0.0f, 0.0f, 2.5f), 0xFF0000FF, 0.75f));
    objects.push_back(new SphereObject(Vec3f(1.0f, 0.0f, 2.5f), 0xFF00FF00, 0.45f));
    objects.push_back(new SphereObject(Vec3f(-1.0f, 0.0f, 1.1f), 0xFFFF0000, 0.32f));

    bool loopShouldStop = false;
    while (!loopShouldStop) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    loopShouldStop = true;
                    break;
            }
        }

        SDL_RenderClear(renderer);

        const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
        const SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

        void *pixels; int pitch;
        SDL_LockTexture(texture, &window_rect, &pixels, &pitch);

        const f32 fov = 1.0472f;
        const f32 aspect = static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT);

        Vec3f origin = Vec3f::splat(0.0f);

        for(int xW = 0; xW < WINDOW_WIDTH; ++xW) {
            for(int yH = 0; yH < WINDOW_HEIGHT; ++yH) {
                u32& pixel = static_cast<u32*>(pixels)[xW + yH * WINDOW_WIDTH];
                pixel = 0xFF000000;

                Vec3f uv(
                    (xW /static_cast<f32>(WINDOW_WIDTH) - 0.5f),
                    (yH /static_cast<f32>(WINDOW_HEIGHT) - 0.5f) / aspect,
                    1.0f
                );  

                Vec3f direction = (uv - origin).normalize();
                Ray ray{origin, direction};

                const u32 max_bounces = 1;

                for(u32 b = 0; b < max_bounces; ++b) {
                    for(auto& obj : objects) {
                        const auto collision = obj->hit(ray); 

                        if(collision.collision) {
                            pixel = collision.color;
                            ray.direction = collision.direction_after;
                        } else {
                            continue;
                        }
                    }
                }

            }
        }

        SDL_UnlockTexture(texture);
        SDL_RenderTexture(renderer, texture, &window_rect_f, &window_rect_f);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}