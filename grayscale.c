#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL,NULL);
    SDL_RenderPresent(renderer);

}

void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = colored;

    draw(renderer, t);

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, colored);
                }
                break;
            case SDL_KEYDOWN:
                draw(renderer, t);
                if(t == colored){
                    t = grayscale;
                }
                else
                    t = colored;
                break;

        }
    }
}


SDL_Surface* load_image(const char* path)
{
    SDL_Surface *li = IMG_Load(path);
    if (li == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surf = SDL_ConvertSurfaceFormat(li, SDL_PIXELFORMAT_RGB888, 0);
        if (surf == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_FreeSurface(li);

    return surf;
}


Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    for(int i = 0;i < len;i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Image", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
         errx(EXIT_FAILURE, "%s", SDL_GetError());

    const char *i = argv[1];
    SDL_Surface *s =  load_image(i);

    SDL_SetWindowSize(window, s->w,s->h);

    SDL_Texture * texture1 = SDL_CreateTextureFromSurface(renderer, s);
    if (texture1 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    surface_to_grayscale(s);

    SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, s);
    if (texture2 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    SDL_FreeSurface(s);

    event_loop(renderer, texture1, texture2);

    SDL_DestroyTexture(texture2);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
