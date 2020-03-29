#include <fstream>

#include "element.h"

Element::Element(int X, int Y, bool ghost)
{
	positionX = X;
	positionY = Y;
	ghosted = ghost;
	texture = nullptr;
	/*
		std::fstream file;
		file.open(file_name);
		int seek_id;
		std::string trash;
		if (file)
		{
			file >> seek_id;
			while (seek_id != id);
			{
				for (int i = 0; i < 4; i++)
					file >> trash;
				file >> seek_id;
			}
			file >> trash;
			std::string bitmap_file;
			file >> bitmap_file;
			texture = al_load_bitmap(bitmap_file.c_str());
			file >> ghosted;
			file.close();
		}*/
}

void Element::draw()
{
}

void Element::draw(int position_x, int position_y)
{
}

bool Element::czy_ghosted()
{
	return ghosted;
}
