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
	INTERACT
};


class Objective
{
protected:
	std::string target_name;
	std::string target_location_name;
	TYPE to_do;
	int how_many;
	bool check;
public:
	Objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many);
	virtual void check_objective(Location* location) = 0;
};

class Character_objective : public Objective
{
protected:
public:
	Character_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many);
	void check_objective(Location* location);
};

class Element_objective : public Objective
{
public:
	Element_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many);
	void check_objective(Location* location);
};

class Location_objective : public Objective
{
public:
	Location_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many);
	void check_objective(Location* location);
};


class Quest
{
protected:
	std::string name;
public:
	std::vector <Objective*> objective;

	Quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class);
	std::string get_name();
	void add_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class);
};

class Quest_list
{
protected:
	std::string name;
	Quest* quest;
	std::string next_quest_name;
public:
	Quest_list(std::string quest_line_name, std::string start_quest_name);
	void quest_file_read(std::string quest_line_name, std::string quest_name);
	void add_quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class);
	void take_next_quest();
};