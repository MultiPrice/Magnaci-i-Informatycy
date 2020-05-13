#include "locations.h"

void add_travel(Travel_list*& pHead, std::string& name, int X, int Y, int toX, int toY)
{
	if (!pHead)
	{
		pHead = new Travel_list{ X, Y, name, toX, toY, nullptr };
		return;
	}
	else
	{
		Travel_list* temp = pHead;
		while (temp->pNext)
		{
			temp = temp->pNext;
		}
		temp->pNext = new Travel_list{ X, Y, name, toX, toY, nullptr };
	}
}

void add_portals(Travel_list* &pHead, Object*** & map)
{
	if (!pHead)
		return;
	else
	{
		Travel_list* tym = pHead;
		while (tym)
		{
			map[tym->X][tym->Y] = new Teleport(tym->X, tym->Y, true, "Element/portal.png", "portal");
			tym = tym->pNext;
		}
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
	pHead = nullptr;
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

Dead_mobs::Dead_mobs(Object* mob, int duration, Inventory* droped)
{
	this->mob = mob;
	this->duration = duration;
	this->droped = droped;
}

Dead_mobs::~Dead_mobs()
{
	dynamic_cast<Container*>(mob)->~Container();
	delete mob;
	droped->~Inventory();	
}

Inventory* Dead_mobs::get_drop()
{
	return droped;
}

void Dead_mobs::wypisz_w_dupe()
{
	for (int i = 0; i < droped->get_inventory_size(); i++)
		std::cout<< droped->get_item(i)->get_item_id() << std::endl;
}



Location::Location(std::string location_name, int X, int Y, Object***& map)
{
	positionX = X;
	positionY = Y;
	id = 0;
	texture = nullptr;
	terrain = INTERIOR;
	pTravel = nullptr;

	read_info_file(location_name);

	std::string colision_file = "Locations/" + find_enum_name(terrain) + "/" + name + "/colision.txt";
	std::string mob_file = "Locations/" + find_enum_name(terrain) + "/" + name + "/mobs.txt";
	std::string travel_list_file = "Locations/" + find_enum_name(terrain) + "/" + name + "/travel_list.txt";

	read_colision_file(colision_file, map);
	mob_file_read(mob_file, map);
	read_travel_file(travel_list_file);
	add_portals(pTravel, map);
}

int Location::get_sizeX()
{
	return sizeX;
}

int Location::get_sizeY()
{
	return sizeY;
}

int Location::get_id()
{
	return id;
}

Object* Location::get_mob(std::string name)
{
	for (int i = 0; i < mobs.size(); i++)
		if (mobs[i]->get_name() == name)
			return mobs[i];
	return nullptr;
}

Dead_mobs* Location::find_dead_mob(int X, int Y)
{
	for (int i = 0; i < dead_mobs.size(); i++)
	{
		if (dead_mobs[i]->mob->get_X() == X && dead_mobs[i]->mob->get_Y() == Y)
			return dead_mobs[i];
	}
	return nullptr;
}

std::string Location::get_terrain_name()
{
	switch ((int)terrain)
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

Travel_list* Location::get_pTravel()
{
	return pTravel;
}

void Location::read_travel_file (std::string travel_list_file)
{
	std::fstream file;
	file.open(travel_list_file);
	std::string travel_name;
	int travel_X, travel_Y, travel_to_X, travel_to_Y = 0;
	char trash;
	if (file)
	{
		while (!file.eof())
		{
			file >> travel_name >> trash >> travel_X >> travel_Y >> travel_to_X >> travel_to_Y >> trash;
			if (file.eof())
			{
				file.close();
				return;
			}
			add_travel(pTravel, travel_name, travel_X, travel_Y, travel_to_X, travel_to_Y);
		}
	}
}

std::string Location::search_travel(int wanted_X, int wanted_Y)
{
	Travel_list* tmp = pTravel;
	while (tmp)
	{
		if (wanted_X == tmp->X && wanted_Y == tmp->Y)
			return tmp->location_name;
		tmp = tmp->pNext;
	}
	return "Error: Nie znaleziono teleporta";
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
		file >> id;
		//file >> read_terrain;
		terrain = (TERRAIN)(id / 10);
		file >> sizeX >> sizeY >> check;
		texture = al_load_bitmap(check.c_str());
		file.close();
	}
}

void Location::read_colision_file(std::string& colision_file, Object***& map)
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
					map[j][i] = new Other_element(j, i, false, "Element/invisible.png", "invisible");
					break;
				}
			}
		}
	}
	file.close();
}

