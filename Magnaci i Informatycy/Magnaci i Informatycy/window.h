#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

#include "buttons.h"
#include "general_functions.h"
#include "characters.h"
#include "locations.h"
#include "action.h"
#include "quest.h"
#include "dialogue.h"
#include "User.h"


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
    ALLEGRO_BITMAP* HUD;
    ALLEGRO_BITMAP* HP;
    ALLEGRO_BITMAP* MANA;
    ALLEGRO_BITMAP* dialogue_window;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_EVENT events;
    ALLEGRO_FONT* header_font;
    ALLEGRO_FONT* setting_font;
    ALLEGRO_KEYBOARD_STATE keyboard;
    ALLEGRO_TIMER* movement_timer;
    ALLEGRO_TIMER* draw_timer;


    Object* player;
    Object*** map;
    Location* location;
    std::vector <Action*> action;
    std::vector <Quest_line*> quest_line;
    std::vector <Question*> dialogue;
public:
    friend Button;
    window(int pwidth, int pheight);
    void registration();
    void working();
    bool game_working();
    void check_location_quest(std::vector <Quest_line*> quest_line, int location_id);
    void clear_all();
    void restart(std::string location_name);
    int check_answer(int mouse_x, int mouse_y, Question* question);
    void show_dialogue(int character_id);
    void check_dialogue_quest(bool quest_check, std::vector<Quest_line*>& quest_line, int character_id); // sprawdza czy zostala wykonana rozowa potrzebna w quescie
    void draw_dialogue(Question* question);
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
    void add_fader(float position_x, float position_y, FADER_FUNCTION pfunction);
    void add_combo(int pposition_x, int pposition_y);

    // funkcje gry
    bool pause_game();
    void draw_hud(Character* player);
    bool player_movement();
    void player_attack();
    void change_position(Object*& who, int prevX, int prevY, int nextX, int nextY);
    void draw_actions(int position_x, int position_y);
    void inventory(Inventory* droped);
    void quests();
    bool dialogue_file_read(int character_id, std::string file_name);
    void check_quest_item(Inventory* inventory);
    void save_game();
    void save_player();
    void save_inventory();
    void save_quests();
    void load_quests();
    bool load_game();
    void add_login();
    bool check_login(std::string email_p, std::string haslo_p);

    // funkcje poruszania
    void attack_in_movement(Character*& tmp); // gracz wykonuje atak
    void action_in_movement(Character*& tmp, Action* new_action, int type); // gracz wyonal akcje
    void go_up(Character*& tmp); // ruch gracza w gore
    void go_down(Character*& tmp); // ruch gracza w dol
    void go_right(Character*& tmp); // ruch gracza w prawo
    void go_left(Character*& tmp); // ruch gracza w lewo
    void inventory_interaction(Character*& tmp); // interakcja z przedmiotem
    void portal_interaction(Character*& tmp); // interakcja z portalem
    void dialogue_interaction(Character*& tmp, Object*& obj); // interakcja z posatcia 
    void any_interaction(Character*& tmp); // wywolanie wszystkich funkcji interakcji
};

void which_x_in_animation(int& start_x);