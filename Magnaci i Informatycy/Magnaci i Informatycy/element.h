#pragma once
#include <iostream>

#include "general_functions.h"

class Element : public Object
{
protected:
	bool ghosted;
	bool teleport;
public:
	Element(int X, int Y, bool ghost, bool teleport, std::string texture_name, std::string name);
	void draw(Object ***map, int max_x, int max_y);
	void draw(int position_x, int position_y);
	bool get_ghosted();
	bool get_teleport();
	~Element();
};