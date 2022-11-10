//#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "err.h"
#include "string.h"

//load an image from a filename and return a surface
SDL_Surface *load_image(char *filename)
{
    SDL_Surface* loadedImage = IMG_Load(filename);//load the image
    if (loadedImage == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());//return an error if there is no surface

    //transform the image into a surface
    SDL_Surface* optimizedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_PIXELFORMAT_RGBA8888, 0);

    if (optimizedImage == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());//return an error if there is no surface

    SDL_FreeSurface(loadedImage);//free the firts surface which is the load image

    return optimizedImage;//return the created surface
}

//put a pixel to grayscle
Uint32 pixel_to_grayscale(Uint32 pixel, SDL_PixelFormat *format)
{
    Uint8 r, g, b; //creat the r, g, b variables
    SDL_GetRGB(pixel, format, &r, &g, &b);//get RGB values from the pixel
    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b; //create an average of the color
    r = g = b = average;
    Uint32 gray = SDL_MapRGB(format, r, g, b); //create a pixel whith the average
    return gray;
}

//for each pixel of the surface we use the previous function to put it on grayscale
void surface_to_grayscale(SDL_Surface *surface)
{
    Uint32 *pixels = surface->pixels;
    int length = surface->w * surface->h;
    SDL_PixelFormat *format = surface->format;
    SDL_LockSurface(surface);
    for (int i = 0; i < length; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);//change each pixel to gray
    SDL_UnlockSurface(surface);
}

//Adaptative thresholding. The threshold is calculated for each pixel based on the average of the surrounding 9 pixels.
void adaptative_threshold(SDL_Surface *surface) {
    Uint32* copy = malloc(surface->w * surface->h * sizeof(Uint32));//take place in the memory
    memcpy(copy, surface->pixels, surface->w * surface->h * sizeof(Uint32));
    Uint32 *pixels = surface->pixels;//create a pointer for the surface pixels
    SDL_PixelFormat *format = surface->format; //create a pointer for the surface format
    SDL_LockSurface(surface);//we lock the surface to work on it
    int range = 15;
    for (int i = 0; i < surface->h; i++) {
        for (int j = 0; j < surface->w; j++) {
            int sum = 0;
            for (int k = -range; k <= range; k++) {
                if (i + k < 0 || i + k >= surface->h)
                    continue;
                for (int l = -range; l <= range; l++) {
                    if (j + l < 0 || j + l >= surface->w)
                        continue;
                    Uint8 r, g, b;
                    SDL_GetRGB(copy[(i + k) * surface->w + (j + l)], format, &r, &g, &b);
                    sum += r;
                }
            }

            Uint8 average = sum / (2 * range + 1) / (2 * range + 1); //we create an average from the sum of the rgb values of pixels around the pixel
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i * surface->w + j], format, &r, &g, &b);
            if (r < average)//if values inferoir at the average
                pixels[i * surface->w + j] = SDL_MapRGB(format, 0, 0, 0);//put pixel in black
            else
                pixels[i * surface->w + j] = SDL_MapRGB(format, 255, 255, 255);//put pixel in white
        }
    }
    SDL_UnlockSurface(surface);//unlock the surface because we have finished to work on it
    free(copy);
}

