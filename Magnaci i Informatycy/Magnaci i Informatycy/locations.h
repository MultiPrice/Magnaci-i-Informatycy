#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
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
#include "locations.h"
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
	int X;
	int Y;
	std::string location_name;
	int toX;
	int toY;
	Travel_list* pNext;
};

class Location : public Object
{
protected:
	int id;
	ALLEGRO_BITMAP* texture;
	std::string name;
	TERRAIN terrain;
	int sizeX;
	int sizeY;
	Travel_list* pHead;
public:
	std::vector <Object*> mobs;
	Location(std::string location_name, int X, int Y, Object***& map);
	void read_info_file(std::string& location_name);
	void read_colision_file(std::string& colision_file, std::string& mob_file, Object***& map);
	void mob_file_read(std::string mob_file, Object***& map);
	void draw();
	void draw(int position_x, int position_y);
	void draw_mobs(int position_x, int position_y);
	void change_mob_coordinates(int changeX, int changeY);
	~Location();
};