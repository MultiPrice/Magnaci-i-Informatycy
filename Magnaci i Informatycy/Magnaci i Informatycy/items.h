#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
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

//Type of item
enum class ITEM_TYPE
{
	WEAPON = 1,
	ARMOUR,
	FOOD,
	SPECIAL
};

//Type of gear-up
enum class ARMOUR_TYPE
{
	HELMET = 1,
	CHESTPLATE,
	BOOTS,
	AMULET,
	RING
};

//Type of weapon
enum class WEAPON_TYPE
{
	ONE_HANDED = 1,
	SHIELD,
	TWO_HANDED,
	BOW,
	STAFF
};

struct drop_element
{
	int item_id;
	int drop_percent;
};

//Main item class
class Item
{
protected:
	int id;
	ALLEGRO_BITMAP* texture;
	int cost;						//Item cost, Item Id
	std::string name;					//Item name
	std::string description;			//Item possible description
	int inventory_x;
	int inventory_y;

public:
	~Item();
	virtual bool File_read(std::string file_name) = 0;
	void draw_in_inventory();
	int get_inventory_x();
	int get_inventory_y();
	int get_item_id();
	void change_inventory_x(int new_x);
	void change_inventory_y(int new_y);
};


class Armour : public Item
{
protected:
	int min_lvl;
	int hero_class; //Coded binary
	int armor_points;			//Points of armour
	int speed;					//Spped or downturn
public:
	Armour(int id, std::string file_name);	//Constuctor of Armour
	bool File_read(std::string file_name);	//Reads armour list file and returns true if finds needed value
	int get_armour();
};

//------------------------------------------------------------

class Weapon : public Item
{
protected:
	int min_lvl;
	int hero_class; //Coded binary
	//Proporties of weapons
	int min_damage;
	int max_damage;
	int attack_speed;
public:
	Weapon(int id, std::string file_name);	//Constuctor of Weapon
	bool File_read(std::string file_name);	//Reads weapon list file and returns true if finds needed value
	int get_min_damage();
	int get_max_damage();
};

//------------------------------------------------------------

class Food : public Item
{
protected:
	//Proporties of food
	int health;
	int mana;
	std::string tp_dest;

public:
	Food(int id, std::string& file_name);	//Constructor of Food
	bool File_read(std::string& file_name);	//Reads food list file and returns true if finds needed value
};

//-------------------------------------------------------------

class Special : public Item
{
protected:
public:
	Special(int id, std::string& file_name);	//Constructor of Special
	bool File_read(std::string& file_name);	//Reads special list file and returns true if finds needed value
};

class Inventory
{
protected:
	std::vector<Item*> inventory;
	Item* equipment[7];
	ALLEGRO_BITMAP* backpack;
	ALLEGRO_BITMAP* background;
	ALLEGRO_BITMAP* player;
	ALLEGRO_BITMAP* grill;
public:
	Inventory();
	Inventory(int size);
	Inventory(std::string file_name);
	~Inventory();
	void first_part_item_file_read(std::fstream& file, int X, int Y, int item_id, Item*& new_weapon, Item*& new_helmet, Item*& new_chestplate, Item*& new_boots, Item*& new_amulet, Item*& new_ring); //Odczytuje z pliku informacje o dodawanym itemie, uzywanne w konstruktorze
	void second_part_item_file_read(std::fstream& file, int size, int X, int Y, int item_id, Item*& new_weapon, Item*& new_helmet, Item*& new_chestplate, Item*& new_boots, Item*& new_amulet, Item*& new_ring); //dodaje itemy odczytane z pliku do wektora i do samego inwentory
	void add_armour(int X, int Y, int i, int item_id, Item*& new_helmet, Item*& new_chestplate, Item*& new_boots, Item*& new_amulet, Item*& new_ring); //dodaje odpowiedni element pancerza w konstruktorze
	void add_armour_to_inventory(int X, int Y, int item_id, Item*& new_helmet, Item*& new_chestplate, Item*& new_boots, Item*& new_amulet, Item*& new_ring); //dodaje odpowiednia czesc armoura do inventory, wywoluje add_item_to_inventory_x_y
	Item* get_equipment(int i);
	Item* get_item(int i);
	int get_inventory_size();
	void show_inventory();
	void show_drop();
	void which_case_inventory(int gap, int which_case, int& new_x, int& new_y, int& base_x, int& max_x); //decyduje ktorego wariantu uzyc w funkcji add_item_to_inventory
	void add_item_to_inventory(Item* new_item, int which_case);
	void add_item_to_inventory_x_y(Item* new_item, int which_case);
	Item* I_want_take_this_item(int sought_x, int sought_y, int &prev_x, int &prev_y, int which_case);
	Item* I_want_swap_this_item(int sought_x, int sought_y, Item* holding_item, int& prev_x, int& prev_y, Inventory* droped);
	int is_there_an_item(int sought_x, int sought_y, int which_case);
	Item* I_want_take_this_equipment(int sought_x, int sought_y);
	Item* I_want_equip_this_item(int sought_x, int sought_y, Item* holding_item);
	int is_there_an_equipment(int sought_x, int sought_y);
	int can_I_equip_this(int licznik, Item* holding_item);
};