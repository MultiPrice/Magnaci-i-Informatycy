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

void Item::draw_in_inventory()
{
	al_draw_bitmap(texture, inventory_x, inventory_y, 0);
}

int Item::get_inventory_x()
{
	return inventory_x;
}

int Item::get_inventory_y()
{
	return inventory_y;
}

void Item::change_inventory_x(int new_x)
{
	inventory_x = new_x;
}

void Item::change_inventory_y(int new_y)
{
	inventory_y = new_y;
}

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
		item_type = (ITEM_TYPE)(enums);
		enums = seek_id / 10000 % 10;
		armour_type = (ARMOUR_TYPE)(enums);

		file >> tmp >> name;
		std::getline(file, description);
		std::getline(file, description);

		if (description == "-") //brak opisu
			description = "";
		file >> tmp;
		texture = al_load_bitmap(tmp.c_str());
		file >> cost >> min_lvl >> hero_class >> armor_points >> speed;
		inventory_x = inventory_y = 0;
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
		item_type = (ITEM_TYPE)(enums);
		enums = seek_id / 10000 % 10;
		weapon_type = (WEAPON_TYPE)(enums);

		file >> tmp >> name;
		std::getline(file, description);
		std::getline(file, description);

		if (description == "-") //brak opisu
			description = "";
		file >> tmp;
		texture = al_load_bitmap(tmp.c_str());
		file >> cost >> min_lvl >> hero_class >> min_damage >> max_damage >> attack_speed;
		inventory_x = inventory_y = 0;
	}
	else
		return false;
	file.close();
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
	for (int i = 0; i < 6; i++)
		equipment[i] = nullptr;
	for (int i = 0; i < 39; i++)
		inventory.push_back(nullptr);
	backpack = al_load_bitmap("bitmaps/items/inventory_backpack.png");
	background = al_load_bitmap("bitmaps/items/inventory_background.png");
	player = al_load_bitmap("player/player_move.png");
	grill = al_load_bitmap("bitmaps/items/inventory_backpack_grill.png");
}

Inventory::~Inventory()
{
	al_destroy_bitmap(backpack);
	al_destroy_bitmap(background);
	al_destroy_bitmap(player);
	al_destroy_bitmap(grill);
	for (int i = 0; i < inventory.size(); i++)
		delete(inventory[i]);
	for (int i = 0; i < 7; i++)
		delete(equipment[i]);
	inventory.clear();
}

void Inventory::show_inventory()
{
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_bitmap(backpack, screen_width / 2 + measure, 0, 0);
	al_draw_bitmap_region(grill, 0, 0, 782, 2 + (inventory.size() / 13) * measure, 1079, 299, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(player, 0, measure * 6, measure * 1.5, measure * 2, al_map_rgb(255, 255, 255), 0, 0, 150, 60, 8, 8, 0, 0);
	for (int i = 0; i <= 6; i++) // rysowanie przedmiotow zalozonych
	{
		if (equipment[i])
			equipment[i]->draw_in_inventory();
	}

	for (int i = 0; i < inventory.size(); i++) //rysowanie przedmiotow z eq
	{
		if (inventory[i])
			inventory[i]->draw_in_inventory();
	}
}

void Inventory::add_item_to_inventory(Item* new_item)
{
	int new_x = 1080, new_y = measure*5;
	for (int i = 0; i < inventory.size(); i++)
	{
		if (inventory[i])
		{
			new_x += measure;
			if (inventory[i]->get_inventory_x() >= (screen_width - measure*2))
			{
				new_x = 1080;
				new_y += measure;
			}
		}
		else
		{
			new_item->change_inventory_x(new_x);
			new_item->change_inventory_y(new_y);
			inventory[i] = new_item;
			break;
		}
	}
}

void Inventory::add_item_to_inventory_x_y(Item* new_item)
{
	int x = new_item->get_inventory_x();
	int y = new_item->get_inventory_y();
	int tmp_x = 1080, tmp_y = measure * 5;
	for (int i = 0; i < inventory.size(); i++)
	{
		if (tmp_x != x || tmp_y != y)
		{
			tmp_x += measure;
			if (tmp_x >= screen_width - measure)
			{
				tmp_x = 1080;
				tmp_y += measure;
			}
		}
		else
		{
			inventory[i] = new_item;
			return;
		}	
	}
}

Item* Inventory::I_want_take_this_item(int sought_x, int sought_y)
{
	int licznik = is_there_an_item(sought_x, sought_y);
	if(licznik<0)
		return nullptr;
	else
	{
		Item* tmp = inventory[licznik];
		inventory[licznik] = nullptr;
		return tmp;
	}
}

Item* Inventory::I_want_swap_this_item(int sought_x, int sought_y, Item* holding_item)
{
	int licznik = is_there_an_item(sought_x, sought_y);
	if (licznik < -1)
	{
		add_item_to_inventory(holding_item);
		return nullptr;
	}
	else if (licznik < 0)
	{
		holding_item->change_inventory_x(sought_x - (sought_x % measure));
		holding_item->change_inventory_y(sought_y - (sought_y % measure));
		add_item_to_inventory_x_y(holding_item);
		return nullptr;
	}
	else
	{
		Item* tmp_item = inventory[licznik];
		inventory[licznik] = holding_item;
		return tmp_item;
	}
}

Item* Inventory::I_want_take_this_equipment(int sought_x, int sought_y)
{
	int licznik = is_there_an_equipment(sought_x, sought_y);
	if (licznik < 0)
		return nullptr;
	else
	{
		Item* tmp = equipment[licznik];
		equipment[licznik] = nullptr;
		return tmp;
	}
}

Item* Inventory::I_want_equip_this_item(int sought_x, int sought_y, Item* holding_item)
{
	int licznik = is_there_an_equipment(sought_x, sought_y);
	std::cout << licznik;
	if (licznik < 0)
		return holding_item;
	else
	{
		holding_item->change_inventory_x(sought_x - (sought_x % measure));
		holding_item->change_inventory_y(sought_y - (sought_y % measure));
		Item* tmp_item = equipment[licznik];
		equipment[licznik] = holding_item;
		return tmp_item;
	}
}

int Inventory::is_there_an_equipment(int sought_x, int sought_y)
{
	int x = sought_x - (sought_x % measure);
	int y = sought_y - (sought_y % measure);
	int tmp_x = 1080, tmp_y = measure * 2;
	if (tmp_y == y)
	{
		for (int i = 0; i <= 6; i++)
		{
			if (tmp_x != x)
				tmp_x += measure * 2;
			else
				return i;
		}
	}
	return -1;
}

int Inventory::is_there_an_item(int sought_x, int sought_y)
{
	int tmp_x = 1080, tmp_y = measure * 5;
	if (sought_x >= tmp_x && sought_x <= screen_width - measure && sought_y >= tmp_y && sought_y <= (tmp_y + measure * inventory.size()/13))
	{
		for (int i = 0; i < inventory.size(); i++)
		{
			if (inventory[i])
			{
				tmp_x = inventory[i]->get_inventory_x();
				tmp_y = inventory[i]->get_inventory_y();
				if (sought_x >= tmp_x && sought_x <= tmp_x + measure && sought_y >= tmp_y && sought_y <= tmp_y + measure)
					return i;
			}
		}
		return -1;
	}
	else
		return -2;
}
