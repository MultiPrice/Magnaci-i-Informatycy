#pragma once
#include <vector>
#include <string>
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

//Type of item
enum ITEM {
	ARMOUR,
	WEAPON,
	FOOD,
	SPECIAL
};

//Type of gear-up
enum ARMOUR_TYPE
{
	HELMET,
	CHESTPLATE,
	BOOTS,
	RING,
	AMULET
};

//Type of weapon
enum WEAPON_TYPE
{
	TWO_HANDED,
	ONE_HANDED,
	SHIELD,
	BOW,
	ARROW,
	DAGGER,
	STAFF
};

//Type of food
enum FOOD_TYPE
{
	//FOOD,          wywala³o b³¹d mo¿liwe dlatego, ¿e taka sama wartoœæ wystêpuje w enumie ITEM
	POTION,
	WEED,
	TP
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
	ITEM item;

public:
	virtual bool File_read(std::string& file_name) = 0;
};


class Armour : public Item
{
protected:
	int armor_points;			//Points of armour
	int speed;					//Spped or downturn
	int magic_resistance;		//Points of Magic Resistance

	//Proporties of wearable items
	int min_lvl;
	int hero_class; //Coded binary

	ARMOUR_TYPE armour_type;	//Type of armour (use ENUM)
public:
	Armour(int id, std::string& file_name);	//Constuctor of Armour
	bool File_read(std::string& file_name);	//Reads armour list file and returns true if finds needed value
};

//------------------------------------------------------------

class Weapon : public Item
{
protected:
	//Proporties of weapons
	int min_damage;
	int max_damage;
	int attack_speed;

	//Proporties of wearable items
	int min_lvl;
	int hero_class; //Coded binary

	WEAPON_TYPE weapon_type;	//Type of weapon (use ENUM)
public:
	Weapon(int id, std::string& file_name);	//Constuctor of Weapon
	bool File_read(std::string& file_name);	//Reads weapon list file and returns true if finds needed value
};

//------------------------------------------------------------

class Food : public Item
{
protected:
	//Proporties of food
	int health;
	std::string tp_dest;

	FOOD_TYPE food_type; //Type of food (use ENUM)
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