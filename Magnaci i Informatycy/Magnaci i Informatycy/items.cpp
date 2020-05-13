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

Item::~Item()
{
	al_destroy_bitmap(texture);
}

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

int Item::get_item_id()
{
	return id;
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
		std::string tmp;

		file >> seek_id; //szukamy wskazanego id
		while (seek_id != id)
		{
			std::getline(file, tmp);
			for (int i = 0; i < 10; i++)
				std::getline(file, tmp);
			file >> seek_id;
		}

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


Armour::Armour(int id, std::string file_name)
{
	this->id = id;
	min_lvl = hero_class = armor_points = speed = 0;

	if (!File_read(file_name))
		std::cout << "blad odczytu z pliku armour";
}

//------------------------------------------------------------

bool Weapon::File_read(std::string file_name)
{
	std::fstream file;
	file.open(file_name);
	if (file)
	{
		int seek_id;
		std::string tmp;

		file >> seek_id; //szukamy wskazanego id
		while (seek_id != id)
		{
			std::getline(file, tmp);
			for (int i = 0; i < 11; i++)
				std::getline(file, tmp);
			file >> seek_id;
		}

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


Weapon::Weapon(int id, std::string file_name)
{
	this->id = id;
	min_lvl = hero_class = min_damage = max_damage = attack_speed = 0;

	if (!File_read(file_name))
		std::cout << "blad odczytu z pliku armour";
}

//----------------------------------------------------

bool Food::File_read(std::string& file_name) {

	//File variable
	std::fstream file;
	file.open(file_name);

	int seek_id;				//To compare IDs from list with needed
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
	for (int i = 0; i < 39; i++)
		inventory.push_back(nullptr);
	backpack = al_load_bitmap("bitmaps/items/inventory_backpack.png");
	background = al_load_bitmap("bitmaps/items/inventory_background.png");
	player = al_load_bitmap("player/player_move.png");
	grill = al_load_bitmap("bitmaps/items/inventory_backpack_grill.png");
}

Inventory::Inventory(int size)
{
	for (int i = 0; i < 7; i++)
		equipment[i] = nullptr;
	for (int i = 0; i < size; i++)
		inventory.push_back(nullptr);
	backpack = al_load_bitmap("bitmaps/items/drop_background.png");
	background = nullptr;
	player = nullptr;
	grill = al_load_bitmap("bitmaps/items/drop_grill.png");
}

Inventory::Inventory(std::string file_name)
{
	std::fstream file;
	file.open("save/save_inventory.txt");
	if (file)
	{
		int item_id;
		int size;
		int X = 1080;
		int Y = measure * 2;
		Item* new_weapon;
		Item* new_helmet;
		Item* new_chestplate;
		Item* new_boots;
		Item* new_amulet;
		Item* new_ring;

		for (int i = 0; i < 7; i++)
		{
			file >> item_id;
			switch (item_id / 100000)
			{
			case 0: //brak itema
				X += measure * 2;
				break;
			case 1:
				new_weapon = new Weapon(item_id, "items/weapon_file.txt");
				new_weapon->change_inventory_x(X);
				new_weapon->change_inventory_y(Y);
				equipment[i] = new_weapon;
				X += measure * 2;
				break;
			case 2:
				switch (item_id / 10000 % 10)
				{
				case 1:
					new_helmet = new Armour(item_id, "items/armour_file.txt");
					new_helmet->change_inventory_x(X);
					new_helmet->change_inventory_y(Y);
					equipment[i] = new_helmet;
					X += measure * 2;
					break;
				case 2:
					new_chestplate = new Armour(item_id, "items/armour_file.txt");
					new_chestplate->change_inventory_x(X);
					new_chestplate->change_inventory_y(Y);
					equipment[i] = new_chestplate;
					X += measure * 2;
					break;
				case 3:
					new_boots = new Armour(item_id, "items/armour_file.txt");
					new_boots->change_inventory_x(X);
					new_boots->change_inventory_y(Y);
					equipment[i] = new_boots;
					X += measure * 2;
					break;
				case 4:
					new_amulet = new Armour(item_id, "items/armour_file.txt");
					new_amulet->change_inventory_x(X);
					new_amulet->change_inventory_y(Y);
					equipment[i] = new_amulet;
					X += measure * 2;
					break;
				case 5:
					new_ring = new Armour(item_id, "items/armour_file.txt");
					new_ring->change_inventory_x(X);
					new_ring->change_inventory_y(Y);
					equipment[i] = new_ring;
					X += measure * 2;
					break;
				}
			}
		}
		file >> size;
		for (int i = 0; i < size; i++)
			inventory.push_back(nullptr);
		while (!file.eof())
		{
			file >> item_id >> X >> Y;
			switch (item_id / 100000)
			{
			case 0: //brak itema
				break;
			case 1:
				new_weapon = new Weapon(item_id, "items/weapon_file.txt");
				new_weapon->change_inventory_x(X);
				new_weapon->change_inventory_y(Y);
				add_item_to_inventory_x_y(new_weapon, 1);
				break;
			case 2:
				switch (item_id / 10000 % 10)
				{
				case 1:
					new_helmet = new Armour(item_id, "items/armour_file.txt");
					new_helmet->change_inventory_x(X);
					new_helmet->change_inventory_y(Y);
					add_item_to_inventory_x_y(new_helmet, 1);
					break;
				case 2:
					new_chestplate = new Armour(item_id, "items/armour_file.txt");
					new_chestplate->change_inventory_x(X);
					new_chestplate->change_inventory_y(Y);
					add_item_to_inventory_x_y(new_chestplate, 1);
					break;
				case 3:
					new_boots = new Armour(item_id, "items/armour_file.txt");
					new_boots->change_inventory_x(X);
					new_boots->change_inventory_y(Y);
					add_item_to_inventory_x_y(new_boots, 1);
					break;
				case 4:
					new_amulet = new Armour(item_id, "items/armour_file.txt");
					new_amulet->change_inventory_x(X);
					new_amulet->change_inventory_y(Y);
					add_item_to_inventory_x_y(new_amulet, 1);
					break;
				case 5:
					new_ring = new Armour(item_id, "items/armour_file.txt");
					new_ring->change_inventory_x(X);
					new_ring->change_inventory_y(Y);
					add_item_to_inventory_x_y(new_ring, 1);
					break;
				}
			}
		}
		backpack = al_load_bitmap("bitmaps/items/inventory_backpack.png");
		background = al_load_bitmap("bitmaps/items/inventory_background.png");
		player = al_load_bitmap("player/player_move.png");
		grill = al_load_bitmap("bitmaps/items/inventory_backpack_grill.png");
		file.close();
	}
}

Inventory::~Inventory()
{
	al_destroy_bitmap(backpack);
	al_destroy_bitmap(grill);
	for (int i = 0; i < inventory.size(); i++)
	{
		if (inventory[i])
		{
			inventory[i]->~Item();
			delete(inventory[i]);
		}
		inventory.clear();
	}

	if (player)
	{
		al_destroy_bitmap(background);
		al_destroy_bitmap(player);
		for (int i = 0; i < 7; i++)
		{
			if (equipment[i])
			{
				equipment[i]->~Item();
				delete(equipment[i]);
			}
		}
	}
	inventory.clear();
}

Item* Inventory::get_equipment(int i)
{
	return equipment[i];
}

Item* Inventory::get_item(int i)
{
	return inventory[i];
}

int Inventory::get_inventory_size()
{
	return inventory.size();
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

void Inventory::show_drop()
{
	al_draw_bitmap(backpack, 0, 0, 0);
	al_draw_bitmap_region(grill, 0, 0, 62, 2 + inventory.size() * measure, 30, 30, 0);
	for (int i = 0; i < inventory.size(); i++) //rysowanie przedmiotow z dropu
	{
		if (inventory[i])
			inventory[i]->draw_in_inventory();
	}
}

void Inventory::add_item_to_inventory(Item* new_item, int which_case)
{
	int new_x, new_y, base_x, max_x = 0;
	switch (which_case)
	{
	case 1:
		new_x = 1080;
		new_y = measure * 5;
		base_x = 1080;
		max_x = screen_width - measure * 2;
		break;
	case 2:
		new_x = measure * 0.5;
		new_y = measure * 0.5;
		base_x = measure * 0.5;
		max_x = measure * 0.5;
		break;
	}
	for (int i = 0; i < inventory.size(); i++)
	{
		if (inventory[i])
		{
			new_x += measure;
			if (inventory[i]->get_inventory_x() >= max_x)
			{
				new_x = base_x;
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

void Inventory::add_item_to_inventory_x_y(Item* new_item, int which_case)
{
	int x = new_item->get_inventory_x();
	int y = new_item->get_inventory_y();
	int tmp_x, tmp_y, base_x, max_x = 0;
	switch (which_case)
	{
	case 1:
		tmp_x = 1080;
		tmp_y = measure * 5;
		base_x = 1080;
		max_x = screen_width;
		break;
	case 2:
		tmp_x = measure * 0.5;
		tmp_y = measure * 0.5;
		base_x = measure * 0.5;
		max_x = measure * 0.5;
		break;
	}
	for (int i = 0; i < inventory.size(); i++)
	{
		if (tmp_x != x || tmp_y != y)
		{
			tmp_x += measure;
			if (tmp_x >= max_x)
			{
				tmp_x = base_x;
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

Item* Inventory::I_want_take_this_item(int sought_x, int sought_y, int& prev_x, int& prev_y, int which_case)
{
	int licznik = is_there_an_item(sought_x, sought_y, which_case);
	if(licznik<0)
		return nullptr;
	else
	{
		prev_x = inventory[licznik]->get_inventory_x();
		prev_y = inventory[licznik]->get_inventory_y();
		Item* tmp = inventory[licznik];
		inventory[licznik] = nullptr;
		return tmp;
	}
}

Item* Inventory::I_want_swap_this_item(int sought_x, int sought_y, Item* holding_item, int& prev_x, int& prev_y, Inventory* droped)
{
	int licznik = is_there_an_item(sought_x, sought_y, 1);
	if (licznik < -1)
	{
		holding_item->change_inventory_x(prev_x);
		holding_item->change_inventory_y(prev_y);
		if (prev_x < 1080) //je¿eli item z dropu
			droped->add_item_to_inventory_x_y(holding_item, 2);
		else
			add_item_to_inventory_x_y(holding_item, 1);
		return nullptr;
	}
	else if (licznik < 0)
	{
		holding_item->change_inventory_x(sought_x - (sought_x % measure));
		holding_item->change_inventory_y(sought_y - (sought_y % measure));
		add_item_to_inventory_x_y(holding_item, 1);
		return nullptr;
	}
	else
	{
		holding_item->change_inventory_x(sought_x - (sought_x % measure));
		holding_item->change_inventory_y(sought_y - (sought_y % measure));
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
	if (licznik < 0)
		return holding_item;
	else
	{
		int tmp_case = can_I_equip_this(licznik, holding_item);
		Item* tmp_item = nullptr;
		switch (tmp_case)
		{
		case -1: //je¿eli siê nie da
			return holding_item;
		case 0: //je¿eli po prostu gitówa
			holding_item->change_inventory_x(sought_x - (sought_x % measure));
			holding_item->change_inventory_y(sought_y - (sought_y % measure));
			tmp_item = equipment[licznik];
			equipment[licznik] = holding_item;
			return tmp_item;
		case 1:  //je¿eli droñ dwurêczna i oba sloty zajête
			holding_item->change_inventory_x(sought_x - (sought_x % measure));
			holding_item->change_inventory_y(sought_y - (sought_y % measure));
			add_item_to_inventory(equipment[1], 1);
			equipment[1] = nullptr;
			tmp_item = equipment[0];
			equipment[0] = holding_item;
			return tmp_item;
		case 2: //jezeli broñ jednorêczna a jest za³o¿ona dwurêczna
			holding_item->change_inventory_x(sought_x - (sought_x % measure));
			holding_item->change_inventory_y(sought_y - (sought_y % measure));
			tmp_item = equipment[0];
			equipment[0] = nullptr;
			equipment[licznik] = holding_item;
			return tmp_item;
		case 3: //je¿eli broñ dwurêczna
			holding_item->change_inventory_x(1080);
			holding_item->change_inventory_y(measure * 2);
			if (equipment[0])
			{
				tmp_item = equipment[0];
				equipment[0] = holding_item;
				return tmp_item;
			}
			else if (equipment[1])
			{
				tmp_item = equipment[1];
				equipment[1] = nullptr;
				equipment[0] = holding_item;
				return tmp_item;
			}
			else
			{
				equipment[0] = holding_item;
				return nullptr;
			}
		}
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

int Inventory::can_I_equip_this(int licznik, Item* holding_item)
{
	int id = (holding_item->get_item_id() / 10000) % 10;
	switch (holding_item->get_item_id() / 100000)
	{
	case 1: //weapon
		if (licznik == 0 || licznik == 1) //jezeli slot broni
		{
			if (licznik == 1 && id == 2) //jezeli tarcza na prawa reke
				return -1;
			else if (id >= 3 && equipment[0] && equipment[1]) //je¿eli droñ dwurêczna i oba sloty zajête
			{
				int pom = 0;
				for (int i = 0; i < inventory.size(); i++)
				{
					if (!inventory[i])
					{
						pom++;
						if (pom == 2) //je¿eli dwa wolne miejsca w inventory
							return 1;
					}
				}
				return -1;
			}
			else if (id == 1 && equipment[0] && (equipment[0]->get_item_id() / 10000) % 10 >= 3) //jezeli broñ jednorêczna a jest za³o¿ona dwurêczna
				return 2;
			else if (id >= 3) //je¿eli broñ dwurêczna
				return 3;
			else //je¿eli po prostu gitówa
				return 0;
		}
		else
			return -1;
	case 2: //armour
		if (licznik == id + 1) //jezeli odpowiedni slow w eq
			return 0;
		else
			return -1;
	}
	return -1;
}

int Inventory::is_there_an_item(int sought_x, int sought_y, int which_case)
{
	int tmp_x, tmp_y, max_x, max_y = 0;
	switch (which_case)
	{
	case 1:
		tmp_x = 1080;
		tmp_y = measure * 5;
		max_x = screen_width - measure;
		max_y = (tmp_y + measure * inventory.size() / 13);
		break;
	case 2:
		tmp_x = measure * 0.5;
		tmp_y = measure * 0.5;
		max_x = measure * 1.5;
		max_y = (tmp_y + measure * inventory.size());
		break;
	}
	if (sought_x >= tmp_x && sought_x <= max_x && sought_y >= tmp_y && sought_y <= max_y)
	{
		for (int i = 0; i < inventory.size(); i++)
		{
			if (inventory[i])
			{
				tmp_x = inventory[i]->get_inventory_x();
				tmp_y = inventory[i]->get_inventory_y();
				if (sought_x >= tmp_x && sought_x <= tmp_x + measure && sought_y >= tmp_y && sought_y <= tmp_y + measure)
					return i; //jest konkretny item
			}
		}
		return -1; //jest nullptr
	}
	else
		return -2; //nie ma tu itemu
}
