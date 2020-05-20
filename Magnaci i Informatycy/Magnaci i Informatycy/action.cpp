#include "action.h"

Action::Action(std::string name, int damage, int cost, double cast_time, double duration, double cooldown, int speed, Object*& player)
{
	this->name = name;
	this->damage = damage;
	this->cost = cost;
	this->cast_time = cast_time;
	this->duration = duration;
	this->cooldown = cooldown;
	this->speed = speed;
	representation = nullptr;
	dynamic_cast<Character*>(player)->change_mana(-cost);
}

void Action::prepare_action()
{
	cast_time--;
}

int Action::get_cast_time()
{
	return cast_time;
}

Element* Action::get_representation()
{
	return representation;
}

Action::~Action()
{
	delete representation;
}

AoE::AoE(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction, int effect_dash_x, int effect_dash_y)
	: Action(name, damage, cost, cast_time, duration, cooldown, speed, player)
{
	switch (direction)
	{
	case UP:
		center_x = positionX;
		center_y = positionY - 1;
		representation = new Other_element(positionX + effect_dash_x, positionY - 1 + effect_dash_y, piercing, texture_file, name);
		break;
	case DOWN:
		center_x = positionX;
		center_y = positionY + 1;
		representation = new Other_element(positionX + effect_dash_x, positionY + 1 + effect_dash_y, piercing, texture_file, name);
		break;
	case RIGHT:
		center_x = positionX + 1;
		center_y = positionY;
		representation = new Other_element(positionX + effect_dash_x + 1, positionY + effect_dash_y, piercing, texture_file, name);
		break;
	case LEFT:
		center_x = positionX - 1;
		center_y = positionY;
		representation = new Other_element(positionX + effect_dash_x - 1, positionY + effect_dash_y, piercing, texture_file, name);
		break;
	}
}

void AoE::try_make_effect(int i, Object***& map, Location*& location, std::vector <Quest_line*>& quest_line, int gap)
{
	if (i > 0 && i < location->get_sizeX())
	{
		if (center_y - gap > 0)
			if (map[i][center_y - gap] != nullptr)
				if (typeid(Teleport) != typeid(*map[i][center_y - gap]) && typeid(Container) != typeid(*map[i][center_y - gap]) && typeid(Other_element) != typeid(*map[i][center_y - gap]))
					if(dynamic_cast<Character*>(map[i][center_y - gap])->get_attitude() == 3)
						dynamic_cast<Character*>(map[i][center_y - gap])->get_damage(damage, map, location, quest_line);
		if (gap == 0)
			return;
		if (center_y + gap < location->get_sizeY())
			if (map[i][center_y + gap] != nullptr)
				if (typeid(Teleport) != typeid(*map[i][center_y + gap]) && typeid(Container) != typeid(*map[i][center_y + gap]) && typeid(Other_element) != typeid(*map[i][center_y + gap]))
					if (dynamic_cast<Character*>(map[i][center_y + gap])->get_attitude() == 3)
						dynamic_cast<Character*>(map[i][center_y + gap])->get_damage(damage, map, location, quest_line);
	}
}

bool AoE::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*> &quest_line)
{
	if (cast_time > 0)
	{
		cast_time--;
		return true;
	}
	if (countdown % speed != 0)
	{
		countdown++;
		return true;
	}
	countdown++;
	if (duration <= 0)
		return false;
	else
		duration--;
	
	for (int i = center_x - 3; i <= center_x + 3; i++)
	{
		try_make_effect(i, map, location, quest_line, 1);
		try_make_effect(i, map, location, quest_line, 0);
	}
	for (int i = center_x - 1; i <= center_x + 1; i++)
		try_make_effect(i, map, location, quest_line, 3);
	for (int i = center_x - 2; i <= center_x + 2; i++)
		try_make_effect(i, map, location, quest_line, 2);
	return true;
}

AoE::~AoE()
{
	delete representation;
}

Wind::Wind(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction)
	: Action(name, damage, cost, cast_time, duration, cooldown, speed, player)
{
	this->direction = direction;
	switch (direction)
	{
	case UP:
		representation = new Other_element(positionX, positionY - 1, piercing, texture_file, name);
		break;
	case DOWN:
		representation = new Other_element(positionX, positionY + 1, piercing, texture_file, name);
		break;
	case RIGHT:
		representation = new Other_element(positionX + 1, positionY, piercing, texture_file, name);
		break;
	case LEFT:
		representation = new Other_element(positionX - 1, positionY, piercing, texture_file, name);
		break;
	}
}

