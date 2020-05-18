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
	double regenertion_cooldown;
	DIRECTION direction;
	bool is_moving;
	Character();
	Character(std::string file_name, std::string inventory_file);
	~Character();
	Character(int X, int Y, int seek_id, std::string& file_name);
	Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y);
	bool File_read(std::string& file_name); // false jak blad odczytu
	void character_moving(int animation_frames); // Jaka klatka powinna byc rysowana gdy postac sie porusza
	void character_not_moving(int animation_frames); // jaka klatka powinna byc rysowana gdy character stoi
	void what_move_should_I_draw(int animation_frames);
	void what_attack_should_I_draw(int animation_frames);
	void change_texture(std::string tmp);
	void resources_regeneration();
	int get_hp();
	int get_max_hp();
	int get_mana();
	int get_max_mana();
	int get_armour();
	int get_id();
	int get_lvl();
	int get_min_damage();
	int get_max_damage();
	void drop_file_get(std::fstream& file, std::string tmp_name, std::string drop_name, std::string tmp_drop_id, int tmp_drop_percent, std::vector<drop_element*>& what_can_I_drop); // odczytuje z pliku potrzebne rzeczy do dropu
	void create_drop(Inventory*& new_inventory, std::vector<drop_element*>& what_can_I_drop, int sum, int drawn);
	Inventory* get_inventory();
	Inventory* drop(std::string drop_name);
	void check_KILL_quests(std::vector<class Quest_line*>& quest_line, int i, class Location* & location); // sprawdza czy zosta³ wykonany el questa zwiazany z zabojstwem moba
	void get_damage(int dmg, Object***& map, class Location* &location, std::vector <class Quest_line*>& quest_line);
	void change_hp(int change);
	void change_mana(int change);
	int get_attack_type();
	void attack_player(Object * &player);
	int get_attitude();
	void change_attack_type(int tmp);
	int interact_up_down(int gap, Object***& map, Location*& location, Object*& player);
	int interact_right_left(int gap, Object***& map, Location*& location, Object*& player);
	int interact(Object*** &map, Location* &location, Object*& player);
	virtual void basic_attack(Object***& map, class Location* location, std::vector <class Quest_line*> quest_line) = 0;
	void add_bonuses();
	void remove_bonuses();
	int interaction();
	void draw_hp(ALLEGRO_BITMAP* hp_texture, int position_x, int position_y);
};

class Magnat : public Character
{
public:
	Magnat(int X, int Y, int id, std::string file_name);
	Magnat(std::string file_name, std::string inventory_file);
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