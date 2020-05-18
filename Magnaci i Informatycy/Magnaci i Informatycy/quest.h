#pragma once

#include <iostream>
#include <fstream>
#include <string>
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
#include "locations.h"


enum TYPE
{
	KILL,
	SPEAK_TO,
	GO_TO,
	TAKE
};


class Objective
{
protected:
	int target_id;
	std::string target_location_name;
	TYPE to_do;
	int how_many;
public:
	Objective(int target_id, std::string target_location_name, TYPE to_do, int how_many);
	bool check_objective(class Location* location);
	int get_target_id();
	int get_how_many();
	std::string get_target_location();
	bool is_it_done();
	TYPE get_to_do();
	int get_to_do_int();
	virtual void XD() = 0;
};

class Character_objective : public Objective
{
protected:
public:
	Character_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many);
	//void check_objective(Location* location);
	void XD() {};
};

class Element_objective : public Objective
{
public:
	Element_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many);
//	void check_objective(Location* location);
	void XD() {};
};

class Location_objective : public Objective
{
public:
	Location_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many);
//	void check_objective(Location* location);
	void XD() {};
};

class Item_objective : public Objective
{
protected:
public:
	Item_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many);
	//void check_objective(Location* location);
	void XD() {};
};


class Quest
{
public:
	std::vector<Objective*> objective;
	std::string name;
	Quest(std::string name, int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class);
	std::string get_name();
	void add_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class);
	~Quest();
};

class Quest_line
{
protected:
	std::string name;
	Quest* quest;
public:
	ALLEGRO_BITMAP* quest_bitmap;
	ALLEGRO_BITMAP* background;
	ALLEGRO_BITMAP* player;
	std::string next_quest_name;
	Quest_line(std::string quest_line_name, std::string start_quest_name);
	Quest_line(std::string quest_line_name);
	void quest_file_read(std::string quest_line_name, std::string quest_name);
	void add_quest(std::string name, int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class);
	bool take_next_quest();
	void save_status(std::fstream &file);
	std::string get_name();
	void load_status(std::fstream &file, std::string quest_name);
	Quest* get_quest();
	~Quest_line();
	void show_quests(ALLEGRO_FONT* setting_font, int &j);
};