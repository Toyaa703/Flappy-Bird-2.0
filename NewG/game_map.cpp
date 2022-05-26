#include "game_map.h"
#include <fstream>

void GameMap::LoadMap(char* name) {
    fstream f;
    f.open(name, ios::in);
    string st;
    char ss;
    while(f >> ss)
        if(ss >= '0' && ss <= '9') st += ss;
    int cnt = -1;
    game_map_.max_x = 0;
    game_map_.max_y = 0;

    for(int i = 0; i < MAX_MAP_Y; i++)
        for(int j = 0; j < MAX_MAP_X; j++) game_map_.tile[i][j] = 0;

    for(int i = 0; i < MAX_MAP_Y; i++)
        for(int j = 0; j < MAX_MAP_X - BLANK_SPACE - HAND_SPACE; j++) {
            cnt++;
            game_map_.tile[i][j] = int(st[cnt]) - 48;
            int val = game_map_.tile[i][j];
            if(val > 0) {
                if(j > game_map_.max_x) game_map_.max_x = j;
                if(i > game_map_.max_y) game_map_.max_y = i;
            }
        }
    f.close();

    cout << MAX_MAP_Y - 1 << ' ' << MAX_MAP_X - BLANK_SPACE - HAND_SPACE - 1<< '\n';

    f.open("File/FinalMap.dat", ios::in);
    st = "";
    while(f >> ss)
        if(ss >= '0' && ss <= '9') st += ss;
    cnt = -1;
    for(int i = 0; i < MAX_MAP_Y; i++)
        for(int j = MAX_MAP_X - BLANK_SPACE - HAND_SPACE; j < MAX_MAP_X - BLANK_SPACE; j++) {
            cnt++;
            game_map_.tile[i][j] = int(st[cnt]) - 48;
            int val = game_map_.tile[i][j];
            if(val > 0) {
                if(j > game_map_.max_x) game_map_.max_x = j;
                if(i > game_map_.max_y) game_map_.max_y = i;
            }
        }
    f.close();

    game_map_.max_x = (MAX_MAP_X - 1) * Tile_Size;
    game_map_.max_y = MAX_MAP_Y * Tile_Size;

    game_map_.start_x = 0;
    game_map_.start_y = 0;
    game_map_.file_name = name;
}

void GameMap::LoadTiles(SDL_Renderer* screen2) {
    char file_img[30];
    FILE* fp = NULL;
    for(int i = 0 ; i < MAX_TILES; i++) {
        sprintf_s(file_img, "img/%d.bmp", i);
        tile_mat[i].LoadImg(file_img, screen2);
    }
}

void GameMap::DrawMap(SDL_Renderer* screen2) {
    int x1, x2, y1, y2;
    x1 = x2 = y1 = y2 = 0;

    int map_x = game_map_.start_x / Tile_Size;
    int map_y = game_map_.start_y / Tile_Size;

    x1 = (game_map_.start_x % Tile_Size) * -1;
    x2 = x1 + screen_w + (x1 == 0 ? 0 : Tile_Size);

    y1 = (game_map_.start_y % Tile_Size) * -1;
    y2 = y1 + screen_h + (y1 == 0 ? 0 : Tile_Size);

    for(int i = y1; i < y2; i += Tile_Size) {
        map_x = game_map_.start_x / Tile_Size;
        for(int j = x1; j < x2; j += Tile_Size) {
            int val = game_map_.tile[map_y][map_x];
            if(val > 0) {
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen2);
            }
            map_x++;
        }
        map_y++;
    }
}
