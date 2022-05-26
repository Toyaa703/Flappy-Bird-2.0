#pragma once
#include "CommonFunc.h"
#include "BaseObject.h"
#define THREATS_FRAME_NUM 1
#define THREATS_SPEED 5

class ThreatsObject : public BaseObject {
public:
    ThreatsObject();
    ~ThreatsObject();

    void set_x_val(const float& xVal) {x_val_ = xVal;}
    void set_y_val(const float& yVal) {y_val_ = yVal;}
    void set_x_pos(const float& xp) {x_pos_ = xp;}
    void set_y_pos(const float& yp) {y_pos_ = yp;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    void SetMapXY(const int& mp_x, const int& mp_y) {
        map_x_ = mp_x;
        map_y_ = mp_y;
    }
    SDL_Rect GetRect() {
        SDL_Rect rect;
        rect.x = x_pos_ - map_x_;
        rect.y = y_pos_ - map_y_;
        rect.w = width_frame_;
        rect.h = height_frame_;
        return rect;
    }
    void Set_clips();
    bool LoadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    void DoPlayer(Map& gMap);
    bool CheckMat(int idx) {
        if(idx == 1 || idx == 2 || idx == 3 || idx == 4) return true;
        return false;
    }
    void CheckToMap(Map& gMap);

private:
    int frame_;
    int width_frame_;
    int height_frame_;
    SDL_Rect frame_clip_[THREATS_FRAME_NUM];
    bool on_ground_;
    int come_back_time_;
    float x_pos_, y_pos_;
    float x_val_, y_val_;
    int map_x_, map_y_;
    Input input_type;
};
