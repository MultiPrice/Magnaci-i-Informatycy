#pragma once
#include <iostream>

#include "general_functions.h"

class Element : public Object
{
protected:
	bool ghosted;
public:
	Element(int X, int Y, bool ghost);
	void draw();
	void draw(int position_x, int position_y);
	bool czy_ghosted();
};