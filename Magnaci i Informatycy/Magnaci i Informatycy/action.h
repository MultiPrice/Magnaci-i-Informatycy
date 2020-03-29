#pragma once

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

class Action: public Object
{
protected:
	std::string name;
	int damage;
	int cost;
	double cast_time;
	double duration;
	double cooldown;
public:
	void draw();
	void draw(int position_x, int position_y);
	void make_action();
};