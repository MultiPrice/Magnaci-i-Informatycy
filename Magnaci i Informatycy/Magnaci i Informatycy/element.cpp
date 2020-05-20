#include <fstream>

#include "element.h"

Element::Element(int X, int Y, bool ghost, std::string texture_name, std::string name)
{
	positionX = X;
	positionY = Y;
	this->ghosted = ghost;
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

Element::~Element()
{
	//al_destroy_bitmap(texture);
	texture = nullptr;
}

Teleport::Teleport(int X, int Y, bool ghost, std::string texture_name, std::string name)
	:Element(X, Y, ghost, texture_name, name)
{

}

int Teleport::interaction()
{
	return 1;
}

Teleport::~Teleport()
{
	texture = nullptr;
}

Container::Container(int X, int Y, bool ghost, std::string texture_name, std::string name)
	: Element(X, Y, ghost, texture_name, name)
{

}

int Container::interaction()
{
	return 2;
}

Container::~Container()
{
	al_destroy_bitmap(texture);
}

Other_element::Other_element(int X, int Y, bool ghost, std::string texture_name, std::string name)
	: Element(X, Y, ghost, texture_name, name)
{

}

int Other_element::interaction()
{
	return 0;
}

Other_element::~Other_element()
{
	texture = nullptr;
}