int Wind::try_make_effect(Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (map[representation->get_X()][representation->get_Y()] != nullptr)
	{
		if (typeid(Teleport) == typeid(*map[representation->get_X()][representation->get_Y()]) || typeid(Container) == typeid(*map[representation->get_X()][representation->get_Y()]) || typeid(Other_element) == typeid(*map[representation->get_X()][representation->get_Y()]))
		{
			if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
				return 0;
		}
		else
		{
			dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, location, quest_line);
			if (representation->get_ghosted() == false)
				return 0;
		}
	}
	if (countdown % speed != 0)
	{
		countdown++;
		return 1;
	}
	countdown++;
	if (duration <= 0)
		return 0;
	else
		duration--;
	return 2;
}

bool Wind::try_changing_position(Object***& map, Location*& location, std::vector <Quest_line*>& quest_line)
{
	switch (direction)
	{
	case UP:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X(), representation->get_Y() - 1);
		return true;
	case DOWN:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X(), representation->get_Y() + 1);
		return true;
	case RIGHT:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X() + 1, representation->get_Y());
		return true;
	case LEFT:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X() - 1, representation->get_Y());
		return true;
	}
}

bool Wind::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line)
{
	if (cast_time > 0)
	{
		cast_time--;
		return true;
	}
	if (representation->get_Y() <= 0 || representation->get_Y() >= size_y || representation->get_X() >= size_x || representation->get_X() <= 0)
		return false;
	try_changing_position(map, location, quest_line);
}

Wind::~Wind()
{
	delete representation;
}

Self::Self(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, bool over_time)
	: Action(name, damage, cost, cast_time, duration, cooldown, speed, player)
{
	representation = new Other_element(positionX, positionY - 1, piercing, texture_file, name);
	check = false;
	this->over_time = over_time;
}

bool Self::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line)
{
	if (cast_time > 0)
	{
		cast_time--;
		return true;
	}
	representation->change_position(player->get_X(), player->get_Y() - 1);
	if (countdown % speed != 0)
	{
		countdown++;
		return true;
	}
	countdown++;
	if (duration <= 0)
		return false;
	else
		duration--;
	if (!over_time)
	{
		if (!check)
		{
			dynamic_cast<Character*>(player)->change_hp(damage);
			check = true;
		}
	}
	else
	{
		if (!check)
			check = true;
		dynamic_cast<Character*>(player)->change_hp(damage);
	}
	return true;
}

Self::~Self()
{
	delete representation;
}

Breath::Breath(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, DIRECTION direction)
	: Action(name, damage, cost, cast_time, duration, cooldown, speed, player)
{
	this->direction = direction;
	check = false;
	switch (direction)
	{
	case UP:
		representation = new Other_element(positionX, positionY - 1, piercing, texture_file + "-1-1.png", name);
		break;
	case DOWN:
		representation = new Other_element(positionX, positionY + 1, piercing, texture_file + "-1-2.png", name);
		break;
	case RIGHT:
		representation = new Other_element(positionX + 1, positionY, piercing, texture_file + "-1-3.png", name);
		break;
	case LEFT:
		representation = new Other_element(positionX - 1, positionY, piercing, texture_file + "-1-4.png", name);
		break;
	}
}

void Breath::split(int positionX, int positionY)
{
	check = true;
	duration = 1;
	speed = 180;
	countdown = 0;
	switch (direction)
	{
	case UP:
		center_x = positionX;
		center_y = positionY;
		representation->change_position(positionX - 2, positionY - 2);
		representation->change_texture("player/player_action4-2-1.png");
		break;
	case DOWN:
		center_x = positionX;
		center_y = positionY;
		representation->change_position(positionX - 2, positionY);
		representation->change_texture("player/player_action4-2-2.png");
		break;
	case RIGHT:
		center_x = positionX;
		center_y = positionY;
		representation->change_position(positionX, positionY - 2);
		representation->change_texture("player/player_action4-2-3.png");
		break;
	case LEFT:
		center_x = positionX;
		center_y = positionY;
		representation->change_position(positionX - 2, positionY - 2);
		representation->change_texture("player/player_action4-2-4.png");
		break;
	}
}

