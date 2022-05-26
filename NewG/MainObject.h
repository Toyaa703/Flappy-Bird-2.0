#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"
#include "TextObject.h"

#define NUM_FRAME 8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 10
#define SHOW_POS 100
#define MAP_RESET 100


class MainObject : public BaseObject{
public:
    MainObject();
    ~MainObject();

    enum WalkType {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };
    bool LoadImg(string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip1 = NULL);
    void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void set_clips();
    void DoPlayer(Map& map_data);
    void CheckToMap(Map& map_data);
    bool CheckMat(int idx) {
        if(idx == 1 || idx == 2 || idx == 3 || idx == 4) return true;
        return false;
    }
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);

    void Set_GameOver(bool& gameover) {gameover = true;}
    bool Get_GameOver() const {return gameover_;}

    bool Get_GameWin() const {return GameWin_;}

    int Get_Score() {
        return int(Score.Get_val() / float(Tile_Size));
    }
     void Set_Score(int new_score) {
        Score.Set_val(new_score * float(Tile_Size));
    }

    bool get_super() const {return super_;}
    void set_super(bool super) {super_ = super;}
    int get_time_trans() const {return time_trans_;}
    void set_time_trans(int time_trans) {time_trans_ = time_trans;}
    void set_num_coins(int num_coins) {num_coins_ = num_coins;}
    int get_num_coins() const {return num_coins_;}
    bool get_touch_coin() const {return touch_coin_;}
    bool set_touch_coin(bool touch_coin) {touch_coin_ = touch_coin;}

private:
    TextObject Score;

    float x_val;
    float y_val;
    float x_pos;
    float y_pos;
    int width_frame;
    int height_frame;
    SDL_Rect frame_clip[8];
    Input input_type;
    int frame;
    int status;
    bool on_ground;

    int map_x_;
    int map_y_;

    int come_back_time_ = 0;
    bool gameover_;
    bool super_;
    int time_trans_;
    int num_coins_;
    bool touch_coin_;
    bool GameWin_;
};
