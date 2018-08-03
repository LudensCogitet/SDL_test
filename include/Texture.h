#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Texture
{
    public:
        Texture(SDL_Renderer * renderer);
        virtual ~Texture();

        bool loadFromFile(std::string path);
        void free();

        void render(SDL_Rect & clip, int x, int y);

        int getWidth();
        int getHeight();

    private:
        SDL_Renderer * renderer;
        SDL_Texture * texture;
        int width;
        int height;
};

#endif // TEXTURE_H
