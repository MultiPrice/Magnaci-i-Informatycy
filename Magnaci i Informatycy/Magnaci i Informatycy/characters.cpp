#include "characters.h"

Character::Character()
{
}

Character::Character(int X, int Y, int seek_id, std::string& file_name)
{
	positionX = X;
	positionY = Y;
	this->id = seek_id;
	hp = mana = lvl = min_damage = max_damage = critical_chance = armor = strength = agility = intelligence = charisma = 0;
	texture = nullptr;
	attitude = FRIEND;
	movement_cooldown = al_get_time();

	File_read(file_name);
}

Character::Character(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
{
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
	this->movement_cooldown = al_get_time();
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
		file >> hp >> mana >> lvl >> min_damage >> max_damage >> critical_chance >> armor >> strength >> agility >> intelligence >> charisma;
		file.close();
	}
	else return false;
	return true;
}

Berserk::Berserk(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Berserk::Berserk(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y)
{}

Berserk::~Berserk()
{
	al_destroy_bitmap(texture);
}

void Berserk::draw()
{
	al_draw_bitmap(this->texture, 16 * measure, 8 * measure, 0);
}

void Berserk::draw(int position_x, int position_y)
{
	al_draw_bitmap_region(this->texture, bitmap_start_x, bitmap_start_y, measure*1.5, measure*2, position_x * measure, position_y * measure, 0);
}

void Berserk::test()
{
}

Paladin::Paladin(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Paladin::Paladin(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y) {}

Paladin::~Paladin()
{
	al_destroy_bitmap(texture);
}

void Paladin::draw()
{
	al_draw_bitmap(this->texture, positionX * measure, positionY * measure, 0);
}

void Paladin::draw(int X, int Y)
{
	al_draw_bitmap(this->texture, X * measure, Y * measure, 0);
}

Rogue::Rogue(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Rogue::Rogue(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y) {}

Rogue::~Rogue()
{
	al_destroy_bitmap(texture);
}

void Rogue::draw()
{
	al_draw_bitmap(this->texture, positionX * measure, positionY * measure, 0);
}

void Rogue::draw(int X, int Y)
{
	al_draw_bitmap(this->texture, X * measure, Y * measure, 0);
}

Mage::Mage(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Mage::Mage(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y) {}

Mage::~Mage()
{
	al_destroy_bitmap(texture);
}

void Mage::draw()
{
	al_draw_bitmap(this->texture, positionX * measure, positionY * measure, 0);
}

void Mage::draw(int X, int Y)
{
	al_draw_bitmap(this->texture, X * measure, Y * measure, 0);
}

Native::Native(int X, int Y, int id, std::string file_name) : Character(X, Y, id, file_name) {}

Native::Native(int id, ALLEGRO_BITMAP* texture, int hp, int mana, int lvl, int min_damage, int max_damage, int critical_chance, int armor, int strength, int agility, int intelligence, int charisma, ATTITUDE attitude, int X, int Y)
	: Character(id, texture, hp, mana, lvl, min_damage, max_damage, critical_chance, armor, strength, agility, intelligence, charisma, attitude, X, Y) {}

Native::~Native()
{
	al_destroy_bitmap(texture);
}

void Native::draw()
{
	al_draw_bitmap(this->texture, positionX * measure, positionY * measure, 0);
}

void Native::draw(int X, int Y)
{
	al_draw_bitmap(this->texture, X * measure, Y * measure, 0);
}
