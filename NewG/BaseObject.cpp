#include "Baseobject.h"
#include "CommonFunc.h"

BaseObject::BaseObject() {
    p_object = NULL;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0;
    rect.w = 0;
}

BaseObject::~BaseObject() {
    Free();
}

bool BaseObject::LoadImg(string path, SDL_Renderer* screen) {
    Free();
    SDL_Surface* load_surface = NULL;
    load_surface  = SDL_LoadBMP(path.c_str());
    if(load_surface != NULL) {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, color_key_r, color_key_g, color_key_b));
        p_object = SDL_CreateTextureFromSurface(screen, load_surface);
        if(p_object != NULL) {
            rect.w = load_surface->w;
            rect.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    return p_object != NULL;
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip) {
    SDL_Rect renderquad = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderCopy(des, p_object, clip, &renderquad);
//    SDL_RenderCopy(des, p_object, NULL, NULL);
}

void BaseObject::Free() {
    if(p_object != NULL) {
        SDL_DestroyTexture(p_object);
        p_object = NULL;
        rect.x = 0;
        rect.y = 0;
    }
}
