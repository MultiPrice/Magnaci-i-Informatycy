#pragma once

#include <iostream>
#include <typeinfo>
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
#include "characters.h"

class Action
{
protected:
	std::string name;
	int damage;
	int cost;
	double cast_time;
	double duration;
	double cooldown;
	Element* representation;
public:
	Action(std::string name, int damage, int cost, double cast_time, double duration, double cooldown);
	virtual bool make_action(Object*** &map, int size_x, int size_y, int position_x, int position_y, std::vector <Object*>& mobs) = 0;
};

class AoE : public Action
{
protected:

public:
	void choose_location();
};

class Wind : public Action
{
protected:
	DIRECTION direction;
public:
	Wind(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, DIRECTION direction, bool piercing);
	bool make_action(Object*** &map, int size_x, int size_y, int position_x, int position_y, std::vector <Object*>& mobs);
};

class Self : public Action
{
public:

};

