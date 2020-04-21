#include <fstream>

#include "element.h"

Element::Element(int X, int Y, bool ghost, bool teleport)
{
	positionX = X;
	positionY = Y;
	this->ghosted = ghost;
	this->teleport = teleport;
	texture = al_load_bitmap("Element/portal.png");
}

void Element::draw(Object ***map)
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
