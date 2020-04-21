#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "buttons.h"
#include "general_functions.h"
#include "characters.h"
#include "locations.h"


struct butt_list
{
    Button* element = nullptr;
    butt_list* next = nullptr;
    butt_list* prev = nullptr;
};

butt_list* add(Button* el, butt_list* buttons);
butt_list* remove(Button* el, butt_list* buttons);              //usuwa przycisk
butt_list* remove_from_list(Button* el, butt_list* buttons);     //wyrzuca przycisk z listy ale nie usuwa przycisku

class window
{
    int mapping_tab[10] = { 0,0,0,0,0,0,0,0,0,0 };
    int screen_width, screen_height;
    bool game_is_on;
    bool mode;
    butt_list* buttons;
    butt_list* setting_buttons;
    ALLEGRO_BITMAP* backgroud;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_EVENT events;
    ALLEGRO_FONT* header_font;
    ALLEGRO_FONT* setting_font;
    ALLEGRO_KEYBOARD_STATE keyboard;


    Object* player;
    Object*** map;
    Location* location;
    bool test;
public:
    friend Button;
    window(int pwidth, int pheight);
    void registration();
    void working();
    bool game_working();
    void restart(std::string location_name);
    void map_clear();

    void display_mode();
    //funkcje ustawiaj¹ce wygl¹d okna
    void menu();
    void start();
    void settings();
    void audio();
    void graphic();
    void move();
    void clear();
    void setting_clear();

    //funkcje operuj¹ce przyciskami
    void draw_buttons();
    void add_functional_button(float position_x, float position_y, BUTTON_FUNCTION function);
    void add_mapping_button(float position_x, float position_y, int pbutton_number);
    void add_fader(float position_x, float position_y);
    // funkcje przemieszczania na mapie
    bool player_movement();
    void player_attack();
    void change_position(Object*& who, int prevX, int prevY, int nextX, int nextY);
    void add_fader(float position_x, float position_y, FADER_FUNCTION pfunction);
    void add_combo(int pposition_x, int pposition_y);
};

void which_x_in_animation(int& start_x);