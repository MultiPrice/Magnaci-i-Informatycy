#include "characters.h"

Character::Character()
{
	positionX = -1000;
	positionY = -1000;
	this->id = 0;
	hp = mana = lvl = min_damage = max_damage = critical_chance = armor = strength = agility = intelligence = charisma = 0;
	texture = nullptr;
	attitude = FRIEND;
	is_moving = false;
	movement_cooldown = al_get_time();
	direction = RIGHT;
	weapon = nullptr;
//	equipment { nullptr, nullptr,nullptr, nullptr ,nullptr, nullptr };
	for (int i = 0; i < 6; i++)
		equipment[i] = nullptr;
}

Character::Character(int X, int Y, int seek_id, std::string& file_name)
{
	name = "";
	positionX = X;
	positionY = Y;
	this->id = seek_id;
	hp = mana = lvl = min_damage = max_damage = critical_chance = armor = strength = agility = intelligence = charisma = 0;
	texture = nullptr;
	attitude = FRIEND;
	movement_cooldown = al_get_time();
	direction = RIGHT;
	weapon = nullptr;
	for (int i = 0; i < 6; i++)
		equipment[i] = nullptr;

	File_read(file_name);
}

Character::Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
{
	this->name = name;
	this->id = id;
	this->texture = texture;
	this->hp = hp;
	this->mana = mana;
	this->lvl = lvl;
	this->min_damage = min_damage;
	this->max_damage = max_damage;
	this->critical_chance = critical_chance;
	this->armor = armor;
	this->strength = strength;
	this->agility = agility;
	this->intelligence = intelligence;
	this->charisma = charisma;
	this->attitude = attitude;
	this->positionX = X;
	this->positionY = Y;
	this->is_moving = false;
	this->attack_type = 0;
	direction = RIGHT;
	weapon = nullptr;
	for (int i = 0; i < 6; i++)
		equipment[i] = nullptr;
	movement_cooldown = al_get_time();
}

bool Character::File_read(std::string& file_name)
{
	std::fstream file;
	file.open(file_name);
	int seek_id;
	std::string trash;
	if (file)
	{
		file >> seek_id;
		int enums;
		enums = seek_id / 10000 % 10;
		attitude = (ATTITUDE)(enums);
		file >> trash;
		if (trash != "{")
			return false;
		std::string bitmap_file;
		file >> bitmap_file;
		texture = al_load_bitmap(bitmap_file.c_str());
		file >> name >> hp >> mana >> lvl >> min_damage >> max_damage >> critical_chance >> armor >> strength >> agility >> intelligence >> charisma;
		file.close();
	}
	else return false;
	return true;
}

void Character::what_move_should_I_draw()
{
	if (is_moving)
	{
		switch (direction)
		{
		case UP:
			if (bitmap_start_x == 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 0;
			break;
		case RIGHT:
			if (bitmap_start_x == 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 240;
			break;
		case DOWN:
			if (bitmap_start_x == 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 360;
			break;
		case LEFT:
			if (bitmap_start_x == 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 120;
			break;
		}
	}
	else
	{
		switch (direction)
		{
		case UP:
			bitmap_start_x = 0;
			bitmap_start_y = 0;
			break;
		case RIGHT:
			bitmap_start_x = 0;
			bitmap_start_y = 240;
			break;
		case DOWN:
			bitmap_start_x = 0;
			bitmap_start_y = 360;
			break;
		case LEFT:
			bitmap_start_x = 0;
			bitmap_start_y = 120;
			break;
		}
	}
}

void Character::what_attack_should_I_draw(int animation_frames)
{
	switch (direction)
	{
	case UP:
		if (bitmap_start_x == measure*1.5*animation_frames)
			this->attack_type = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 0;
		break;
	case RIGHT:
		if (bitmap_start_x == measure * 1.5 * animation_frames)
			this->attack_type = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*4;
		break;
	case DOWN:
		if (bitmap_start_x == measure * 1.5 * animation_frames)
			this->attack_type = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*6;
		break;
	case LEFT:
		if (bitmap_start_x == measure * 1.5 * animation_frames)
			this->attack_type = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*2;
		break;
	}
}

void Character::change_texture(std::string tmp)
{
	texture = al_load_bitmap(tmp.c_str());
}

int Character::get_hp()
{
	return hp;
}

void Character::get_damage(int dmg, Object *** &map, std::vector <Object*> &mobs)
{
	if (armor > dmg)
		return;
	hp = (hp + armor) - dmg;
	if (hp <= 0)
	{
		map[positionX][positionY] = nullptr;
		for (int i = 0; i < mobs.size(); i++)
		{
			if (dynamic_cast<Character*>(mobs[i])->get_hp() <= 0)
				mobs.erase(mobs.begin() + i);
		}
	}
}

int Character::get_attack_type()
{
	return attack_type;
}

int Character::get_attitude()
{
	return (int)attitude;
}

void Character::change_attack_type(int tmp)
{
	attack_type = tmp;
}

Magnat::Magnat(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Magnat::Magnat(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(name, id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y)
{}

Magnat::~Magnat()
{
	al_destroy_bitmap(texture);
}

void Magnat::draw(Object ***map)// rysuje gracza
{
	al_draw_bitmap_region(texture, bitmap_start_x, bitmap_start_y, measure * 1.5, measure * 2, shiftX * measure, shiftY * measure, 0);
	int n = 0;
	while (true)
	{
		if (map[positionX][positionY + n + 1] == nullptr)
			break;
		else
			n++;
	}
	int j = 1;
	while (n >= j)
	{
		dynamic_cast<Character*>(map[positionX][positionY + j])->draw(positionX - shiftX, positionY - shiftY);
		j++;
	}
	//al_draw_bitmap(texture, shiftX * measure, shiftY * measure, 0);
}

void Magnat::draw(int position_x, int position_y)//rysuje moba
{
	//al_draw_bitmap_region(this->texture, bitmap_start_x, bitmap_start_y, measure*1.5, measure*2, position_x * measure, position_y * measure, 0);
	//al_draw_bitmap(texture, positionX * measure, positionY * measure, 0);
	al_draw_bitmap(texture, (positionX - position_x) * measure, (positionY - position_y) * measure, 0);
}

void Magnat::basic_attack(Object***& map, std::vector <Object*>& mobs)
{
	int damage = rand() % (max_damage - min_damage) + min_damage;
	switch (direction)
	{
	case UP:
		for (int i = positionY - 1; i < positionY; i++)
		{
			for (int j = positionX - 1; j < positionX + 2; j++)
				if (map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Element))
					dynamic_cast<Character*>(map[j][i])->get_damage(damage, map, mobs);
		}
		break;
	case RIGHT:
		for (int i = positionY - 1; i < positionY + 2; i++)
		{
			if (map[positionX + 1][i] != nullptr && typeid(*map[positionX + 1][i]) != typeid(Element))
				dynamic_cast<Character*>(map[positionX + 1][i])->get_damage(damage, map, mobs);
		}
		break;
	case DOWN:
		for (int i = positionY + 1; i < positionY + 2; i++)
		{
			for (int j = positionX - 1; j < positionX + 2; j++)
				if (map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Element))
					dynamic_cast<Character*>(map[j][i])->get_damage(damage, map, mobs);
		}
		break;
	case LEFT:
		for (int i = positionY - 1; i < positionY + 2; i++)
		{
			if (map[positionX - 1][i] != nullptr && typeid(*map[positionX - 1][i]) != typeid(Element))
				dynamic_cast<Character*>(map[positionX - 1][i])->get_damage(damage, map, mobs);
		}
		break;
	}
}