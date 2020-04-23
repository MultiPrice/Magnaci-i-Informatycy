#include <fstream>

#include "window.h"
#include "characters.h"

const int FPS = 60;

butt_list* add(Button* el, butt_list* buttons)
{
    if (!buttons)
    {
        butt_list* new_list = new butt_list();
        new_list->element = el;
        return new_list;
    }
    else
    {
        butt_list* new_list = new butt_list();
        new_list->element = el;
        new_list->next = buttons;
        buttons->prev = new_list;
        return new_list;
    }
}

butt_list* remove(Button* el, butt_list* buttons)
{
    butt_list* new_list = buttons, * ret = buttons;
    while (new_list)
    {
        if (new_list->element == el)
            break;
        else
            new_list = new_list->next;
    }
    butt_list* temp;
    if (new_list->prev)
    {
        temp = new_list->prev;
        temp->next = nullptr;
        if (new_list->next)
        {
            temp->next = new_list->next;
            temp = new_list->next;
            temp->prev = new_list->prev;
        }
        delete new_list->element;
        delete new_list;
        return ret;
    }
    else
    {
        if (!new_list->next)
        {
            delete new_list->element;
            delete new_list;
            return nullptr;
        }
        temp = new_list->next;
        temp->prev = nullptr;
        delete new_list->element;
        delete new_list;
        return temp;
    }
}

butt_list* remove_from_list(Button* el, butt_list* buttons)
{
    butt_list* new_list = buttons, * ret = buttons;
    while (new_list)
    {
        if (new_list->element == el)
            break;
        else
            new_list = new_list->next;
    }
    butt_list* temp;
    if (new_list->prev)
    {
        temp = new_list->prev;
        temp->next = nullptr;
        if (new_list->next)
        {
            temp->next = new_list->next;
            temp = new_list->next;
            temp->prev = new_list->prev;
        }
        return ret;
    }
    else
    {
        if (!new_list->next)
        {
            return nullptr;
        }
        temp = new_list->next;
        temp->prev = nullptr;
        return temp;
    }
}

void which_x_in_animation(int& start_x)
{
    start_x = 0;
}

window::window(int pwidth, int pheight)
    :screen_width(pwidth), screen_height(pheight)
{
    std::fstream file;
    file.open("mapping.txt", std::ios::in);
    for (int el : mapping_tab)
    {
        file >> el;
    }
    file.close();
    game_is_on = false;
    mode = true;
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(screen_width, screen_height);
    header_font = al_load_font("fonts/font.ttf", (150.0 / 1920) * al_get_display_width(display), 0);
    setting_font = al_load_font("fonts/font.ttf", (75.0 / 1920) * al_get_display_height(display), 0);
    event_queue = al_create_event_queue();
    backgroud = al_load_bitmap("bitmaps/background/sztandar.bmp");
    timer = al_create_timer(6.0 / FPS);
    move_timer = al_create_timer(1.0 / FPS);
    HUD = al_load_bitmap("bitmaps/background/hud.png");
    HP = al_load_bitmap("bitmaps/background/hp.png");
    MANA = al_load_bitmap("bitmaps/background/mana.png");
    player = nullptr;
    map = nullptr;
    registration();
    menu();
    working();
}

void window::registration()
{
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
}

void window::working()
{
    bool is_working = true;
    int mouse_position_x = events.mouse.x, mouse_position_y = events.mouse.y;
    while (is_working)
    {
        al_wait_for_event(event_queue, &events);
        if (game_is_on)
            game_is_on = game_working();
        if (mouse_position_x != events.mouse.x || mouse_position_y != events.mouse.y)
        {
            butt_list* temp = buttons;
            while (temp)
            {
                if (temp->element->check_position(events.mouse.x, events.mouse.y))
                {
                    temp->element->select();
                }
                else
                    temp->element->unselect();
                temp = temp->next;
            }

            al_set_target_bitmap(al_get_backbuffer(display));
            draw_buttons();
            al_flip_display();
            mouse_position_x = events.mouse.x;
            mouse_position_y = events.mouse.y;
        }
        else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            int i = 0;
            butt_list* temp = buttons;
            bool checking = true;
            while (checking)
            {
                if (temp->element->is_selected)
                {
                    switch (temp->element->click(mapping_tab))
                    {
                    case 0:
                        checking = false;
                        start();
                        game_is_on = true;
                        break;
                    case 1:
                        checking = false;
                        break;
                    case 2:
                        checking = false;
                        is_working = false;
                        break;
                    case 3:
                        checking = false;
                        break;
                    case 4:
                        checking = false;
                        settings();
                        break;
                    case 5:
                        checking = false;
                        break;
                    case 6:
                        checking = false;
                        game_is_on = false;
                        menu();
                        break;
                    case 7:
                        checking = false;
                        audio();
                        break;
                    case 8:
                        checking = false;
                        graphic();
                        break;
                    case 9:
                        checking = false;
                        move();
                        break;
                    case 10:
                        checking = false;
                        display_mode();
                        break;
                    case 11:
                        checking = false;
                        break;
                    }
                }
                if (temp = temp->next)
                    continue;
                else
                    checking = false;
            }
        }
    }
}

void window::display_mode()
{
    if (mode)
    {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        mode = false;
    }
    else
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
        mode = true;
    }
    al_flip_display();
}

