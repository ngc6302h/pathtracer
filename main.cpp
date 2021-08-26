#include <SDL2/SDL.h>
#include <Vectors.h>
#include <Color.h>
#include <GeometricPrimitives.h>

using namespace std;

void pathtrace(SDL_Renderer* renderer, int window_width, int window_height);

int main()
{
    constexpr int SCREEN_WIDTH = 640;
    constexpr int SCREEN_HEIGHT = 480;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error initializing SDL2: %s\n", SDL_GetError());
        exit(-1);
    }
    
    SDL_Window* window = SDL_CreateWindow("Pathtracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        printf("Error creating window: %s\n", SDL_GetError());
        exit(-1);
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_RenderClear(renderer);
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    printf("Rect w: %d h: %d\n", w, h);
    
    double3 a(0., 1., 0.);
    printf("length: %f\n", a.normalized().y);
    if (a.length() != 1.)
        abort();
    
    pathtrace(renderer, w, h);
    SDL_RenderPresent(renderer);
    
    SDL_Delay(3000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
    
}

rgb sample_skybox(Ray const& ray)
{
    float t = 0.5 * (ray.direction().y + 1.0f);
    return t * rgb(1.f, 1.f, 1.f) + (1.0f-t) * rgb(0.5f, 0.7f, 1.0f);
}

void pathtrace(SDL_Renderer* renderer, int window_width, int window_height)
{
    double aspect_ratio = double(window_width) / window_height;
    
    double viewport_height = 2.;
    double viewport_width = viewport_height * aspect_ratio;
    
    double focal_length = 1.;
    
    auto origin = double3(0, 0 ,0);
    auto horizontal = double3(viewport_width, 0 ,0);
    auto vertical = double3(0, viewport_height ,0);
    auto lower_left_corner = origin - horizontal/2. - vertical/2. - double3(0, 0, focal_length);
    
    for(int j = window_height; j >= 0; j--)
    {
        for (int i = 0; i < window_width; i++)
        {
            double u = double(i) / (window_width-1);
            double v = double(j) / (window_height-1);
            Ray ray(origin, (lower_left_corner + u * horizontal + v * vertical - origin).normalized());
            rgb color = sample_skybox(ray);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
}