#include "CommonFunc.h"
#include "Baseobject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "TextObject.h"
#include "ThreatsOnject.h"
#include <fstream>

BaseObject bgr, menu, win, lose;

GameMap game_map;

MainObject p_player;

TextObject time_game, cur_score, num_coins, How_to_play[10], Best_Score, Rank;

TTF_Font* font_point = NULL;
TTF_Font* font_rule = NULL;
TTF_Font* font_bigpoint = NULL;
TTF_Font* a_LCDNova = NULL;

Mix_Chunk* chunk = NULL;
Mix_Music* music = NULL;

vector < int > v;
vector < ThreatsObject > Threats;
ThreatsObject diamond;

bool InitData() {
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0) return false;
    window = SDL_CreateWindow("Hello World!",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              screen_w, screen_h, screen_bpp);
    if(window == NULL) return false;
    g_screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(g_screen == NULL) return false;

    if(TTF_Init() == -1) {
        cout << "Init TTF fail" << '\n';
        return false;
    }
    font_point = TTF_OpenFont("Font/dlxfont_.ttf", 15);
    font_rule = TTF_OpenFont("Font/timesbd.ttf", 30);
    font_bigpoint = TTF_OpenFont("Font/dlxfont_.ttf", 40);
    a_LCDNova = TTF_OpenFont("Font/a_LCDNova.ttf", 30);
    if(font_point == NULL) return false;
    if(font_rule == NULL) return false;
    if(font_bigpoint == NULL) return false;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        cout << "Init Mix fail";
        return false;
    }

    return true;
}

bool LoadData() {
    bool ret = bgr.LoadImg("Img/background3.bmp", g_screen);
    if(ret == false) return false;
    return true;
}