void window::menu()
{
    clear();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(backgroud, 0, 0, 1920, 1080, 0, 0, screen_width, screen_height, 0);
    add_functional_button(220, 400, START);
    add_functional_button(220, 470, LOAD);
    add_functional_button(220, 540, SETTINGS);
    add_functional_button(220, 610, EXIT);
    draw_buttons();
    al_flip_display();
}

void window::settings()
{
    clear();
    add_functional_button(950, 1000, MENU);
    add_functional_button(650, 240, AUDIO);
    add_functional_button(850, 240, GRAPHIC);
    add_functional_button(1050, 240, MOVE);
    audio();
}

void window::audio()
{
    setting_clear();
    al_clear_to_color(al_map_rgb(150, 75, 0));
    al_draw_text(header_font, al_map_rgb(0, 0, 0), (650.0 / 1920) * screen_width, (50.0 / 1020) * screen_height, 0, "Ustawienia");
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (850.0 / 1920) * screen_width, (350.0 / 1020) * screen_height, 0, "Glosnosc");
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (700.0 / 1920) * screen_width, (400.0 / 1020) * screen_height, 0, "Ogolna");
    add_fader(1000, 435, MAIN);
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (700.0 / 1920) * screen_width, (470.0 / 1020) * screen_height, 0, "Muzyka");
    add_fader(1000, 510, SOUNDTRACK);
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (700.0 / 1920) * screen_width, (540.0 / 1020) * screen_height, 0, "Dialogi");
    add_fader(1000, 585, TALK);
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (700.0 / 1920) * screen_width, (610.0 / 1020) * screen_height, 0, "Efekty");
    add_fader(1000, 660, EFECTS);
    draw_buttons();
    al_flip_display();
}

void window::graphic()
{
    setting_clear();
    al_clear_to_color(al_map_rgb(150, 75, 0));
    al_draw_text(header_font, al_map_rgb(0, 0, 0), (650.0 / 1920) * al_get_display_width(display), (50.0 / 1020) * al_get_display_height(display), 0, "Ustawienia");
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (200.0 / 1920) * al_get_display_width(display), (400.0 / 1020) * al_get_display_height(display), 0, "Ustawienia grafiki");
    add_functional_button(800, 800, SCREEN_MODE);
    add_combo(500, 500);
    draw_buttons();
    al_flip_display();
}

void window::move()
{
    setting_clear();
    al_clear_to_color(al_map_rgb(150, 75, 0));
    al_draw_text(header_font, al_map_rgb(0, 0, 0), (650.0 / 1920) * al_get_display_width(display), (50.0 / 1020) * al_get_display_height(display), 0, "Ustawienia");
    al_draw_text(setting_font, al_map_rgb(0, 0, 0), (200.0 / 1920) * al_get_display_width(display), (400.0 / 1020) * al_get_display_height(display), 0, "Ustawienia sterowania");
    draw_buttons();
    al_flip_display();
}

void window::clear()
{
    while (buttons)
    {
        buttons = remove(buttons->element, buttons);
    }
    setting_buttons = nullptr;
}

void window::setting_clear()
{
    while (setting_buttons)
    {
        buttons = remove_from_list(setting_buttons->element, buttons);
        setting_buttons = remove(setting_buttons->element, setting_buttons);
    }
}

void window::draw_buttons()
{
    al_set_target_bitmap(al_get_backbuffer(display));
    butt_list* temp = buttons;
    while (temp)
    {
        if (temp->element->is_fader)
        {
            ((Fader*)temp->element)->fader_draw();
        }
        else
        {
            temp->element->draw();
        }
        temp = temp->next;
    }
}

void window::add_functional_button(float position_x, float position_y, BUTTON_FUNCTION function)
{
    Functional_Button* new_button = new Functional_Button((position_x / 1920) * al_get_display_width(display), (position_y / 1080) * al_get_display_height(display), (188.0 / 1920) * al_get_display_width(display), (43.0 / 1080) * al_get_display_height(display), function);
    buttons = add(new_button, buttons);
    if (function == SCREEN_MODE)
    {
        setting_buttons = add(new_button, setting_buttons);
    }
}

void window::add_mapping_button(float position_x, float position_y, int pbutton_number)
{
    Mapping_Button* new_button = new Mapping_Button((position_x / 1920) * al_get_display_width(display), (position_y / 1080) * al_get_display_height(display), (188.0 / 1920) * al_get_display_width(display), (43.0 / 1080) * al_get_display_height(display), pbutton_number);
    buttons = add(new_button, buttons);
    setting_buttons = add(new_button, setting_buttons);
}

void window::add_fader(float position_x, float position_y, FADER_FUNCTION pfunction)
{
    Fader* new_fader = new Fader((position_x / 1920) * al_get_display_width(display), (position_y / 1080) * al_get_display_height(display), (10.0 / 1920) * al_get_display_width(display), (29.0 / 1080) * al_get_display_height(display), (210.0 / 1920) * al_get_display_width(display), (29.0 / 1080) * al_get_display_height(display), pfunction, mapping_tab);
    buttons = add(new_fader, buttons);
    setting_buttons = add(new_fader, setting_buttons);
}

void window::add_combo(int pposition_x, int pposition_y)
{
    ComboBox* new_combo = new ComboBox((pposition_x / 1920.0) * al_get_display_width(display), (pposition_y / 1080.0) * al_get_display_height(display), (188.0 / 1920) * al_get_display_width(display), (43.0 / 1080) * al_get_display_height(display));
    buttons = add(new_combo, buttons);
    setting_buttons = add(new_combo, setting_buttons);
}