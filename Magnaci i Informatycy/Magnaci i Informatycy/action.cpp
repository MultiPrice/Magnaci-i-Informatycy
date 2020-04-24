#include "action.h"

Action::Action(std::string name, int damage, int cost, double cast_time, double duration, double cooldown)
{
	this->name = name;
	this->damage = damage;
	this->cast_time = cast_time;
	this->duration = duration;
	this->cooldown = cooldown;
	representation = nullptr;
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

Wind::Wind(int positionX, int positionY, std::string texture_file, std::string name, int damage, int cost, double cast_time, double duration, double cooldown, DIRECTION direction, bool piercing)
	: Action(name, damage, cost, cast_time, duration, cooldown)
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

bool Wind::make_action(Object*** &map, int size_x, int size_y, int position_x, int position_y, std::vector <Object*>& mobs)
{
	if (cast_time > 0)
		return true;
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
			//representation->draw(position_x - shiftX, position_y - shiftY);
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
		//	representation->draw(position_x - shiftX, position_y - shiftY);
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
			//representation->draw(position_x - shiftX, position_y - shiftY);
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
			//representation->draw(position_x - shiftX, position_y - shiftY);
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