int Breath::try_make_effect(Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (map[representation->get_X()][representation->get_Y()] != nullptr)
	{
		if (typeid(Teleport) == typeid(*map[representation->get_X()][representation->get_Y()]) || typeid(Container) == typeid(*map[representation->get_X()][representation->get_Y()]) || typeid(Other_element) == typeid(*map[representation->get_X()][representation->get_Y()]))
		{
			if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
				return 0;
		}
		else
		{
			dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, location, quest_line);
			if (representation->get_ghosted() == false)
			{
				split(representation->get_X(), representation->get_Y());
				return 1;
			}
		}
	}
	if (countdown % speed != 0)
	{
		countdown++;
		return 1;
	}
	countdown++;
	if (duration <= 0)
	{
		if (!check)
		{
			split(representation->get_X(), representation->get_Y());
			return 1;
		}
		else
			return 0;
	}
	else
		duration--;
	return 2;
}

bool Breath::try_changing_position(Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	switch (direction)
	{
	case UP:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X(), representation->get_Y() - 1);
		return true;
	case DOWN:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X(), representation->get_Y() + 1);
		return true;
	case RIGHT:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X() + 1, representation->get_Y());
		return true;
	case LEFT:
		switch (try_make_effect(map, location, quest_line))
		{
		case 0: return false;
		case 1: return true;
		default: break;
		}
		representation->change_position(representation->get_X() - 1, representation->get_Y());
		return true;
	}
}

void Breath::try_make_effect_after_split_up(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (center_y - 2 > 0)
		for (int i = center_x - 2; i <= center_x + 2; i++)
			if (i > 0 && i < size_x)
				if (map[i][center_y - 2] != nullptr)
					if (typeid(Teleport) != typeid(*map[i][center_y - 2]) && typeid(Container) != typeid(*map[i][center_y - 2]) && typeid(Other_element) != typeid(*map[i][center_y - 2]))
					{
						
						dynamic_cast<Character*>(map[i][center_y - 2])->get_damage(damage, map, location, quest_line);
					}
	if (center_y - 1 > 0)
		for (int i = center_x - 1; i <= center_x + 1; i++)
			if (i > 0 && i < size_x)
				if (map[i][center_y - 1] != nullptr)
					if (typeid(Teleport) != typeid(*map[i][center_y - 1]) && typeid(Container) != typeid(*map[i][center_y - 1]) && typeid(Other_element) != typeid(*map[i][center_y - 1]))
						dynamic_cast<Character*>(map[i][center_y - 1])->get_damage(damage, map, location, quest_line);
	if (center_y > 0)
		if (center_x > 0 && center_x < size_x)
			if (map[center_x][center_y] != nullptr)
				if (typeid(Teleport) != typeid(*map[center_x][center_y]) && typeid(Container) != typeid(*map[center_x][center_y]) && typeid(Other_element) != typeid(*map[center_x][center_y]))
					dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, location, quest_line);
}

void Breath::try_make_effect_after_split_down(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (center_y + 2 < size_y)
		for (int i = center_x - 2; i <= center_x + 2; i++)
			if (i > 0 && i < size_x)
				if (map[i][center_y + 2] != nullptr)
					if (typeid(Teleport) != typeid(*map[i][center_y + 2]) && typeid(Container) != typeid(*map[i][center_y + 2]) && typeid(Other_element) != typeid(*map[i][center_y + 2]))
						dynamic_cast<Character*>(map[i][center_y + 2])->get_damage(damage, map, location, quest_line);
	if (center_y + 1 < size_y)
		for (int i = center_x - 1; i <= center_x + 1; i++)
			if (i > 0 && i < size_x)
				if (map[i][center_y + 1] != nullptr)
					if (typeid(Teleport) != typeid(*map[i][center_y + 1]) && typeid(Container) != typeid(*map[i][center_y + 1]) && typeid(Other_element) != typeid(*map[i][center_y + 1]))
						dynamic_cast<Character*>(map[i][center_y + 1])->get_damage(damage, map, location, quest_line);
	if (center_y < size_y)
		if (center_x > 0 && center_x < size_x)
			if (map[center_x][center_y] != nullptr)
				if (typeid(Teleport) != typeid(*map[center_x][center_y]) && typeid(Container) != typeid(*map[center_x][center_y]) && typeid(Other_element) != typeid(*map[center_x][center_y]))
					dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, location, quest_line);
}

