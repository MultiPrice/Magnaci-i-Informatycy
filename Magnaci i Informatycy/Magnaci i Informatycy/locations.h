#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
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

#include "element.h"
#include "characters.h"
#include "general_functions.h"

enum TERRAIN
{
	PLAINS = 1,
	FOREST = 2,
	DESERT = 3,
	MOUNTAINS = 4,
	ARCTIC = 5,
	CITY = 6,
	VILLAGE = 7,
	CAVE = 8,
	TOMB = 9,
	MINE = 10,
	INTERIOR = 11
};

struct Travel_list
{
	int X; // gdzie teleport
	int Y; // gdzie teleport
	std::string location_name;
	int toX; // gdzie gracz sie pojawi w tablicy
	int toY; // gdzie gracz sie pojawi w tablicy
	Travel_list* pNext;
};

struct Dead_mobs
{
	Object* mob; 
	int duration; // jak dlugo mob zostanie
	Inventory* droped;

	Dead_mobs(Object* mob, int duration, Inventory* droped);
	~Dead_mobs();
	Inventory* get_drop();
};

class Location : public Object
{
protected:
	int id;
	TERRAIN terrain;
	int sizeX;
	int sizeY;
	Travel_list* pTravel;
public:
	std::vector <Object*> mobs;
	std::vector <Dead_mobs*> dead_mobs;
	Location(std::string location_name, int X, int Y, Object***& map);
	int get_sizeX();
	int get_sizeY();
	int get_id();
	Object* get_mob(std::string name);
	Dead_mobs* find_dead_mob(int X, int Y);
	std::string get_terrain_name();
	Travel_list* get_pTravel(std::string location_name);
	void read_travel_file(std::string location_name);
	std::string search_travel(int wanted_X, int wanted_Y);
	void read_info_file(std::string& location_name);
	void read_colision_file(std::string& colision_file, Object***& map);
	void mob_file_read(std::string mob_file, Object***& map);
	void draw(Object ***map, int max_x, int max_y);
	void draw(int position_x, int position_y);
	void draw_mobs(int position_x, int position_y, Object*** map);
	void draw_dead_mobs(int position_x, int position_y, Object*** map);
	void draw_portals(int position_x, int position_y, Object*** map);
	void prepere_mob_attack(class Character*& tmp_mob, Object*& player, int dir); // ustawia moba na atak 
	void where_can_mob_go(Object***& map, int i, float x2, float y2, Object*& player, double distance, bool& up, bool& down, bool& right, bool& left); // Sprawdza w ktorym kierunku moze przemiescic sie mob
	void which_quater(class Character*& tmp_mob, int i, bool& up, bool& down, bool& right, bool& left, Object*& player, bool& first, bool& second, bool& third, bool& fourth); // decyduje na której æwiartce zglêdem moba jest gracz
	void check_first_quoter(class Character*& tmp_mob, Object*& player, int i, bool up, bool down, bool right, bool left); // jesli gracz jest w pierwszej cwiartce to szuka i wykonuje najszybsza droge
	void check_second_quoter(class Character*& tmp_mob, Object*& player, int i, bool up, bool down, bool right, bool left); // jesli gracz jest w drugiej cwiartce to szuka i wykonuje najszybsza droge
	void check_third_quoter(class Character*& tmp_mob, Object*& player, int i, bool up, bool down, bool right, bool left); // jesli gracz jest w trzeciej cwiartce to szuka i wykonuje najszybsza droge
	void check_fourth_quoter(class Character*& tmp_mob, Object*& player, int i, bool up, bool down, bool right, bool left); // jesli gracz jest w czwartej cwiartce to szuka i wykonuje najszybsza droge
	void mob_not_attacking(class Character*& tmp_mob, int i, bool& up, bool& down, bool& right, bool& left, Object*& player, bool& first, bool& second, bool& third, bool& fourth, float x2, float y2, double distance, Object***& map); // kiedy mob nie atakuje tylko sie porusza
	void mob_movement(Object* &player, Object*** map);
	void mob_attack(Object* mob_tmp);
	int interaction() { return 0; };
	~Location();
};