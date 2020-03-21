#include <vector>
#include <iostream>
#include <fstream>
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

#include "items.h"

bool Armour::File_read(std::string& file_name) {

	//File variable
	std::fstream file;
	file.open(file_name);

	int seek_id;				//To compare IDs from list with needed
	int enums;					//To read code information to enums
	std::string temp;			//Temp string to skip unneeded item

	//Checks whenever file exists and was opened
	if (file)
	{
		//Loops until find needed item
		file >> seek_id;
		while (seek_id != id);
		{
			for (int i = 0; i < 11; i++)
				file >> temp;
			file >> seek_id;
		}

		//Decode first leters of ID into variables
		enums = seek_id / 100000;
		item = (ITEM)(enums - 1);
		enums = seek_id / 10000 % 10;
		armour_type = (ARMOUR_TYPE)(enums - 1);

		//False if doesn't have brackets
		file >> temp;
		if (temp != "{")
			return false;

		//Puts values into variables
		file >> temp;
		name = temp;

		//"-" means no description
		file >> temp;
		if (temp == "-") {
			description = "";
		}
		else {
			description = temp;
		}

		file >> temp;
		texture = al_load_bitmap(temp.c_str());
		file >> cost >> min_lvl >> hero_class >> armor_points >> speed >> magic_resistance;
	}
	else return false;
	return true;
}


Armour::Armour(int id, std::string& file_name) {

	this->id = id;
	min_lvl = hero_class = armor_points = speed = magic_resistance = 0;
	armour_type = (ARMOUR_TYPE)(0);

	File_read(file_name);
}

//------------------------------------------------------------

bool Weapon::File_read(std::string& file_name) {

	//File variable
	std::fstream file;
	file.open(file_name);

	int seek_id;				//To compare IDs from list with needed
	int enums;					//To read code information to enums
	std::string temp;			//Temp string to skip unneeded item

	//Checks whenever file exists and was opened
	if (file)
	{
		//Loops until find needed item
		file >> seek_id;
		while (seek_id != id);
		{
			for (int i = 0; i < 11; i++)
				file >> temp;
			file >> seek_id;
		}

		//Decode first leters of ID into variables
		enums = seek_id / 100000;
		item = (ITEM)(enums - 1);
		enums = seek_id / 10000 % 10;
		weapon_type = (WEAPON_TYPE)(enums - 1);

		//False if doesn't have brackets
		file >> temp;
		if (temp != "{")
			return false;

		//Puts values into variables
		file >> temp;
		name = temp;

		//"-" means no description
		file >> temp;
		if (temp == "-") {
			description = "";
		}
		else {
			description = temp;
		}

		file >> temp;
		texture = al_load_bitmap(temp.c_str());
		file >> cost >> min_lvl >> hero_class >> min_damage >> max_damage >> attack_speed;

		//False if doesn't have brackets
		file >> temp;
		if (temp != "}")
			return false;
	}
	else return false;
	return true;
}


Weapon::Weapon(int id, std::string& file_name) {

	this->id = id;
	min_lvl = hero_class = min_damage = max_damage = attack_speed = 0;
	weapon_type = (WEAPON_TYPE)(0);

	File_read(file_name);
}

//----------------------------------------------------

bool Food::File_read(std::string& file_name) {

	//File variable
	std::fstream file;
	file.open(file_name);

	int seek_id;				//To compare IDs from list with needed
	int enums;					//To read code information to enums
	std::string temp;			//Temp string to skip unneeded item

	//Checks whenever file exists and was opened
	if (file)
	{
		//Loops until find needed item
		file >> seek_id;
		while (seek_id != id);
		{
			for (int i = 0; i < 7; i++)
				file >> temp;
			file >> seek_id;
		}

		//Decode first leters of ID into variables
		enums = seek_id / 100000;
		item = (ITEM)(enums - 1);
		enums = seek_id / 10000 % 10;
		food_type = (FOOD_TYPE)(enums - 1);

		//False if doesn't have brackets
		file >> temp;
		if (temp != "{")
			return false;

		//Puts values into variables
		file >> temp;
		name = temp;

		//"-" means no description
		file >> temp;
		if (temp == "-") {
			description = "";
		}
		else {
			description = temp;
		}

		file >> temp;
		texture = al_load_bitmap(temp.c_str());
		file >> cost;

		//Determinates whenever item is food/potion or teleport
		if (enums < 4) {
			file >> health;
		}
		else {
			file >> tp_dest;
		}

		//False if doesn't have brackets
		file >> temp;
		if (temp != "}")
			return false;
	}
	else return false;
	return true;
}


Food::Food(int id, std::string& file_name) {

	this->id = id;
	health = 0;
	tp_dest = "";
	food_type = (FOOD_TYPE)(0);

	File_read(file_name);
}

//---------------------------------------------------

bool Special::File_read(std::string& file_name) {

	//File variable
	std::fstream file;
	file.open(file_name);

	int seek_id;				//To compare IDs from list with needed
	int enums;					//To read code information to enums
	std::string temp;			//Temp string to skip unneeded item

	//Checks whenever file exists and was opened
	if (file)
	{
		//Loops until find needed item
		file >> seek_id;
		while (seek_id != id);
		{
			for (int i = 0; i < 6; i++)
				file >> temp;
			file >> seek_id;
		}

		//Decode first leters of ID into variables
		enums = seek_id / 100000;
		item = (ITEM)(enums - 1);
		enums = seek_id / 10000 % 10;
		//special_type = (SPECIAL_TYPE)(enums - 1);

		//False if doesn't have brackets
		file >> temp;
		if (temp != "{")
			return false;

		//Puts values into variables
		file >> temp;
		name = temp;

		//"-" means no description
		file >> temp;
		if (temp == "-") {
			description = "";
		}
		else {
			description = temp;
		}

		file >> temp;
		texture = al_load_bitmap(temp.c_str());
		file >> cost;

		//False if doesn't have brackets
		file >> temp;
		if (temp != "}")
			return false;
	}
	else return false;
	return true;
}


Special::Special(int id, std::string& file_name) {

	this->id = id;

	File_read(file_name);
}