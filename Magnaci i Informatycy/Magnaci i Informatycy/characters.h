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
	std::string name;
	int id;
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
	int attack_type;
	double frames;
	ATTITUDE attitude;
	std::vector<Item*> inventory;
	Item* equipment[6];
	Weapon* weapon;

public:
	Character();
	Character(int X, int Y, int seek_id, std::string& file_name);
	Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	bool File_read(std::string& file_name); // false jak blad odczytu
	void what_move_should_I_draw();
	void what_attack_should_I_draw(int animation_frames);
	void change_texture(std::string tmp);
	int get_hp();
	void get_damage(int dmg, Object***& map, std::vector <Object*>& mobs);
	int get_attack_type();
	void change_attack_type(int tmp);
	double movement_cooldown;
	DIRECTION direction;
	bool is_moving;
	virtual void basic_attack(Object*** &map, std::vector <Object*>& mobs) = 0;
};

class Magnat : public Character
{
public:
	Magnat(int X, int Y, int id, std::string file_name);
	Magnat(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y);
	~Magnat();
	void draw();
	void draw(int position_x, int position_y);
	void basic_attack(Object *** &map, std::vector <Object*>& mobs);
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