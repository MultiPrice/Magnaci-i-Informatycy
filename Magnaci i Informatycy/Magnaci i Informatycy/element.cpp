#include <fstream>

#include "element.h"

Element::Element(int X, int Y, bool ghost, bool teleport, std::string texture_name, std::string name)
{
	positionX = X;
	positionY = Y;
	this->ghosted = ghost;
	this->teleport = teleport;
	this->name = name;
	texture = al_load_bitmap(texture_name.c_str());
}

void Element::draw(Object ***map, int max_x, int max_y)
{
}

void Element::draw(int position_x, int position_y)
{
	al_draw_bitmap(texture, (positionX - position_x) * measure, (positionY - position_y) * measure, 0);
}

bool Element::get_ghosted()
{
	return ghosted;
}

bool Element::get_teleport()
{
	return teleport;
}

Element::~Element()
{
	//al_destroy_bitmap(texture);
	texture = nullptr;
}
