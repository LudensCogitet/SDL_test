#include "Texture.h"

Texture::Texture(SDL_Renderer * r)
{
    this->renderer = r;
    this->texture = NULL;
    this->width = 0;
    this->height = 0;
}

Texture::~Texture()
{
    this->free();
}

bool Texture::loadFromFile(std::string path) {
    free();

    SDL_Texture * tex = NULL;
    SDL_Surface * surface = IMG_Load(path.c_str());
    if(surface == NULL) {
        printf("Unable to load image '%s' Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    //SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface -> format, 0, 0xFF, 0xFF));

    tex = SDL_CreateTextureFromSurface(this->renderer, surface);
    if(tex == NULL) {
        printf("Unable to make texture from '%s' Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    this->width = surface->w;
    this->height = surface->h;

    SDL_FreeSurface(surface);
    this->texture = tex;

    return this->texture != NULL;
}

void Texture::free() {
    if(this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture    = NULL;
        this->width      = 0;
        this->height     = 0;
    }
}

void Texture::render(SDL_Rect & clip, int x, int y) {
    SDL_Rect dest = { x, y, clip.w, clip.h };

    SDL_RenderCopy(
        this->renderer,
        this->texture,
        &clip,
        &dest
    );
}

int Texture::getWidth() {
    return this->width;
}

int Texture::getHeight() {
    return this->height;
}
