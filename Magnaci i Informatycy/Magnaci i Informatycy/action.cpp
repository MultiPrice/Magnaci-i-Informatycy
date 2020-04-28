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
		representation = new Element(positionX + effect_dash_x, positionY - 1 + effect_dash_y, piercing, false, texture_file);
		break;
	case DOWN:
		center_x = positionX;
		center_y = positionY + 1;
		representation = new Element(positionX + effect_dash_x, positionY + 1 + effect_dash_y, piercing, false, texture_file);
		break;
	case RIGHT:
		center_x = positionX + 1;
		center_y = positionY;
		representation = new Element(positionX + effect_dash_x + 1, positionY + effect_dash_y, piercing, false, texture_file);
		break;
	case LEFT:
		center_x = positionX - 1;
		center_y = positionY;
		representation = new Element(positionX + effect_dash_x - 1, positionY + effect_dash_y, piercing, false, texture_file);
		break;
	}
}

bool AoE::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, std::vector<Object*>& mobs, Object* player)
{
	if (cast_time > 0)
		return true;
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
	
	for (int i = center_x - 1; i <= center_x + 1; i++)
	{
		if (i > 0 && i < size_x)
		{
			if (center_y - 3 > 0)
				if (map[i][center_y - 3] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y - 3]))
						dynamic_cast<Character*>(map[i][center_y - 3])->get_damage(damage, map, mobs);
			if (center_y + 3 < size_y)
					if (map[i][center_y + 3] != nullptr)
						if (typeid(Element) != typeid(*map[i][center_y + 3]))
							dynamic_cast<Character*>(map[i][center_y + 3])->get_damage(damage, map, mobs);
		}
	}
	for (int i = center_x - 3; i <= center_x + 3; i++)
	{
		if (i > 0 && i < size_x)
		{
			if (center_y - 1 > 0)
				if (map[i][center_y - 1] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y - 1]))
						dynamic_cast<Character*>(map[i][center_y - 1])->get_damage(damage, map, mobs);
			if (center_y + 1 < size_y)
				if (map[i][center_y + 1] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y + 1]))
						dynamic_cast<Character*>(map[i][center_y + 1])->get_damage(damage, map, mobs);
				if (map[i][center_y] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y ]))
						dynamic_cast<Character*>(map[i][center_y ])->get_damage(damage, map, mobs);
		}
	}
	for (int i = center_x - 2; i <= center_x + 2; i++)
	{
		if (i > 0 && i < size_x)
		{
			if (center_y - 2 > 0)
				if (map[i][center_y - 2] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y - 2]))
						dynamic_cast<Character*>(map[i][center_y - 2])->get_damage(damage, map, mobs);
			if (center_y + 2 < size_y)
				if (map[i][center_y + 2] != nullptr)
					if (typeid(Element) != typeid(*map[i][center_y + 2]))
						dynamic_cast<Character*>(map[i][center_y + 2])->get_damage(damage, map, mobs);
		}
	}
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
		representation = new Element(positionX, positionY - 1, piercing, false, texture_file);
		break;
	case DOWN:
		representation = new Element(positionX, positionY + 1, piercing, false, texture_file);
		break;
	case RIGHT:
		representation = new Element(positionX + 1, positionY, piercing, false, texture_file);
		break;
	case LEFT:
		representation = new Element(positionX - 1, positionY, piercing, false, texture_file);
		break;
	}
}

bool Wind::make_action(Object*** &map, int size_x, int size_y, int position_x, int position_y, std::vector <Object*>& mobs, Object *player)
{
	if (cast_time > 0)
		return true;
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
		if (representation->get_Y() > 0)
		{
			if (map[representation->get_X()][representation->get_Y()] != nullptr)
			{
				if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
				{
					if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
						return false;
				}
				else
				{
					dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
					if (representation->get_ghosted() == false)
						return false;
				}
			}
			representation->change_position(representation->get_X(), representation->get_Y() - 1);
		}
		else
			return false;
		return true;
	case DOWN:
		if (representation->get_Y() < size_y)
		{
			if (map[representation->get_X()][representation->get_Y()] != nullptr)
			{
				if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
				{
					if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
						return false;
				}
				else
				{
					dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
					if (representation->get_ghosted() == false)
						return false;
				}
			}
			representation->change_position(representation->get_X(), representation->get_Y() + 1);
		}
		else
			return false;
		return true;
	case RIGHT:
		if (representation->get_X() < size_x)
		{
			if (map[representation->get_X()][representation->get_Y()] != nullptr)
			{
				if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
				{
					if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
						return false;
				}
				else
				{
					dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
					if (representation->get_ghosted() == false)
						return false;
				}
			}
			representation->change_position(representation->get_X() + 1, representation->get_Y());
		}
		else
			return false;
		return true;
	case LEFT:
		if (representation->get_X() > 0)
		{
			if (map[representation->get_X()][representation->get_Y()] != nullptr)
			{
				if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
				{
					if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
						return false;
				}
				else
				{
					dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
					if (representation->get_ghosted() == false)
						return false;
				}
			}
			representation->change_position(representation->get_X() - 1, representation->get_Y());
		}
		else
			return false;
		return true;
	}
}

