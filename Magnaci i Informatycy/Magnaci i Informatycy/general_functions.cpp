#include <vector>
#include <iostream>
#include <allegro5/bitmap.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "general_functions.h"

bool al_addon() //funkcja wywyo³uje wewnêtrzne funkcje biblioteki allegro
{
    if (!al_init())
    {
        std::cout << "Cloud not initialize Allegro 5";
        return 0;
    }

    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_install_audio();
    al_install_keyboard();
    al_install_mouse();
    //al_reserve_samples(x); x zalezny od liczby soundtrackow
    return 1;
}

void intro() // funkcja wyœwietla intro przed wejœciem do menu
{

}

int Object::get_X()
{
    return positionX;
}

int Object::get_Y()
{
    return positionY;
}

std::string Object::get_name()
{
	return name;
}

void Object::change_position(int new_X, int new_Y)
{
    positionX = new_X;
    positionY = new_Y;
}

void Object::change_texture(std::string new_texture_file)
{
    texture = nullptr;
    texture = al_load_bitmap(new_texture_file.c_str());
}

ALLEGRO_BITMAP* Object::get_texture()
{
    return texture;
}
