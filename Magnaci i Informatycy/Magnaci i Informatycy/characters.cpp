#include "characters.h"

Character::Character()
{
	positionX = -1000;
	positionY = -1000;
	this->id = 0;
	hp = max_hp = max_mana = mana = lvl = min_damage = max_damage = armour = attack_type = 0;
	texture = nullptr;
	attitude = FRIEND;
	is_moving = false;
	movement_cooldown = al_get_time();
	direction = DOWN;
	inventory = new Inventory();
}

Character::Character(std::string file_name, std::string inventory_file, std::string& location_name)
{
	std::fstream file;
	file.open(file_name);
	if (file)
	{
		attack_type = 0;
		direction = DOWN;
		is_moving = false;
		movement_cooldown = al_get_time();
		regenertion_cooldown = al_get_time();
		int tmp_attitude;
		file >> positionX >> positionY >> name >> id >> hp >> max_hp >> mana >> max_mana >> lvl >> min_damage >> max_damage >> armour >> tmp_attitude >> location_name;
		attitude = (ATTITUDE)tmp_attitude;
		inventory = new Inventory(inventory_file);
		texture = al_load_bitmap("player/player_move.png");
		file.close();
	}
}

Character::~Character()
{
	inventory->~Inventory();
	//al_destroy_bitmap(texture);
	texture = nullptr;
}

Character::Character(int X, int Y, int seek_id, std::string& file_name)
{
	name = "";
	positionX = X;
	positionY = Y;
	id = seek_id;
	hp = mana = lvl = min_damage = max_damage = armour = 0;
	is_moving = false;
	texture = nullptr;
	attitude = FRIEND;
	movement_cooldown = al_get_time();
	direction = RIGHT;
	inventory = new Inventory();

	File_read(file_name);
}

Character::Character(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y)
{
	this->name = name;
	this->id = id;
	this->texture = texture;
	this->hp = this->max_hp = hp;
	this->mana = this->max_mana = mana;
	this->lvl = lvl;
	this->min_damage = min_damage;
	this->max_damage = max_damage;
	this->armour = armor;
	this->attitude = attitude;
	this->positionX = X;
	this->positionY = Y;
	this->is_moving = false;
	this->attack_type = 0;
	direction = DOWN;
	inventory = nullptr;
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
		file >> name >> hp >> mana >> lvl >> min_damage >> max_damage >> armour;
		max_hp = hp;
		max_mana = mana;
		file.close();
	}
	else return false;
	return true;
}

void Character::character_moving(int animation_frames)
{
	switch (direction)
	{
	case DOWN:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
			bitmap_start_x = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 0;
		break;
	case RIGHT:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
			bitmap_start_x = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 240;
		break;
	case UP:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
			bitmap_start_x = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 360;
		break;
	case LEFT:
		if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
			bitmap_start_x = 0;
		else
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = 120;
		break;
	}
}

void Character::character_not_moving(int animation_frames)
{
	switch (direction)
	{
	case DOWN:
		bitmap_start_x = 0;
		bitmap_start_y = 0;
		break;
	case RIGHT:
		bitmap_start_x = 0;
		bitmap_start_y = 240;
		break;
	case UP:
		bitmap_start_x = 0;
		bitmap_start_y = 360;
		break;
	case LEFT:
		bitmap_start_x = 0;
		bitmap_start_y = 120;
		break;
	}
}

void Character::what_move_should_I_draw(int animation_frames)
{
	if (is_moving)
		character_moving(animation_frames);
	else
		character_not_moving(animation_frames);
}

void Character::what_attack_should_I_draw(int animation_frames)
{
	if (bitmap_start_x >= measure * 1.5 * ((double)animation_frames - 1))
	{
		attack_type = 0;
		bitmap_start_x = 0;
	}
	else
		bitmap_start_x += measure * 1.5;
	switch (direction)
	{
	case DOWN:
		bitmap_start_y = 0;
		break;
	case RIGHT:
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*4;
		break;
	case UP:
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*6;
		break;
	case LEFT:
			bitmap_start_x += measure * 1.5;
		bitmap_start_y = measure*2;
		break;
	}
}

void Character::change_texture(std::string tmp)
{
	texture = al_load_bitmap(tmp.c_str());
}