Wind::~Wind()
{
	delete representation;
}

Self::Self(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, bool piercing, int speed, Object*& player, bool over_time)
	: Action(name, damage, cost, cast_time, duration, cooldown, speed, player)
{
	representation = new Element(positionX, positionY - 1, piercing, false, texture_file);
	check = false;
	this->over_time = over_time;
}

bool Self::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, std::vector<Object*>& mobs, Object *player)
{
	if (cast_time > 0)
		return true;
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
		representation = new Element(positionX, positionY - 1, piercing, false, texture_file + "-1-1.png");
		break;
	case DOWN:
		representation = new Element(positionX, positionY + 1, piercing, false, texture_file + "-1-2.png");
		break;
	case RIGHT:
		representation = new Element(positionX + 1, positionY, piercing, false, texture_file + "-1-3.png");
		break;
	case LEFT:
		representation = new Element(positionX - 1, positionY, piercing, false, texture_file + "-1-4.png");
		break;
	}
}

void Breath::split(int positionX, int positionY) // NARAZIE NA SZTYWNO
{
	check = true;
	duration = 1;
	speed = 80;
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

bool Breath::make_action(Object***& map, int size_x, int size_y, int position_x, int position_y, std::vector<Object*>& mobs, Object* player)
{
	if (cast_time > 0)
		return true;
	if (countdown % speed != 0)
	{
		countdown++;
		return true;
	}
	countdown++;
	if (duration <= 0)
	{
		if (!check)
			split(representation->get_X(), representation->get_Y());
		else 
			return false;
	}
	else
		duration--;
	if (!check)
	{
		switch (direction)
		{
		case UP:
			if (representation->get_Y() > 0)
			{
				if (map[representation->get_X()][representation->get_Y()] != nullptr)
				{
					if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
					{
						if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
							return false;
					}
					else
					{
						dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
						if (representation->get_ghosted() == false)
						{
							split(representation->get_X(), representation->get_Y());
							return true;
						}
					}
				}
				representation->change_position(representation->get_X(), representation->get_Y() - 1);
			}
			else
				return false;
			return true;
		case DOWN:
			if (representation->get_Y() < size_y)
			{
				if (map[representation->get_X()][representation->get_Y()] != nullptr)
				{
					if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
					{
						if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
							return false;
					}
					else
					{
						dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
						if (representation->get_ghosted() == false)
						{
							split(representation->get_X(), representation->get_Y());
							return true;
						}
					}
				}
				representation->change_position(representation->get_X(), representation->get_Y() + 1);
			}
			else
				return false;
			return true;
		case RIGHT:
			if (representation->get_X() < size_x)
			{
				if (map[representation->get_X()][representation->get_Y()] != nullptr)
				{
					if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
					{
						if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
							return false;
					}
					else
					{
						dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
						if (representation->get_ghosted() == false)
						{
							split(representation->get_X(), representation->get_Y());
							return true;
						}
					}
				}
				representation->change_position(representation->get_X() + 1, representation->get_Y());
			}
			else
				return false;
			return true;
		case LEFT:
			if (representation->get_X() > 0)
			{
				if (map[representation->get_X()][representation->get_Y()] != nullptr)
				{
					if (typeid(Element) == typeid(*map[representation->get_X()][representation->get_Y()]))
					{
						if (dynamic_cast<Element*>(map[representation->get_X()][representation->get_Y()])->get_ghosted() == false)
							return false;
					}
					else
					{
						dynamic_cast<Character*>(map[representation->get_X()][representation->get_Y()])->get_damage(damage, map, mobs);
						if (representation->get_ghosted() == false)
						{
							split(representation->get_X(), representation->get_Y());
							return true;
						}
					}
				}
				representation->change_position(representation->get_X() - 1, representation->get_Y());
			}
			else
				return false;
			return true;
		}
	}
	else
	{
		switch (direction)
		{
		case UP:
			if (center_y - 2 > 0)
				for (int i = center_x - 2; i <= center_x + 2; i++)
					if(i > 0 && i < size_x)
						if (map[i][center_y - 2] != nullptr)
							if (typeid(Element) != typeid(*map[i][center_y - 2]))
								dynamic_cast<Character*>(map[i][center_y - 2])->get_damage(damage, map, mobs);
			if (center_y - 1 > 0)
				for (int i = center_x - 1; i <= center_x + 1; i++)
					if (i > 0 && i < size_x)
						if (map[i][center_y - 1] != nullptr)
							if (typeid(Element) != typeid(*map[i][center_y - 1]))
								dynamic_cast<Character*>(map[i][center_y - 1])->get_damage(damage, map, mobs);
			if (center_y > 0)
				if (center_x > 0 && center_x < size_x)
					if (map[center_x][center_y] != nullptr)
						if (typeid(Element) != typeid(*map[center_x][center_y]))
							dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, mobs);
			return true;
		case DOWN:
			if (center_y + 2 < size_y)
				for (int i = center_x - 2; i <= center_x + 2; i++)
					if (i > 0 && i < size_x)
						if (map[i][center_y + 2] != nullptr)
							if (typeid(Element) != typeid(*map[i][center_y + 2]))
								dynamic_cast<Character*>(map[i][center_y + 2])->get_damage(damage, map, mobs);
			if (center_y + 1 < size_y)
				for (int i = center_x - 1; i <= center_x + 1; i++)
					if (i > 0 && i < size_x)
						if (map[i][center_y + 1] != nullptr)
							if (typeid(Element) != typeid(*map[i][center_y + 1]))
								dynamic_cast<Character*>(map[i][center_y + 1])->get_damage(damage, map, mobs);
			if (center_y < size_y)
				if (center_x > 0 && center_x < size_x)
					if (map[center_x][center_y] != nullptr)
						if (typeid(Element) != typeid(*map[center_x][center_y]))
							dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, mobs);
			return true;
		case RIGHT:
			if (center_x + 2 < size_x)
				for (int i = center_y - 2; i <= center_y + 2; i++)
					if (i > 0 && i < size_y)
						if (map[center_x + 2][i] != nullptr)
							if (typeid(Element) != typeid(*map[center_x + 2][i]))
								dynamic_cast<Character*>(map[center_x + 2][i])->get_damage(damage, map, mobs);
			if (center_x + 1 < size_x)
				for (int i = center_y - 1; i <= center_y + 1; i++)
					if (i > 0 && i < size_y)
						if (map[center_x + 1][i] != nullptr)
							if (typeid(Element) != typeid(*map[center_x + 1][i]))
								dynamic_cast<Character*>(map[center_x + 1][i])->get_damage(damage, map, mobs);
			if (center_x < size_x)
				if (center_y > 0 && center_y < size_y)
					if (map[center_x][center_y] != nullptr)
						if (typeid(Element) != typeid(*map[center_x][center_y]))
							dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, mobs);
			return true;
		case LEFT:
			if (center_x - 2 > 0)
				for (int i = center_y - 2; i <= center_y + 2; i++)
					if (i > 0 && i < size_y)
						if (map[center_x - 2][i] != nullptr)
							if (typeid(Element) != typeid(*map[center_x - 2][i]))
								dynamic_cast<Character*>(map[center_x - 2][i])->get_damage(damage, map, mobs);
			if (center_x - 1 > 0)
				for (int i = center_y - 1; i <= center_y + 1; i++)
					if (i > 0 && i < size_y)
						if (map[center_x - 1][i] != nullptr)
							if (typeid(Element) != typeid(*map[center_x - 1][i]))
								dynamic_cast<Character*>(map[center_x - 1][i])->get_damage(damage, map, mobs);
			if (center_x > 0)
				if (center_y > 0 && center_y < size_y)
					if (map[center_x][center_y] != nullptr)
						if (typeid(Element) != typeid(*map[center_x][center_y]))
							dynamic_cast<Character*>(map[center_x][center_y])->get_damage(damage, map, mobs);
			return true;
		}
	}
}

Breath::~Breath()
{
	delete representation;
}
