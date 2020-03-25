#include "locations.h"

void add_travel(Travel_list*& pHead, std::string& name, int positionX, int positionY, int X, int Y)
{
	if (!pHead)
		pHead = new Travel_list{ X, Y, name, positionX, positionY, nullptr };
	else
	{
		Travel_list* temp = pHead;
		while (temp->pNext)
		{
			temp = temp->pNext;
		}
		temp->pNext = new Travel_list{ X, Y, name, positionX, positionY, nullptr };
	}
}

void delete_travel_list(Travel_list*& pHead)
{
	while (pHead)
	{
		Travel_list* temp = pHead->pNext;
		delete pHead;
		pHead = temp;
	}
}

std::string find_enum_name(int enum_nr)
{
	switch (enum_nr)
	{
	case 1:
		return "PLAINS";
	case 2:
		return "FOREST";
	case 3:
		return "DESERT";
	case 4:
		return "MOUNTAINS";
	case 5:
		return "ARCTIC";
	case 6:
		return "CITY";
	case 7:
		return "VILLAGE";
	case 8:
		return "CAVE";
	case 9:
		return "TOMB";
	case 10:
		return "MINE";
	case 11:
		return "INTERIOR";
	}
}

Location::Location(std::string location_name, int X, int Y, Object***& map)
{
	positionX = X;
	positionY = Y;
	id = 0;
	texture = nullptr;
	terrain = INTERIOR;
	pHead = nullptr;

	read_info_file(location_name);

	std::string colision_file = "Locations/" + find_enum_name(terrain) + "/" + name + "/colision.txt";
	std::string mob_file = "Locations/" + find_enum_name(terrain) + "/" + name + "/mobs.txt";

	read_colision_file(colision_file, mob_file, map);
}

void Location::read_info_file(std::string& location_name)
{
	std::fstream file;
	file.open("Locations/info.txt");
	if (file)
	{
		std::string check, trash, travel_name;
		int travel_positionX, travel_positionY;
		int read_terrain;
		file >> check;
		while (check != location_name)
		{
			file >> trash;
			while (trash != "}")
				file >> trash;
			file >> check;
		}
		file >> trash;
		name = check;
		file >> read_terrain;
		terrain = (TERRAIN)(read_terrain);
		file >> sizeX >> sizeY >> check;
		texture = al_load_bitmap(check.c_str());
		file >> travel_name;
		while (travel_name != "}")
		{
			file >> travel_positionX >> travel_positionY;
			file >> travel_name;
		}
		file.close();
	}
}

void Location::read_colision_file(std::string& colision_file, std::string& mob_file, Object***& map)
{
	std::fstream file;
	file.open(colision_file);
	if (file)
	{
		map = new Object **[sizeX];
		char tmp;
		for (int i = 0; i < sizeX; ++i)
			map[i] = new Object *[sizeY];
		for (int i = 0; i < sizeY; ++i)
		{
			for (int j = 0; j < sizeX; ++j)
			{
				file >> tmp;
				switch (tmp)
				{
				case 'O'://void
					map[j][i] = nullptr;
					break;
				case 'W'://wall
					map[j][i] = new Element(j, i, false);
					break;
				}
			}
		}
	}
	file.close();
	mob_file_read(mob_file, map);
}

void Location::mob_file_read(std::string mob_file, Object***& map)
{
	std::fstream file;
	file.open(mob_file);
	if (file)
	{
		int id;
		std::string trash;
		std::string bitmap_file;
		ALLEGRO_BITMAP* texture;
		int enums;
		int hp;
		int mana;
		int lvl;
		int min_damage;
		int max_damage;
		int critical_chance;
		int armor;
		int strength;
		int agility;
		int intelligence;
		int charisma;
		ATTITUDE attitude;
		int X;
		int Y;
		while (file)
		{
			file >> id >> trash >> bitmap_file;
			texture = al_load_bitmap(bitmap_file.c_str());
			enums = id / 10000 % 10;
			attitude = (ATTITUDE)(enums);
			file >> hp >> mana >> lvl >> min_damage >> max_damage >> critical_chance >> armor >> strength >> agility >> intelligence >> charisma >> trash;
			while (trash != "}")
			{
				X = stoi(trash);
				file >> Y;
				switch (id / 100000)
				{
				case 1:
					map[X][Y] = new Berserk(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y);
					//mobs.push_back(new Berserk(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y));
					mobs.push_back(map[X][Y]);
					break;
				case 2:
					map[X][Y] = new Paladin(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y);
					mobs.push_back(map[X][Y]);
					break;
				case 3:
					map[X][Y] = new Rogue(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y);
					mobs.push_back(map[X][Y]);
					break;
				case 4:
					map[X][Y] = new Mage(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y);
					mobs.push_back(map[X][Y]);
					break;
				case 5:
					map[X][Y] = new Native(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y);
					mobs.push_back(map[X][Y]);
					break;
				}
				file >> trash;
			}
		}
		file.close();
	}
}

void Location::draw()
{
	al_draw_bitmap_region(texture, positionX * measure, positionY * measure, screen_width, screen_height,0,0,0);
}

void Location::draw(int position_x, int position_y)//funckaj dostaje koordynaty gracza
{
	al_draw_bitmap_region(texture, (position_x-shiftX)*measure, (position_y-shiftY-1)*measure, screen_width, screen_height, 0, 0, 0);
}

void Location::draw_mobs(int position_x, int position_y)//funckaj dostaje koordynaty gracza
{
	for (int i = 0; i < mobs.size(); i++)
	{
		if (dynamic_cast<Character*>(mobs[i])->get_hp() == 0)
			mobs.erase(mobs.begin()+i);
		mobs[i]->draw(position_x - shiftX, position_y - shiftY);
	}
}

void Location::change_mob_coordinates(int changeX, int changeY)
{
	for (int i = 0; i < mobs.size(); i++)
		mobs[i]->change_coordinates(mobs[i]->get_X() + changeX, mobs[i]->get_Y() + changeY);
}

Location::~Location()
{
	delete_travel_list(pHead);
	al_destroy_bitmap(texture);
}