void Breath::try_make_effect_after_split_right(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (center_x + 2 < size_x)
		for (int i = center_y - 2; i <= center_y + 2; i++)
			if (i > 0 && i < size_y)
				if (map[center_x + 2][i] != nullptr)
					if (typeid(Teleport) != typeid(*map[center_x + 2][i]) && typeid(Container) != typeid(*map[center_x + 2][i]) && typeid(Other_element) != typeid(*map[center_x + 2][i]))
						dynamic_cast<Character*>(map[center_x + 2][i])->get_damage(damage, map, location, quest_line);
	if (center_x + 1 < size_x)
		for (int i = center_y - 1; i <= center_y + 1; i++)
			if (i > 0 && i < size_y)
				if (map[center_x + 1][i] != nullptr)
					if (typeid(Teleport) != typeid(*map[center_x + 1][i]) && typeid(Container) != typeid(*map[center_x + 1][i]) && typeid(Other_element) != typeid(*map[center_x + 1][i]))
						dynamic_cast<Character*>(map[center_x + 1][i])->get_damage(damage, map, location, quest_line);
	if (center_x < size_x)
		if (center_y > 0 && center_y < size_y)
			if (map[center_x][center_y] != nullptr)
				if (typeid(Teleport) != typeid(*map[center_x][center_y]) && typeid(Container) != typeid(*map[center_x][center_y]) && typeid(Other_element) != typeid(*map[center_x][center_y]))
					dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, location, quest_line);
}

void Breath::try_make_effect_after_split_left(int size_x, int size_y, Object***& map, Location*& location, std::vector<Quest_line*>& quest_line)
{
	if (center_x - 2 > 0)
		for (int i = center_y - 2; i <= center_y + 2; i++)
			if (i > 0 && i < size_y)
				if (map[center_x - 2][i] != nullptr)
					if (typeid(Teleport) != typeid(*map[center_x - 2][i]) && typeid(Container) != typeid(*map[center_x - 2][i]) && typeid(Other_element) != typeid(*map[center_x - 2][i]))
						dynamic_cast<Character*>(map[center_x - 2][i])->get_damage(damage, map, location, quest_line);
	if (center_x - 1 > 0)
		for (int i = center_y - 1; i <= center_y + 1; i++)
			if (i > 0 && i < size_y)
				if (map[center_x - 1][i] != nullptr)
					if (typeid(Teleport) != typeid(*map[center_x - 1][i]) && typeid(Container) != typeid(*map[center_x - 1][i]) && typeid(Other_element) != typeid(*map[center_x - 1][i]))
						dynamic_cast<Character*>(map[center_x - 1][i])->get_damage(damage, map, location, quest_line);
	if (center_x > 0)
		if (center_y > 0 && center_y < size_y)
			if (map[center_x][center_y] != nullptr)
				if (typeid(Teleport) != typeid(*map[center_x][center_y]) && typeid(Container) != typeid(*map[center_x][center_y]) && typeid(Other_element) != typeid(*map[center_x][center_y]))
					dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, location, quest_line);
}

bool Breath::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, Object* player, Location* &location, std::vector <Quest_line*>& quest_line)
{
	if (cast_time > 0)
	{
		cast_time--;
		return true;
	}
	if (!check)
	{
		if (try_changing_position(map, location, quest_line))
			return true;
		else return false;
	}
	else
	{
		if (countdown % speed != 0)
		{
			countdown++;
			return true;
		}
		countdown++;
		if (duration <= 0)
			return false;
		else
			duration--;
		switch (direction)
		{
		case UP:
			try_make_effect_after_split_up(size_x, size_y, map, location, quest_line);
			return true;
		case DOWN:
			try_make_effect_after_split_down(size_x, size_y, map, location, quest_line);
			return true;
		case RIGHT:
			try_make_effect_after_split_right(size_x, size_y, map, location, quest_line);
			return true;
		case LEFT:
			try_make_effect_after_split_left(size_x, size_y, map, location, quest_line);
			return true;
		}
	}
}

Breath::~Breath()
{
	delete representation;
}