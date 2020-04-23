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

bool Armour::File_read(std::string file_name) 
{
	std::fstream file;
	file.open(file_name);
	if (file)
	{
		int seek_id;
		int enums;
		std::string tmp;

		file >> seek_id; //szukamy wskazanego id
		while (seek_id != id)
		{
			for (int i = 0; i < 10; i++)
				file >> tmp;
			file >> seek_id;
		}
		enums = seek_id / 100000; //na podstawie id z pliku ustalany jest rodzaj przedmiodu
		item_type = (ITEM_TYPE)(enums - 1);
		enums = seek_id / 10000 % 10;
		armour_type = (ARMOUR_TYPE)(enums - 1);

		file >> tmp >> name;
		std::getline(file, description);
		std::getline(file, description);

		if (description == "-") //brak opisu
			description = "";
		file >> tmp;
		texture = al_load_bitmap(tmp.c_str());
		file >> cost >> min_lvl >> hero_class >> armor_points >> speed;
	}
	else
		return false;
	file.close();
	return true;
}

int Armour::get_armour()
{
	return armor_points;
}


Armour::Armour(int id, std::string file_name) {

	this->id = id;
	min_lvl = hero_class = armor_points = speed = 0;
	armour_type = (ARMOUR_TYPE)(0);

	if (!File_read(file_name))
		std::cout << "blad odczytu z pliku armour";
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
		item_type = (ITEM_TYPE)(enums - 1);
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

int Weapon::get_min_damage()
{
	return min_damage;
}

int Weapon::get_max_damage()
{
	return max_damage;
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
		item_type = (ITEM_TYPE)(enums - 1);

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
		item_type = (ITEM_TYPE)(enums - 1);
		enums = seek_id / 10000 % 10;

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

Inventory::Inventory()
{
	for (int i = 0; i < 7; i++)
		equipment[i] = nullptr;
	backpack = al_load_bitmap("bitmaps/items/inventory_backpack.png");
	background = al_load_bitmap("bitmaps/items/inventory_background.png");
	player = al_load_bitmap("player/player_move.png");
	item_in_backpack_x = item_in_backpack_y = 0;
}

Inventory::~Inventory()
{
	al_destroy_bitmap(backpack);
	al_destroy_bitmap(background);
}

void Inventory::show_inventory()
{
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_bitmap(backpack, screen_width/2+measure, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(player, 0, measure * 6, measure*1.5, measure*2, al_map_rgb(255, 255, 255), 0, 0, 150, 60, 8, 8, 0, 0);
	item_in_backpack_x = 1080;
	item_in_backpack_y = measure * 2;
	for (int i = 0; i < 6; i++)
	{
		if(equipment[i])
			equipment[i]->draw_in_inventory(item_in_backpack_x, item_in_backpack_y);
		item_in_backpack_x += measure * 2;
	}
	item_in_backpack_x = 1080;
	item_in_backpack_y = measure * 5;
	for (int i = 0; i < inventory.size(); i++)
	{
		inventory[i]->draw_in_inventory(item_in_backpack_x, item_in_backpack_y);
		item_in_backpack_x += measure * 2;
		if (item_in_backpack_x > 1080 + 780)
		{
			item_in_backpack_x = 1080;
			item_in_backpack_y += measure * 2;
		}
	}
	al_flip_display();
	al_rest(3);
}

void Inventory::add_item_to_inventory(Item* new_item)
{
	inventory.push_back(new_item);
}

void Item::draw_in_inventory(int x, int y)
{
	al_draw_bitmap(texture, x, y, 0);
}
