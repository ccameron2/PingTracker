#include "AppIcon.h"
#include "Icon.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

AppIcon::AppIcon(SDL_Window& SDLWindow)
{
    LoadWindowIcon(SDLWindow);
}

AppIcon::~AppIcon()
{
    SDL_DestroySurface(mIconSurface);
}

void AppIcon::LoadWindowIcon(SDL_Window& SDLWindow)
{
    // this leaks
    const bin2cpp::File& resource = bin2cpp::getIconPngFile();
    //

    auto pixelCPPData = resource.getBuffer();
    auto pixelCPPDataUnsigned = (const stbi_uc*)pixelCPPData;

    int width, height, numComponents;

    unsigned char* pixelData = stbi_load_from_memory(pixelCPPDataUnsigned, resource.getSize(), &width, &height, &numComponents, 4);

    mIconSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    SDL_LockSurface(mIconSurface);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Uint32* pixel;
            int surfaceBytesPerPixel = mIconSurface->format->bytes_per_pixel;
            int surfaceRowSize = mIconSurface->pitch;
            int surfaceOffset = x * surfaceBytesPerPixel + y * surfaceRowSize;
            Uint8* pixels = (Uint8*)mIconSurface->pixels;
            SDL_PixelFormat* pixelFormat = mIconSurface->format;

            int pixelBufferOffset = 4 * (x + width * y);
            unsigned char red = pixelData[pixelBufferOffset];
            unsigned char green = pixelData[pixelBufferOffset + 1];
            unsigned char blue = pixelData[pixelBufferOffset + 2];
            unsigned char alpha = pixelData[pixelBufferOffset + 3];

            Uint32 colour = SDL_MapRGBA(pixelFormat, red, green, blue, alpha);

            pixel = (Uint32*)(pixels + surfaceOffset);
            *pixel = colour;
        }
    }

    SDL_UnlockSurface(mIconSurface);

    stbi_image_free(pixelData);

    // This wants a pointer not sure if && is a pointer or not check later if broken
    SDL_SetWindowIcon(&SDLWindow, mIconSurface);
}


