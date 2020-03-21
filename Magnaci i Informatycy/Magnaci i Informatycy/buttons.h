#pragma once
#include <iostream>
#include <allegro5/bitmap.h>
#include <allegro5/allegro.h>
#include <string>

#include "general_functions.h"

struct cont_list;

enum BUTTON_FUNCTION
{
	START, PAUSE, EXIT, RESUME, SETTINGS, LOAD, MENU, AUDIO, GRAPHIC, MOVE, SCREEN_MODE
};

enum FADER_FUNCTION
{
	MAIN, SOUNDTRACK, EFECTS, TALK
};

class Button
{
protected:
	int position_x, position_y, size_x, size_y;
	ALLEGRO_BITMAP* visible_bitmap, * selected, * unselected;
public:
	const bool is_fader;
	bool is_selected;
	Button(int pposition_x, int pposition_y, int psize_x, int psize_y, bool pis_fader);
	virtual int click(int* mapping_tab) = 0;
	void select();
	void unselect();
	void draw();
	virtual bool check_position(int mouse_position_x, int mouse_position_y) = 0;
};


class Functional_Button : public Button
{
protected:
	BUTTON_FUNCTION function;
public:
	Functional_Button(int pposition_x, int pposition_y, int psize_x, int psize_y, BUTTON_FUNCTION pfunction);
	int click(int* mapping_tab);
	bool check_position(int mouse_position_x, int mouse_position_y);
};

class Mapping_Button : public Button
{
	int button_number;
public:
	Mapping_Button(int pposition_x, int pposition_y, int psize_x, int psize_y, int pbutton_number);
	int click(int* mapping_tab);
	bool check_position(int mouse_position_x, int mouse_position_y);
};


class Fader : public Button
{
	int pointer_position;
	int back_size_x, back_size_y;
	ALLEGRO_BITMAP* fader;
	FADER_FUNCTION function;
public:
	Fader(int pposition_x, int pposition_y, int psize_x, int psize_y, int back_size_x, int back_size_y, FADER_FUNCTION pfunction, int* mapping_tab);
	void fader_draw();
	int click(int* mapping_tab);
	bool check_position(int mouse_position_x, int mouse_position_y);
};

class ComboBox : public Button
{
	cont_list* content;
public:
	ComboBox(int pposition_x, int pposition_y, int psize_x, int psize_y);
	int click(int* mapping_tab);
	bool check_position(int mouse_position_x, int mouse_position_y);
};

struct cont_list
{
	Button* element = nullptr;
	cont_list* next = nullptr;
	cont_list* prev = nullptr;
};

cont_list* add_cont(Button* el, cont_list* buttons);
cont_list* remove_cont(Button* el, cont_list* buttons);