void Location::mob_file_read(std::string mob_file, Object***& map)
{
	std::fstream file;
	file.open(mob_file);
	if (file)
	{
		std::string name;
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
		int armor;
		ATTITUDE attitude;
		int X;
		int Y;
		while (file)
		{
			file >> id >> trash >> bitmap_file;
			texture = al_load_bitmap(bitmap_file.c_str());
			enums = id / 10000 % 10;
			attitude = (ATTITUDE)(enums);
			file >> name >> hp >> mana >> lvl >> min_damage >> max_damage >> armor >> trash;
			while (trash != "}")
			{
				X = stoi(trash);
				file >> Y;
				switch (id / 100000)
				{
				case 1:
					map[X][Y] = new Magnat(name, id, texture, hp, hp, mana, mana, lvl, min_damage, max_damage, armor, attitude, X, Y);
					mobs.push_back(map[X][Y]);
					break;
				}
				file >> trash;
			}
		}
		file.close();
	}
}

void Location::draw(Object ***map, int max_x, int max_y)
{
	al_draw_bitmap_region(texture, positionX * measure, positionY * measure, screen_width, screen_height,0,0,0);
}

void Location::draw(int position_x, int position_y)//funckaj dostaje koordynaty gracza
{
	al_draw_bitmap_region(texture, (position_x-shiftX)*measure, (position_y-shiftY-1)*measure, screen_width, screen_height, 0, 0, 0);
}

void Location::draw_mobs(int position_x, int position_y, Object*** map)//funckaj dostaje koordynaty gracza
{
	for (int i = 0; i < mobs.size(); i++)
	{
		int n = 0;
		while (true)
		{
			if (mobs[i]->get_Y() + n + 1 >= sizeY)
				break;
			if (map[mobs[i]->get_X()][mobs[i]->get_Y() + n + 1] == nullptr)
				break;
			else
				n++;
		}
		int j = 0;
		while (n >= j)
		{
			if (typeid(Element) == typeid(*map[mobs[i]->get_X()][mobs[i]->get_Y() + j]))
				map[mobs[i]->get_X()][mobs[i]->get_Y() + j]->draw(position_x - shiftX, position_y - shiftY);
			else if(dynamic_cast<Character*>(map[mobs[i]->get_X()][mobs[i]->get_Y() + j])->get_attitude() != 4)
				map[mobs[i]->get_X()][mobs[i]->get_Y() + j]->draw(position_x - shiftX, position_y - shiftY);
			j++;
		}
	}
}

void Location::draw_dead_mobs(int position_x, int position_y, Object*** map)
{
	for (int i = 0; i < dead_mobs.size(); i++)
	{
		if (dead_mobs[i]->duration <= 0)
		{
			//dead_mobs[i]->~Dead_mobs();
			Dead_mobs* tmp = dead_mobs[i];
			dead_mobs.erase(dead_mobs.begin() + i);
			i--;
			//tmp->~Dead_mobs();
		}
		else
		{
			dead_mobs[i]->mob->draw(position_x - shiftX, position_y - shiftY);
			dead_mobs[i]->duration--;
		}
	}
}

void Location::draw_portals(int position_x, int position_y, Object*** map)
{
	Travel_list* tmp = pTravel;
	while (tmp)
	{
		map[tmp->X][tmp->Y]->draw(position_x - shiftX, position_y - shiftY);
		tmp = tmp->pNext;
	}
}