void close() {
    bgr.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

void Show_Score(string str_score, int x_pos, int y_pos) {
    str_score += to_string(p_player.Get_Score());
    cur_score.SetText(str_score);
    cur_score.LoadFromRenderText(font_bigpoint, g_screen);
    cur_score.RenderText(g_screen, x_pos, y_pos);
}

void Show_LastScore(string str_score, int x_pos, int y_pos) {
    str_score += to_string(p_player.Get_Score() + p_player.get_num_coins());
    cur_score.SetText(str_score);
    cur_score.LoadFromRenderText(a_LCDNova, g_screen);
    cur_score.RenderText(g_screen, x_pos, y_pos);
}

void Show_coins(string str_coins, int x_pos, int y_pos) {
    str_coins += to_string(p_player.get_num_coins());
    num_coins.SetText(str_coins);
    num_coins.LoadFromRenderText(font_point, g_screen);
    num_coins.RenderText(g_screen, x_pos, y_pos);
}

int to_int(string data) {
    int val = 0;
    for(auto i : data)
        if(i >= '0' && i <= '9')
            val = (val * 10 + int(i) - 48);
    return val;
}

int Get_Best_Score() {
    ifstream f;
    f.open("File/Score.dat");
    string data;
    int val = 0;
    int fval;
    while(f >> data) {
        fval = to_int(data);
        v.push_back(fval);
        val = max(val, fval);
    }
    f.close();
    return val;
}

void Show_Rank(int x_pos, int y_pos) {
    int val = 0;
    int score = p_player.Get_Score();
    v.push_back(score);
    for(int i = 0; i < v.size(); i++) v[i] *= -1;
    sort(v.begin(), v.end());
    for(int i = 0; i < v.size(); i++) v[i] *= -1;
    for(auto i : v) {
        val++;
        if(i == score) break;
    }
    // ghi file
    fstream f;
    f.open("File/Score.dat", ios::out);
    for(auto i : v) f << to_string(i) << '\n';
    f.close();

    string data = "Your Rank: " + to_string(val) + "th";
    Rank.SetColor(TextObject::RED_TEXT);
    Rank.SetText(data);
    Rank.LoadFromRenderText(a_LCDNova, g_screen);
    Rank.RenderText(g_screen, x_pos, y_pos);
}

void EndGame() {
    bool ret = lose.LoadImg("Img/Game_Over.bmp", g_screen);
    SDL_RenderClear(g_screen);
    lose.Render(g_screen, NULL);

    Show_LastScore("Your score: ", screen_w - 1050, 375);
    Show_Rank(screen_w - 1050, 435);

    SDL_RenderPresent(g_screen);
}

void Loadmenu() {
    bool ret = menu.LoadImg("Img/MENU.bmp", g_screen);
    SDL_RenderClear(g_screen);
    menu.Render(g_screen, NULL);

//    for(int i = 1; i <= 4; i++) How_to_play[i].SetColor(TextObject::WHITE_TEXT);
    How_to_play[4].SetColor(TextObject::PURPLE_TEXT);
    Best_Score.SetColor(TextObject::RED_TEXT);

//    How_to_play[1].SetText("Press w to jump");
//    How_to_play[2].SetText("Touch the spider, you lose 2 points");
//    How_to_play[3].SetText("With 5 coins, become super bird in 5s");
    How_to_play[4].SetText("Press space to start ! Good luck !");

//    for(int i = 1; i <= 4; i++) How_to_play[i].LoadFromRenderText(a_LCDNova, g_screen);
    How_to_play[4].LoadFromRenderText(a_LCDNova, g_screen);
//    How_to_play[1].RenderText(g_screen, 45, 200);
//    How_to_play[2].RenderText(g_screen, 45, 250);
//    How_to_play[3].RenderText(g_screen, 45, 300);
    How_to_play[4].RenderText(g_screen, 45, 200);

    string str_best_score = "Best Record: ";
    str_best_score += to_string(Get_Best_Score());
    Best_Score.SetText(str_best_score);
    Best_Score.LoadFromRenderText(a_LCDNova, g_screen);
    Best_Score.RenderText(g_screen, 45, 250);

    SDL_RenderPresent(g_screen);


    SDL_Event events;
    bool is_quit = false;
    while(is_quit == false) {
        while(SDL_PollEvent(&events) != 0)
            if(events.type == SDL_KEYDOWN)
                if(events.key.keysym.sym == SDLK_SPACE)
                    is_quit = true;
    }
}

void Win_Game() {
    bool ret = win.LoadImg("Img/Win_Game.bmp", g_screen);
    SDL_RenderClear(g_screen);
    win.Render(g_screen, NULL);
    Show_LastScore("Your score: ", screen_w - 850, 400);
    Show_Rank(screen_w - 850, 450);
    SDL_RenderPresent(g_screen);
}

void Show_time_trans(int x_pos, int y_pos) {
    string str_time = "Time:";
    int val = (TIME_TRANS - p_player.get_time_trans()) / 1000;
    str_time  += to_string(val);
    time_game.SetText(str_time);
    time_game.LoadFromRenderText(font_point, g_screen);
    time_game.RenderText(g_screen, screen_w - 200, 45);
}

void set_threats() {
    fstream f;
    f.open("File/Threats.dat", ios::in);
    string data;

    ThreatsObject spi;
    for(int i = 0; i < NUM_THREATS; i++) Threats.push_back(spi);
    int cnt = 0;
    while(f >> data) {
        Threats[cnt].LoadImg("Img/spider.bmp", g_screen);
        Threats[cnt].Set_clips();
        Threats[cnt].set_x_pos(to_int(data));
        Threats[cnt].set_y_pos(64 * 3 + 1);
        cnt++;
    }

    f.close();
}

void show_diamond() {
    diamond.LoadImg("Img/diamond.bmp", g_screen);
    diamond.Set_clips();
    diamond.set_x_pos(Tile_Size * MAX_MAP_X - Tile_Size * 10);
    diamond.set_y_pos(Tile_Size);
}

void return_normal() {
    p_player.set_time_trans(0);
    p_player.set_super(false);
}

bool LoadWav(string path) {
    chunk = Mix_LoadWAV(path.c_str());
    return chunk != NULL;
}

bool LoadMusic(string path) {
    music = Mix_LoadMUS(path.c_str());
    return music != NULL;
}

int main (int argc, char* argv[])
{
    ImpTimer fps_timer;

    if(InitData() == false) {
        cout << "Init fail";
        return -1;
    }

    if(!LoadMusic("Sound/Trap.wav")) {
        cout << "LoadMusic fail";
    }

    if(!LoadWav("Sound/ting.wav")) {
        cout << "LoadWav fail";
    }

    // play music bgr
    if(!Mix_PlayingMusic()) {
            Mix_PlayMusic(music, -1);
    }

    //Load menu
    Loadmenu();

    if(LoadData() == false) {
        cout << "load bgr fail";
        return -1;
    }
    game_map.LoadMap("File/MAP.dat");
    game_map.LoadTiles(g_screen);

    p_player.LoadImg("Img/fl_bird_jump.bmp", g_screen);
    p_player.set_clips();

    // Set threat object
//    set_threats();
    ThreatsObject spi;

    // diamond
    show_diamond();
    bool diamond_stt = true;

    //time text
    time_game.SetColor(TextObject::WHITE_TEXT);
    cur_score.SetColor(TextObject::RED_TEXT);

//    Checkcollision
    bool valc, cur_stt;
    cur_stt = false;
    valc = false;

// Time transform
    int time_trans = 0;
    bool is_quit = false;
    string str_time, str_score, str_val;
    int val;

    bool game_over = false;
    bool game_win = false;

    while(is_quit == false) {

        fps_timer.start();

        // return normal
        if(p_player.get_time_trans() >= TIME_TRANS) return_normal();

        while(SDL_PollEvent(&g_event) != 0) {
            if(g_event.type == SDL_QUIT) is_quit = true;
            p_player.HandleInputAction(g_event, g_screen);
        }

        if(game_over) continue;
        if(game_win) continue;
        SDL_RenderClear(g_screen);
        bgr.Render(g_screen, NULL);


        Map map_data = game_map.getMap();


        p_player.SetMapXY(map_data.start_x, map_data.start_y);
        p_player.DoPlayer(map_data);

        if(p_player.get_touch_coin()) {
            Mix_PlayChannel(-1, chunk, 0);
            p_player.set_touch_coin(false);
        }

        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        //show threat object
        for(int i = 0; i < Threats.size(); i++) {
            Threats[i].SetMapXY(map_data.start_x, map_data.start_y);
            Threats[i].DoPlayer(map_data);
            Threats[i].Show(g_screen);
        }

        // show diamond
        if(diamond_stt) {
            diamond.SetMapXY(map_data.start_x, map_data.start_y);
            diamond.Show(g_screen);
        }

        // check collision
        SDL_Rect object1, object2;
        object2 = p_player.GetRect();
        object2.w /= NUM_FRAME;
//        cout << object2.w << '\n';
        for(int i = Threats.size() - 1; i >= 0; i--) {
            spi = Threats[i];
            object1 = spi.GetRect();
            valc = SDLCommonFunc::CheckCollision(object1, object2);
            if(valc) {
                cur_stt = p_player.get_super();
                if(cur_stt) p_player.Set_Score(p_player.Get_Score() + 5);
                else p_player.Set_Score(p_player.Get_Score() - 2);
                swap(Threats[i], Threats.back());
                Threats.pop_back();

            }
        }

        if(diamond_stt) {
            object1 = diamond.GetRect();
            if(SDLCommonFunc::CheckCollision(object1, object2)) {
                diamond_stt = false;
                p_player.Set_Score(p_player.Get_Score() + 50);
            }
        }

        str_val = "";

        // show score
        Show_Score("", screen_w - 600, 35);

        // Show coins
        Show_coins("Coin: ",screen_w - 200, 15);

//      show trans time
        if(p_player.get_super()) Show_time_trans(screen_w - 200, 45);

        SDL_RenderPresent(g_screen);

        if(p_player.Get_GameWin() == true) {
            Win_Game();
            game_win= true;
            continue;
        }

        if(p_player.Get_GameOver() == true) {
            EndGame();
            game_over = true;
            continue;
        }

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

        if(real_imp_time < time_one_frame) {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0) SDL_Delay(delay_time);
        }

        // cal time trans
        if(!p_player.get_super()) continue;
        time_trans = max(time_one_frame, real_imp_time);
        p_player.set_time_trans(p_player.get_time_trans() + time_trans);
    }
    close();
    return 0;
}
