#pragma once
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

#include "general_functions.h"
#include "items.h"
#include "element.h"

enum ATTITUDE
{
	FRIEND = 1,
	NEUTRAL,
	ENEMY
};

enum DIRECTION
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Character : public Object
{
protected:
	int id;
	ALLEGRO_BITMAP* texture;
	int hp;// hit points
	int mana;
	int lvl;
	int min_damage;
	int max_damage;
	int critical_chance;
	int armor;
	int strength;// zwieksza dmg od broni bialej i luku, potrzebny do noszenia zbroi 
	int agility;// szybkosc ataku, szybkosc ruchu
	int intelligence;// zmniejsza koszt czarow, zwieksza obrazenia magiczne, odblokowywuje opcje dialogowe
	int charisma;// gadanie i cena, odblokowywuje opcje dialogowe
	ATTITUDE attitude;
	DIRECTION direction;
	std::vector<Item*> inventory;
	Item* equipment[7];

public:
	Character();
	Character(int X, int Y, int seek_id, std::string& file_name);
	Character(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	bool File_read(std::string& file_name); // false jak blad odczytu
	double movement_cooldown;
};

class Berserk : public Character
{
public:
	Berserk(int X, int Y, int id, std::string file_name);
	Berserk(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Berserk();
	void draw();
	void draw(int position_x, int position_y);
	int basic_attack(Object*** map);
};

class Paladin : public Character
{
public:
	Paladin(int X, int Y, int id, std::string file_name);
	Paladin(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Paladin();
	void draw();
	void draw(int X, int Y);
};

class Rogue : public Character
{
public:
	Rogue(int X, int Y, int id, std::string file_name);
	Rogue(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Rogue();
	void draw();
	void draw(int X, int Y);
};

class Mage : public Character
{
public:
	Mage(int X, int Y, int id, std::string file_name);
	Mage(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Mage();
	void draw();
	void draw(int X, int Y);
};

class Native : public Character
{
public:
	Native(int X, int Y, int id, std::string file_name);
	Native(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Native();
	void draw();
	void draw(int X, int Y);
};