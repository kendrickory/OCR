#include "Imagery/Utils/op.h"

void init_sdl()
{

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path)
{
    SDL_Surface *img;


    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface *screen;


    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        errx(1, "Couldn't set %dx%d video mode: %s\n", img->w, img->h,
             SDL_GetError());
    }

    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN);

    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);
