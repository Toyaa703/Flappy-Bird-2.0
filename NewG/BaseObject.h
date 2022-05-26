#pragma once

#include "CommonFunc.h"

class BaseObject
{
    public:
        BaseObject();
        ~BaseObject();
        void SetRect(const int& x, const int& y) {rect.x = x, rect.y = y;}
        SDL_Rect GetRect() const {return rect;}
        SDL_Texture* GetTexture() const {return p_object;}
        virtual bool LoadImg(string path, SDL_Renderer* screen);
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();

    protected:
        SDL_Texture* p_object;
        SDL_Rect rect;
};