void Character::resources_regeneration()
{
	regenertion_cooldown = al_get_time();
	change_hp(1);
	change_mana(2);
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

int Character::get_armour()
{
	return armour;
}

int Character::get_id()
{
	return id;
}

int Character::get_lvl()
{
	return lvl;
}

int Character::get_min_damage()
{
	return min_damage;
}

int Character::get_max_damage()
{
	return max_damage;
}

void Character::drop_file_get(std::fstream &file, std::string tmp_name, std::string drop_name, std::string tmp_drop_id, int tmp_drop_percent, std::vector<drop_element*>& what_can_I_drop)
{
	file >> tmp_name;
	if (tmp_name != drop_name)
	{
		file >> tmp_name;
		while (tmp_name != drop_name)
			file >> tmp_name;
	}
	file >> tmp_name;
	while (true)
	{
		file >> tmp_drop_id >> tmp_drop_percent;
		if (tmp_drop_id == "}")
			break;
		what_can_I_drop.push_back(new drop_element{ stoi(tmp_drop_id), tmp_drop_percent });
	}
}

void Character::create_drop(Inventory*& new_inventory, std::vector<drop_element*>& what_can_I_drop, int sum, int drawn)
{
	for (int i = 0; i < new_inventory->get_inventory_size(); i++)
		for (int j = 0; j < what_can_I_drop.size(); j++)
		{
			sum += what_can_I_drop[j]->drop_percent;
			if (drawn <= sum)
				switch (what_can_I_drop[j]->item_id / 100000)
				{
				case 1: //weapon
					new_inventory->add_item_to_inventory(new Weapon(what_can_I_drop[j]->item_id, "items/weapon_file.txt"), 2);
					break;
				case 2: //armour
					new_inventory->add_item_to_inventory(new Armour(what_can_I_drop[j]->item_id, "items/armour_file.txt"), 2);
					break;
				}
		}
}

Inventory* Character::get_inventory()
{
	return inventory;
}

Inventory* Character::drop(std::string drop_name, int how_many)
{
	Inventory* new_inventory = new Inventory(how_many);
	std::vector<drop_element*> what_can_I_drop;
	Item* new_item;
	std::fstream file;
	file.open("mob/drop.txt");
	if (file)
	{
		std::string tmp_name;
		std::string tmp_drop_id;
		int tmp_drop_percent = 0;

		drop_file_get(file, tmp_name, drop_name, tmp_drop_id, tmp_drop_percent, what_can_I_drop);
		int drawn = rand() % 100 + 1;
		int sum = 0;
		create_drop(new_inventory, what_can_I_drop, sum, drawn);
	}
	else
		return nullptr;
	file.close();
	return new_inventory;
}

void Character::check_KILL_quests(std::vector<Quest_line*>& quest_line, int i, Location*& location)
{
	for (int h = 0; h < quest_line.size(); h++)
		for (int j = 0; j < quest_line[h]->get_quest()->objective.size(); j++)
			if (quest_line[h]->get_quest()->objective[j]->check_objective(location))
				if (quest_line[h]->get_quest()->objective[j]->get_to_do() == KILL)
					if (dynamic_cast<Character*>(location->mobs[i])->get_id() == quest_line[h]->get_quest()->objective[j]->get_target_id())
						if (quest_line[h]->get_quest()->objective[j]->is_it_done())
						{
							quest_line[h]->get_quest()->objective.erase(quest_line[h]->get_quest()->objective.begin() + j);
							if (quest_line[h]->get_quest()->objective.empty())
								if (!quest_line[h]->take_next_quest())
								{
									quest_line[h]->~Quest_line();
									quest_line.erase(quest_line.begin() + h);
									h--;
									break;
								}
						}
}

void Character::get_damage(int dmg, Object *** &map, Location*& location, std::vector <Quest_line*>& quest_line)
{
	if (armour > dmg)
		return;
	if (attitude == FRIEND)
		return;
	if ((hp + armour) - dmg < 0)
		hp = 0;
	else
		hp = (hp + armour) - dmg;
	if (hp == 0)
	{
		map[positionX][positionY] = nullptr;
		for (int i = 0; i < location->mobs.size(); i++)
			if (dynamic_cast<Character*>(location->mobs[i])->get_hp() <= 0)
			{
				check_KILL_quests(quest_line, i, location);
				std::string dead_bitmap_file_name = "mob/" + name + "_dead.png";
				int how_many = 0;
				int do_I_drop = 0;
				if (name == "mob1")
				{
					how_many = rand() % 5 + 1;
					do_I_drop = rand() % 3;
				}
				if (name == "mob2")
				{
					how_many = rand() % 2 + 1;
					do_I_drop = rand() % 3;
				}
				if (name == "mob3")
				{
					how_many = 1;
					do_I_drop = 1;
				}
				switch (do_I_drop)
				{
				case 0:
					location->dead_mobs.push_back(new Dead_mobs(new Container(location->mobs[i]->get_X(), location->mobs[i]->get_Y(), true, dead_bitmap_file_name, "trup"), 600, nullptr));
					break;
				default:
					location->dead_mobs.push_back(new Dead_mobs(new Container(location->mobs[i]->get_X(), location->mobs[i]->get_Y(), true, dead_bitmap_file_name, "trup"), 600, drop(location->mobs[i]->get_name(), how_many)));
					break;
				}
				location->mobs.erase(location->mobs.begin() + i);
				return;
			}
	}
}

void Character::change_hp(int change)
{
	hp += change;
	if (hp <= 0)
	{
		std::cout << "YOU DIED" << std::endl;
		hp = 0;
	}
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

void Character::attack_player(Object*& player)
{
	dynamic_cast<Character*>(player)->change_hp(-(rand() % (max_damage - min_damage) + min_damage - dynamic_cast<Character*>(player)->get_armour()));
}

int Character::get_attitude()
{
	return (int)attitude;
}

void Character::change_attack_type(int tmp)
{
	attack_type = tmp;
}

int Character::interact_up_down(int gap, Object***& map, Location*& location, Object*& player)
{
	if (map[player->get_X()][player->get_Y() + gap] == nullptr)
	{
		for (int i = 0; i < location->dead_mobs.size(); i++)
		{
			if (location->find_dead_mob(player->get_X(), player->get_Y() + gap))
				if ((location->find_dead_mob(player->get_X(), player->get_Y() + gap))->droped)
					return 2;
		}
		return 0;
	}
	return map[player->get_X()][player->get_Y() + gap]->interaction();
}

int Character::interact_right_left(int gap, Object***& map, Location*& location, Object*& player)
{
	if (map[player->get_X() + gap][player->get_Y()] == nullptr)
	{
		for (int i = 0; i < location->dead_mobs.size(); i++)
		{
			if (location->find_dead_mob(player->get_X() + gap, player->get_Y()))
				if ((location->find_dead_mob(player->get_X() + gap, player->get_Y()))->droped)
					return 2;
		}
		return 0;
	}
	return map[player->get_X() + gap][player->get_Y()]->interaction();
}

int Character::interact(Object*** &map, Location* &location, Object* &player)
{
	switch (direction)
	{
	case UP:
		if (player->get_Y() - 1 > 0)
			return interact_up_down(-1, map, location, player);
		return 0;
	case DOWN:
		if (player->get_Y() + 1 < location->get_sizeY())
			return interact_up_down(1, map, location, player);
		return 0;
	case RIGHT:
		if (player->get_X() + 1 < location->get_sizeX())
			return interact_right_left(1, map, location, player);
		return 0;
	case LEFT:
		if (player->get_X() - 1 > 0)
			return interact_right_left(- 1, map, location, player);
		return 0;
	}
}

int Character::interaction()
{
	return 3;
}

void Character::draw_hp(ALLEGRO_BITMAP* hp_texture, int position_x, int position_y)
{
	al_draw_bitmap_region(hp_texture, 0, 0, measure * hp / max_hp, 10, (positionX - position_x) * measure, (positionY - position_y) * measure, 0);
}

void Character::add_bonuses()
{
	Item* new_item;
	for(int i = 0; i < 7; i++)
	{
		if (new_item = inventory->get_equipment(i))
		{
			if (new_item->get_item_id() / 100000 == 1) //weapon
			{
				Weapon* weapon_tmp = dynamic_cast<Weapon*>(new_item);
				min_damage += weapon_tmp->get_min_damage();
				max_damage += weapon_tmp->get_max_damage();
			}
			else if (new_item->get_item_id() / 100000 == 2) //armour
			{
				Armour* armour_tmp = dynamic_cast<Armour*>(new_item);
				armour += armour_tmp->get_armour();
			}
		}
	}
}

void Character::remove_bonuses()
{
	Item* old_item;
	for (int i = 0; i < 7; i++)
	{
		if (old_item = inventory->get_equipment(i))
		{
			if (old_item->get_item_id() / 100000 == 1) //weapon
			{
				Weapon* weapon_tmp = dynamic_cast<Weapon*>(old_item);
				min_damage -= weapon_tmp->get_min_damage();
				max_damage -= weapon_tmp->get_max_damage();
			}
			else if (old_item->get_item_id() / 100000 == 2) //armour
			{
				Armour* armour_tmp = dynamic_cast<Armour*>(old_item);
				armour -= armour_tmp->get_armour();
			}
		}
	}
}

Magnat::Magnat(int X, int Y, int id, std::string file_name)
	: Character(X, Y, id, file_name)
{
}

Magnat::Magnat(std::string file_name, std::string inventory_file, std::string& location_name)
	: Character(file_name, inventory_file, location_name)
{
}

Magnat::Magnat(std::string name, int id, ALLEGRO_BITMAP* texture, int hp, int max_hp, int mana, int max_mana, int lvl, int min_damage, int max_damage, int armor, ATTITUDE attitude, int X, int Y)
	: Character(name, id, texture, hp, max_hp, mana, max_mana, lvl, min_damage, max_damage, armor, attitude, X, Y)
{}

Magnat::~Magnat()
{
	al_destroy_bitmap(texture);
	texture = nullptr;
}

void Magnat::draw(Object ***map, int max_x, int max_y) // rysuje gracza
{
	al_draw_bitmap_region(texture, bitmap_start_x, bitmap_start_y, measure * 1.5, measure * 2, shiftX * measure - (0.25 * measure), shiftY * measure, 0);
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
		if (positionY + j >= max_y)
			return;
		map[positionX][positionY + j]->draw(positionX - shiftX, positionY - shiftY);
		j++;
	}
}

void Magnat::draw(int position_x, int position_y) //rysuje moba
{
	al_draw_bitmap_region(texture, bitmap_start_x, bitmap_start_y, measure * 1.5, measure * 2, (positionX - position_x) * measure - (0.25 * measure), (positionY - position_y) * measure, 0);
	//al_draw_bitmap(texture, (positionX - position_x) * measure, (positionY - position_y) * measure, 0);
}

void Magnat::basic_attack(Object***& map, Location* location, std::vector <Quest_line*> quest_line)
{
	int damage = rand() % (max_damage - min_damage) + min_damage;
	switch (direction)
	{
	case UP:
		for (int i = positionY - 1; i < positionY; i++)
		{
			for (int j = positionX - 1; j < positionX + 2; j++)
				if (map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Teleport) && map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Other_element) && map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Container))
					dynamic_cast<Character*>(map[j][i])->get_damage(damage, map, location, quest_line);
		}
		break;
	case RIGHT:
		for (int i = positionY - 1; i < positionY + 2; i++)
		{
			if (map[positionX + 1][i] != nullptr && typeid(*map[positionX + 1][i]) != typeid(Teleport) && map[positionX + 1][i] != nullptr && typeid(*map[positionX + 1][i]) != typeid(Container) && map[positionX + 1][i] != nullptr && typeid(*map[positionX + 1][i]) != typeid(Other_element))
				dynamic_cast<Character*>(map[positionX + 1][i])->get_damage(damage, map, location, quest_line);
		}
		break;
	case DOWN:
		for (int i = positionY + 1; i < positionY + 2; i++)
		{
			for (int j = positionX - 1; j < positionX + 2; j++)
				if (map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Teleport) && map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Container) && map[j][i] != nullptr && typeid(*map[j][i]) != typeid(Other_element))
					dynamic_cast<Character*>(map[j][i])->get_damage(damage, map, location, quest_line);
		}
		break;
	case LEFT:
		for (int i = positionY - 1; i < positionY + 2; i++)
		{
			if (map[positionX - 1][i] != nullptr && typeid(*map[positionX - 1][i]) != typeid(Teleport) && map[positionX - 1][i] != nullptr && typeid(*map[positionX - 1][i]) != typeid(Container) && map[positionX - 1][i] != nullptr && typeid(*map[positionX - 1][i]) != typeid(Other_element))
				dynamic_cast<Character*>(map[positionX - 1][i])->get_damage(damage, map, location, quest_line);
		}
		break;
	}
}