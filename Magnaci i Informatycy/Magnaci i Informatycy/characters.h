#pragma once
#include <iostream>
#include <fstream>
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
#include "items.h"
#include "locations.h"
#include "element.h"
#include "quest.h"

enum ATTITUDE
{
	FRIEND = 1,
	NEUTRAL,
	ENEMY,
	PLAYER
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
	int hp;
	int max_hp;
	int mana;
	int max_mana;
	int lvl;
	int min_damage;
	int max_damage;
	int armour;
	int attack_type;
	ATTITUDE attitude;
	Inventory* inventory;

public:
	double movement_cooldown;
	DIRECTION direction;
	bool is_moving;
	Character();
	~Character();
	Character(int X, int Y, int seek_id, std::string& file_name);
	Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y);
	bool File_read(std::string& file_name); // false jak blad odczytu
	void what_move_should_I_draw();
	void what_attack_should_I_draw(int animation_frames);
	void change_texture(std::string tmp);
	int get_hp();
	int get_max_hp();
	int get_mana();
	int get_max_mana();
	int get_armour();
	int get_id();
	Inventory* get_inventory();
	Inventory* drop(std::string drop_name);
	void get_damage(int dmg, Object***& map, class Location* location, std::vector <class Quest_line*>& quest_line);
	void change_hp(int change);
	void change_mana(int change);
	int get_attack_type();
	int get_attitude();
	void change_attack_type(int tmp);
	int interact(Object*** map, Location* location, Object* player);
	virtual void basic_attack(Object***& map, class Location* location, std::vector <class Quest_line*> quest_line) = 0;
	void add_bonuses();
	void remove_bonuses();
	int interaction();
};

class Magnat : public Character
{
public:
	Magnat(int X, int Y, int id, std::string file_name);
	Magnat(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y);
	~Magnat();
	void draw(Object ***map, int max_x, int max_y);
	void draw(int position_x, int position_y);
	void basic_attack(Object *** &map, Location* location, std::vector <Quest_line*> quest_line);
};

class Informatyk : public Character
{
public:
	Informatyk(int X, int Y, int id, std::string file_name);
	Informatyk(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Informatyk();
	void draw();
	void draw(int position_x, int position_y);
};