void Location::mob_movement(Object* player, Object*** map)
{
	bool up, down, right, left;
	bool first, second, third, fourth;
	double distance;
	float x2, y2;
	Character* tmp_mob;
	for (int i = 0; i < mobs.size(); i++)
	{
		x2 = player->get_X() - mobs[i]->get_X();
		y2 = player->get_Y() - mobs[i]->get_Y();
		distance = sqrt(x2 * x2 + y2 * y2);

		tmp_mob = dynamic_cast<Character*>(mobs[i]);
		if(tmp_mob->get_attitude() == 3)
			if (distance < screen_height / measure / 2)
			{
				map[mobs[i]->get_X()][mobs[i]->get_Y()] = nullptr;
				up = left = right = down = false;
				first = second = third = fourth = false;

				if (map[mobs[i]->get_X()][mobs[i]->get_Y() - 1] == nullptr)
				{
					x2 = player->get_X() - mobs[i]->get_X();
					y2 = player->get_Y() - (mobs[i]->get_Y() - 1);
					if (distance > sqrt(x2 * x2 + y2 * y2))
						up = true;
				}
				if (map[mobs[i]->get_X()][mobs[i]->get_Y() + 1] == nullptr)
				{
					x2 = player->get_X() - mobs[i]->get_X();
					y2 = player->get_Y() - (mobs[i]->get_Y() + 1);
					if (distance > sqrt(x2 * x2 + y2 * y2))
						down = true;
				}
				if (map[mobs[i]->get_X() + 1][mobs[i]->get_Y()] == nullptr)
				{
					x2 = player->get_X() - (mobs[i]->get_X() + 1);
					y2 = player->get_Y() - mobs[i]->get_Y();
					if (distance > sqrt(x2 * x2 + y2 * y2))
						right = true;
				}
				if (map[mobs[i]->get_X() - 1][mobs[i]->get_Y()] == nullptr)
				{
					x2 = player->get_X() - (mobs[i]->get_X() - 1);
					y2 = player->get_Y() - mobs[i]->get_Y();
					if (distance > sqrt(x2 * x2 + y2 * y2))
						left = true;
				}

				if (!up && !down && !right && !left)
					tmp_mob->is_moving = false;
				else
					tmp_mob->is_moving = true;

					if ((player->get_X() > mobs[i]->get_X()))
					{
						if ((player->get_Y() > mobs[i]->get_Y()))
							fourth = true;
						else
							first = true;
					}
					else
					{
						if ((player->get_Y() > mobs[i]->get_Y()))
							third = true;
						else
							second = true;
					}

					if (first)
					{
						if (abs(player->get_X() - mobs[i]->get_X()) > abs(player->get_Y() - mobs[i]->get_Y()))
						{
							if (right)
							{
								mobs[i]->change_position(mobs[i]->get_X() + 1, mobs[i]->get_Y());
								tmp_mob->direction = RIGHT;
							}
							else if (up)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() - 1);
								tmp_mob->direction = UP;
							}
						}
						else
						{
							if (up)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() - 1);
								tmp_mob->direction = UP;
							}

							else if (right)
							{
								mobs[i]->change_position(mobs[i]->get_X() + 1, mobs[i]->get_Y());
								tmp_mob->direction = RIGHT;
							}
						}
					}
					else if (second)
					{
						if (abs(player->get_X() - mobs[i]->get_X()) > abs(player->get_Y() - mobs[i]->get_Y()))
						{
							if (left)
							{
								mobs[i]->change_position(mobs[i]->get_X() - 1, mobs[i]->get_Y());
								tmp_mob->direction = LEFT;
							}
							else if (up)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() - 1);
								tmp_mob->direction = UP;
							}
						}
						else
						{
							if (up)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() - 1);
								tmp_mob->direction = UP;
							}
							else if (left)
							{
								mobs[i]->change_position(mobs[i]->get_X() - 1, mobs[i]->get_Y());
								tmp_mob->direction = LEFT;
							}
						}
					}
					else if (third)
					{
						if (abs(player->get_X() - mobs[i]->get_X()) > abs(player->get_Y() - mobs[i]->get_Y()))
						{
							if (left)
							{
								mobs[i]->change_position(mobs[i]->get_X() - 1, mobs[i]->get_Y());
								tmp_mob->direction = LEFT;
							}
							else if (down)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() + 1);
								tmp_mob->direction = DOWN;
							}
						}
						else
						{
							if (down)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() + 1);
								tmp_mob->direction = DOWN;
							}
							else if (left)
							{
								mobs[i]->change_position(mobs[i]->get_X() - 1, mobs[i]->get_Y());
								tmp_mob->direction = LEFT;
							}
						}
					}
					else if (fourth)
					{
						if (abs(player->get_X() - mobs[i]->get_X()) > abs(player->get_Y() - mobs[i]->get_Y()))
						{
							if (right)
							{
								mobs[i]->change_position(mobs[i]->get_X() + 1, mobs[i]->get_Y());
								tmp_mob->direction = RIGHT;
							}
							else if (down)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() + 1);
								tmp_mob->direction = DOWN;
							}
						}
						else
						{
							if (down)
							{
								mobs[i]->change_position(mobs[i]->get_X(), mobs[i]->get_Y() + 1);
								tmp_mob->direction = DOWN;
							}
							else if (right)
							{
								mobs[i]->change_position(mobs[i]->get_X() + 1, mobs[i]->get_Y());
								tmp_mob->direction = RIGHT;
							}
						}
					}
					map[mobs[i]->get_X()][mobs[i]->get_Y()] = mobs[i];
			}
			else
				tmp_mob->is_moving = false;
		tmp_mob->what_move_should_I_draw();
	}
}

Location::~Location()
{
	delete_travel_list(pTravel);
	al_destroy_bitmap(texture);
	texture = nullptr;
	//for (int i = 0; i < mobs.size(); i++)
		//dynamic_cast<Character*>(mobs[i])->~Character();
	mobs.clear();
	//for (int i = 0; i < dead_mobs.size(); i++)
	//	delete dead_mobs[i];
	dead_mobs.clear();
}


