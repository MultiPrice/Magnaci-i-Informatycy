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
	int speed;
	int countdown;
	Element* representation;
public:
	Action(std::string name, int damage, int cost, double cast_time, double duration, double cooldown, int speed, Object *& player);
	virtual bool make_action(Object*** &map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line) = 0;
	void prepare_action();
	int get_cast_time();
	Element* get_representation();
	~Action();
};

class AoE : public Action
{
protected:
	int center_x;
	int center_y;
public:
	AoE(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction, int effect_dash_x, int effect_dash_y);
	void try_make_effect(int i, Object***& map, Location*& location, std::vector <Quest_line*>& quest_line, int gap); // probuje wywolac efekt spella. Sprawdza warunki i wywoluje get_damage
	bool make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line);
	~AoE();
};

class Wind : public Action
{
protected:
	DIRECTION direction;
public:
	Wind(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction);
	int try_make_effect(Object***& map, Location*& location, std::vector <Quest_line*>& quest_line); // probuje wywolac efekt spella. Sprawdza warunki i wywoluje get_damage
	bool try_changing_position(Object***& map, Location*& location, std::vector <Quest_line*>& quest_line); // sprawdza czy mo¿liwe jest zmiana po³o¿ena i wywo³uje zmianê po³o¿enia reprezentacji
	bool make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location*& location, std::vector <Quest_line*>& quest_line);
	~Wind();
};

class Self : public Action
{
protected:
	bool over_time;
	bool check;
public:
	Self(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, bool over_time);
	bool make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line);
	~Self();
};

class Breath : public Action
{
protected:
	DIRECTION direction;
	int center_x;
	int center_y;
	bool check;
public:
	Breath(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction);
	void split(int positionX, int positionY);
	int try_make_effect(Object***& map, Location*& location, std::vector<Quest_line*>& quest_line);
	bool try_changing_position(Object***& map, Location*& location, std::vector <Quest_line*>& quest_line);
	void try_make_effect_after_split_up(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line);// sprawdza warunki zioniêcia po rozszczepieniu gdy byl on puszczony w gore
	void try_make_effect_after_split_down(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line);// sprawdza warunki zioniêcia po rozszczepieniu gdy byl on puszczony w dol
	void try_make_effect_after_split_right(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line);// sprawdza warunki zioniêcia po rozszczepieniu gdy byl on puszczony w prawo
	void try_make_effect_after_split_left(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line);// sprawdza warunki zioniêcia po rozszczepieniu gdy byl on puszczony w lewo
	bool make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location*& location, std::vector <Quest_line*>& quest_line);
	~Breath();
};

