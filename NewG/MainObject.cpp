#include "MainObject.h"
#include "BaseObject.h"

MainObject::MainObject() {
    frame = 0;
    x_pos = 0;
    y_pos = 0;
    x_val = 0;
    y_val = 0;
    width_frame = 0;
    height_frame = 0;
    status = WALK_NONE;
    input_type.left = 0;
    input_type.right = 1;
    input_type.down = 0;
    input_type.up = 0;
    input_type.jump = 0;
    on_ground = false;
    map_x_= 0;
    map_y_ = 0;
    gameover_ = false;
    Score.Set_val(0.0);
    super_ = false;
    time_trans_ = 0;
    num_coins_ = 0;
    touch_coin_ = false;
    GameWin_ = false;
}

MainObject::~MainObject() {

}

bool MainObject::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true) {
        width_frame = rect.w / NUM_FRAME;
        height_frame = rect.h;
    }
    return ret;
}

void MainObject::set_clips() {
    if(width_frame > 0 && height_frame > 0) {
        for(int i = 0; i < NUM_FRAME; i++) {
            frame_clip[i].x = width_frame * i;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }
}

void MainObject::Show(SDL_Renderer* des) {
    UpdateImagePlayer(des);

    if(input_type.jump == 1) frame++;
    else frame = 0;

    if(frame >= NUM_FRAME) frame = 0;

    rect.x = x_pos - map_x_;
    rect.y = y_pos - map_y_;

    SDL_Rect* current_clip = &frame_clip[frame];

    SDL_Rect renderQuad = {rect.x, rect.y , width_frame, height_frame};

    SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen) {
    if(events.type == SDL_KEYDOWN) {
        switch(events.key.keysym.sym) {
            case SDLK_w: {
                input_type.jump = 1;
            }
            break;

            case SDLK_UP: {
                if(num_coins_ >= 5 && !super_) {
                    super_ = true;
                    num_coins_ -= 5;
                }
            }
            break;
        }
    }
    else
        if(events.type == SDL_KEYUP) {
            switch(events.key.keysym.sym) {
                case SDLK_w: {
                    input_type.jump = 0;
                }
                break;
            }
        }
}

void MainObject::DoPlayer(Map& map_data) {
//    if(come_back_time_ == 0) {
        x_val = 0;
        y_val += float(0.8);   // fall speed
        if(y_val >= MAX_FALL_SPEED) y_val = MAX_FALL_SPEED;
//        if(input_type.left == 1) x_val -= PLAYER_SPEED;
//        else
        if(input_type.right == 1) x_val += PLAYER_SPEED;

        if(input_type.jump == 1)
            y_val = - PLAYER_JUMP_VAL;

        CheckToMap(map_data);
        CenterEntityOnMap(map_data);
}

void MainObject::CenterEntityOnMap(Map& map_data) {
    map_data.start_x = x_pos - (MAP_RESET);
    if(map_data.start_x < 0) map_data.start_x = 0;
    else
        if(map_data.start_x + screen_w >= map_data.max_x)
            map_data.start_x = map_data.max_x - screen_w;

    map_data.start_y = y_pos - (screen_h / 2);
    if(map_data.start_y < 0) map_data.start_y = 0;
    else
        if(map_data.start_y + screen_h >= map_data.max_y)
            map_data.start_y = map_data.max_y - screen_h;
//    cout << map_data.start_x << ' ' << x_pos << ' ' << y_pos << '\n';
}

void MainObject::CheckToMap(Map& map_data) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // check horizontal
    int height_min = height_frame < Tile_Size ? height_frame : Tile_Size;

    x1 = (x_pos + x_val) / Tile_Size;
    x2 = (x_pos + x_val + width_frame - 1) / Tile_Size;

    y1 = (y_pos) / Tile_Size;
    y2 = (y_pos + height_min - 1) / Tile_Size;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y) {
        if(x_val > 0) // moving to right
        {
            if(map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE) {
                if(CheckMat(map_data.tile[y1][x2]) || CheckMat(map_data.tile[y2][x2])) {
                    Set_GameOver(gameover_);
                }
                else {
                    if(map_data.tile[y1][x2] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y1][x2] = 0;
                        touch_coin_ = true;
                    }
                    if(map_data.tile[y2][x2] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y2][x2] = 0;
                        touch_coin_ = true;
                    }
                }
            }
        }
//        else
//        if(x_val < 0) {
//            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE) {
//                if(CheckMat(map_data.tile[y1][x1]) || CheckMat(map_data.tile[y2][x1])) {
//                    Set_GameOver(gameover_);
//                }
//                else {
//                    if(map_data.tile[y1][x1] == COIN_VAL) {
//                        num_coins_++;
//                        map_data.tile[y1][x1] = 0;
//                        touch_coin_ = true;
//                    }
//                    if(map_data.tile[y2][x1] == COIN_VAL) {
//                        num_coins_++;
//                        map_data.tile[y2][x1] = 0;
//                        touch_coin_ = true;
//                    }
//                }
//            }
//        }
    }

    // check vertical

    int width_min = width_frame < Tile_Size ? width_frame : Tile_Size;
    x1 = x_pos / Tile_Size;
    x2 = (x_pos + width_min) / Tile_Size;

    y1 = (y_pos + y_val) / Tile_Size;
    y2 = (y_pos + y_val + height_frame - 3) / Tile_Size;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if(y_val > 0) {
            if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE) {
                if(CheckMat(map_data.tile[y2][x1]) || CheckMat(map_data.tile[y2][x2])) {
                    Set_GameOver(gameover_);
                }
                else {
                    if(map_data.tile[y2][x1] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y2][x1] = 0;
                        touch_coin_ = true;
                    }
                    if(map_data.tile[y2][x2] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y2][x2] = 0;
                        touch_coin_ = true;
                    }
                }
                if(status == WALK_NONE) status = WALK_RIGHT;
            }
        }
        else
        if(y_val < 0) {
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE) {
                if(CheckMat(map_data.tile[y1][x1]) || CheckMat(map_data.tile[y1][x2])) {
                    Set_GameOver(gameover_);
                }
                else {
                    if(map_data.tile[y1][x1] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y1][x1] = 0;
                        touch_coin_ = true;
                    }
                    if(map_data.tile[y1][x2] == COIN_VAL) {
                        num_coins_++;
                        map_data.tile[y1][x2] = 0;
                        touch_coin_ = true;
                    }
                }
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    Score.Add_val(x_val);

    if(x_pos > map_data.max_x) GameWin_ = true;

    if(y_pos > map_data.max_y) Set_GameOver(gameover_);

    if(y_pos < 0) Set_GameOver(gameover_);
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des) {
    if(super_) LoadImg("Img/super_fl_bird_jump.bmp", des);
    else LoadImg("Img/fl_bird_jump.bmp", des);
}

//bool MainObject::Get_GameWin() {
//    int cur_pos = int(x_pos) / Tile_Size;
//    int limit = MAX_MAP_X - BLANK_SPACE;
//    if(cur_pos >= limit) return true;
//    return false;
//}
