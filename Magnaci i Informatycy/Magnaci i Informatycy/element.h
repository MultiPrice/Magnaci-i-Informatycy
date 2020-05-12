#pragma once
#include <iostream>

#include "general_functions.h"

class Element : public Object
{
protected:
	bool ghosted;
public:
	Element(int X, int Y, bool ghost, std::string texture_name, std::string name);
	void draw(Object ***map, int max_x, int max_y);
	void draw(int position_x, int position_y);
	bool get_ghosted();
	~Element();
};

class Teleport : public Element
{
public:
	Teleport(int X, int Y, bool ghost, std::string texture_name, std::string name);
	int interaction();
	~Teleport();
};

class Container : public Element
{
public:
	Container(int X, int Y, bool ghost, std::string texture_name, std::string name);
	int interaction();
	~Container();
};

class Other_element : public Element
{
public:
	Other_element(int X, int Y, bool ghost, std::string texture_name, std::string name);
	int interaction();
	~Other_element();
};