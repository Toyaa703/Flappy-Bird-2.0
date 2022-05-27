#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

static SDL_Window* window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int FRAME_PER_SECOND = 30;
const int screen_w = 1200;
const int screen_h = 640;
const int screen_bpp = 32;

const int color_key_r = 167;
const int color_key_g = 175;
const int color_key_b = 180;


#define BLANK_TILE 0
#define Tile_Size 64
#define MAX_MAP_X 440
#define MAX_MAP_Y 10
#define BLANK_SPACE 20
#define HAND_SPACE 20
#define COIN_VAL 6
#define TIME 10
#define NUM_THREATS 80
#define TIME_TRANS 5000

typedef struct Map{
    int start_x, start_y, max_x, max_y;
    int tile[MAX_MAP_Y][MAX_MAP_X];
//    pair < pair < int , int >, int > pos[screen_w + 10][screen_h + 10];
    char* file_name;
};

typedef struct Input{
    int left, right, up, down, jump;
};

namespace SDLCommonFunc {
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

