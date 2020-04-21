#include "characters.h"

Character::Character()
{
	positionX = -1000;
	positionY = -1000;
	this->id = 0;
	hp = mana = lvl = min_damage = max_damage = armor = 0;
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
	id = seek_id;
	hp = mana = lvl = min_damage = max_damage = armor = 0;
	is_moving = false;
	texture = nullptr;
	attitude = FRIEND;
	movement_cooldown = al_get_time();
	direction = RIGHT;
	weapon = nullptr;
	for (int i = 0; i < 6; i++)
		equipment[i] = nullptr;

	File_read(file_name);
}

Character::Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y)
{
	this->name = name;
	this->id = id;
	this->texture = texture;
	this->hp = hp;
	this->mana = mana;
	this->lvl = lvl;
	this->min_damage = min_damage;
	this->max_damage = max_damage;
	this->armor = armor;
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
		file >> name >> hp >> mana >> lvl >> min_damage >> max_damage >> armor;
		max_hp = hp;
		max_mana = mana;
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
			if (bitmap_start_x >= 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 0;
			break;
		case RIGHT:
			if (bitmap_start_x >= 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 240;
			break;
		case DOWN:
			if (bitmap_start_x >= 720)
				bitmap_start_x = 0;
			else
				bitmap_start_x += measure * 1.5;
			bitmap_start_y = 360;
			break;
		case LEFT:
			if (bitmap_start_x >= 720)
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
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
		{
			attack_type = 0;
			bitmap_start_x = 0;
		}
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 0;
		break;
	case RIGHT:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
		{
			attack_type = 0;
			bitmap_start_x = 0;
		}
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*4;
		break;
	case DOWN:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
		{
			attack_type = 0;
			bitmap_start_x = 0;
		}
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*6;
		break;
	case LEFT:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
		{
			attack_type = 0;
			bitmap_start_x = 0;
		}
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

int Character::get_max_hp()
{
	return max_hp;
}

int Character::get_mana()
{
	return mana;
}

int Character::get_max_mana()
{
	return max_mana;
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

void Character::change_hp(int change)
{
	hp += change;
	if (hp <= 0)
		std::cout << "bohater_umiera";
	else if (hp > max_hp)
		hp = max_hp;
}

void Character::change_mana(int change)
{
	mana += change;
	if (mana < 0)
		mana = 0;
	else if (mana > max_mana)
		mana = max_mana;
}

int Character::get_attack_type()
{
	return attack_type;
}

void Character::change_attack_type(int tmp)
{
	attack_type = tmp;
}

Magnat::Magnat(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Magnat::Magnat(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y)
	: Character(name, id, texture, hp, max_hp, mana, max_mana, lvl, min_damage, max_damage, armor, attitude, X, Y)
{}

Magnat::~Magnat()
{
	al_destroy_bitmap(texture);
}

void Magnat::draw()// rysuje gracza
{
	al_draw_bitmap_region(texture, bitmap_start_x, bitmap_start_y, measure * 1.5, measure * 2, shiftX * measure, shiftY * measure, 0);
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