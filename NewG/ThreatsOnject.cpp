#include "ThreatsOnject.h"

ThreatsObject::ThreatsObject() {
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time_ = 0;
    frame_ = 0;
    input_type.down = 1;
    input_type.up = 0;
}

ThreatsObject::~ThreatsObject() {

}

bool ThreatsObject::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret) {
        width_frame_ = rect.w / THREATS_FRAME_NUM;
        height_frame_ = rect.h;
    }
    return ret;
}

void ThreatsObject::Set_clips() {
    if(width_frame_ > 0 && height_frame_ > 0) {
        for(int i = 0; i < THREATS_FRAME_NUM; i++) {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}


void ThreatsObject::Show(SDL_Renderer* des) {
    if(come_back_time_ == 0) {
        rect.x = x_pos_ - map_x_;
        rect.y = y_pos_ - map_y_;
        frame_++;
        if(frame_ >= THREATS_FRAME_NUM) frame_ = 0;
    }


    SDL_Rect* current_clip = &frame_clip_[frame_];
    SDL_Rect renderQuad = {rect.x, rect.y , width_frame_, height_frame_};
    SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}

void ThreatsObject::DoPlayer(Map& gMap) {
    if(input_type.down == 1) {
        x_val_ = 0;
        y_val_ = THREATS_SPEED;
        CheckToMap(gMap);
    }
    else
    if(input_type.up == 1) {
        x_val_ = 0;
        y_val_ = - THREATS_SPEED;
    }

    CheckToMap(gMap);
}

void ThreatsObject::CheckToMap(Map& map_data) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // check vertical

    int width_min = width_frame_ < Tile_Size ? width_frame_ : Tile_Size;
    x1 = x_pos_ / Tile_Size;
    x2 = (x_pos_ + width_min) / Tile_Size;

    y1 = (y_pos_ + y_val_) / Tile_Size;
    y2 = (y_pos_ + y_val_ + height_frame_ - 3) / Tile_Size;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if(y_val_ > 0) { // move down
            if(CheckMat(map_data.tile[y2][x1]) || CheckMat(map_data.tile[y2][x2])) {
                y_pos_ = y2 * Tile_Size;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                input_type.down = 0;
                input_type.up = 1;
            }
        }
        else
        if(y_val_ < 0) {
            if(CheckMat(map_data.tile[y1][x1]) || CheckMat(map_data.tile[y1][x2])) {
                y_pos_ = (y1 + 1) * Tile_Size;
                y_val_ = 0;
                input_type.down = 1;
                input_type.up = 0;
            }
        }
    }
    x_pos_ += x_val_;
    y_pos_ += y_val_;


    if(x_pos_ < 0) x_pos_ = 0;
    else
        if(x_pos_ + width_frame_ > map_data.max_x) x_pos_ = map_data.max_x - width_frame_ - 1;

    if(y_pos_ > map_data.max_y) {
        come_back_time_ = 20;
    }
}
