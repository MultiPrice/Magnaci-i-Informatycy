#pragma once
#include <iostream>

#include "general_functions.h"

class Element : public Object
{
protected:
	bool ghosted;
	bool teleport;
public:
	Element(int X, int Y, bool ghost, bool teleport);
	void draw(Object ***map);
	void draw(int position_x, int position_y);
	bool get_ghosted();
	bool get_teleport();
	~Element();
};