//accentuates the shape
//for example, the shape of the numbers will be clearer
void dilatation(SDL_Surface* surface) {
    Uint32* pixels = surface->pixels;//create a pointer for the surface pixels
    for (int i = 0; i < surface->h; i++) {
        for (int j = 0; j < surface->w; j++) {
            if (pixels[i * surface->w + j] == SDL_MapRGB(surface->format, 0, 0, 0)) { //if piwel is black
                if (i - 1 >= 0)
                    pixels[(i - 1) * surface->w + j] = SDL_MapRGB(surface->format, 0, 0, 0);
                else if (i + 1 < surface->h)
                    pixels[(i + 1) * surface->w + j] = SDL_MapRGB(surface->format, 0, 0, 0);
                else if (j - 1 >= 0)
                    pixels[i * surface->w + (j - 1)] = SDL_MapRGB(surface->format, 0, 0, 0);
                else if (j + 1 < surface->w)
                    pixels[i * surface->w + (j + 1)] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }
}

//reverses the white and black
void invert(SDL_Surface* surface) {
    Uint32* pixels = surface->pixels;//create a pointer for the surface pixels
    for (int i = 0; i < surface->h; i++) {
        for (int j = 0; j < surface->w; j++) {
            if (pixels[i * surface->w + j] == SDL_MapRGB(surface->format, 0, 0, 0)) //if black
                pixels[i * surface->w + j] = SDL_MapRGB(surface->format, 255, 255, 255);//put pixel in white
            else
                pixels[i * surface->w + j] = SDL_MapRGB(surface->format, 0, 0, 0);//put pixel in black
        }
    }
}

//will look for the biggest form
//fill the form
int flood_fill(SDL_Surface* surface, size_t x, size_t y, Uint32 color) {
    int res = 0;
    Uint32* pixels = surface->pixels;//create a pointer for the surface pixels
    if (x < (size_t)surface->w && y < (size_t)surface->h){
        if (pixels[x + y * surface->w] == SDL_MapRGB(surface->format, 255, 255, 255)) { //if pixel is white
            res++;
            pixels[x + y * surface->w] = SDL_MapRGB(surface->format, 0, 255, 0);//put pixel in green
            res += flood_fill(surface, x - 1, y, color);
            res += flood_fill(surface, x + 1, y, color);
            res += flood_fill(surface, x, y - 1, color);
            res += flood_fill(surface, x, y + 1, color);
        }
    }
    return res;
}

//retrieve the grid
void get_grid(SDL_Surface* surface1, SDL_Surface* surface2, size_t x, size_t y, Uint32 color) {
    Uint32* pixels1 = surface1->pixels;//create a pointer for the surface1 pixels
    Uint32* pixels2 = surface2->pixels;//create a pointer for the surface2 pixels
    if (x < (size_t)surface1->w && y < (size_t)surface1->h){
        if (pixels1[x + y * surface1->w] == SDL_MapRGB(surface1->format, 255, 255, 255)) { //if pixel1 is white
            pixels1[x + y * surface1->w] = SDL_MapRGB(surface1->format, 0, 255, 0);//put pixels1 in green
            pixels2[x + y * surface2->w] = SDL_MapRGB(surface2->format, 0, 255, 0);//put pixels2 in green
            get_grid(surface1, surface2, x - 1, y, color);
            get_grid(surface1, surface2, x + 1, y, color);
            get_grid(surface1, surface2, x, y - 1, color);
            get_grid(surface1, surface2, x, y + 1, color);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: %s image", argv[0]);

    SDL_Surface *image = load_image(argv[1]);
    char *filename = argv[2];

    SDL_LockSurface(image);
    surface_to_grayscale(image);
    IMG_SavePNG(image,"/home/francois.halbout/afs/Junior-master/sudoku-solver/image/grayscale.png");
    adaptative_threshold(image);
    IMG_SavePNG(image, "/home/francois.halbout/afs/Junior-master/sudoku-solver/image/threshold.png");
    //dilatation(image);
    //IMG_SavePNG(image, "/home/maxime.cambou/afs/SPE/sudoku-solver/dilatation.png");
    invert(image);
    IMG_SavePNG(image, "/home/francois.halbout/afs/Junior-master/sudoku-solver/image/invert.png");
    SDL_Surface *copy = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
    size_t x = 0;
    size_t y = 0;
    int mini = 0;
    int tmp = 0;
    for (size_t i = 0; i < (size_t) copy->h; i++) {
        for (size_t j = 0; j < (size_t) copy->w; j++) {
            tmp = flood_fill(copy, j, i, SDL_MapRGB(copy->format, 0, 255, 0));
            if (tmp > mini) {
                mini = tmp;
                x = j;
                y = i;
            }
        }
    }
    //flood_fill(image, x, y, SDL_MapRGB(image->format, 0, 255, 0));
    SDL_Surface* copy_ = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
    for (size_t i = 0; i < (size_t)(copy_->h * copy->w); i++)
        ((Uint32*)copy_->pixels)[i] = SDL_MapRGB(copy_->format, 0, 0, 0);
    get_grid(image, copy_, x, y, SDL_MapRGB(copy_->format, 0, 255, 0));
    SDL_UnlockSurface(image);
    IMG_SavePNG(copy_, filename);
    SDL_FreeSurface(image);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(copy_);

    //hough(copy2);
    //rotate(copy2, 0.25);
    return EXIT_SUCCESS;
}
