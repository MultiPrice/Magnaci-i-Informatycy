#pragma once
#include <vector>
#include <iostream>
#include <allegro5/bitmap.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>


const int screen_width = 1920;
const int screen_height = 1060;
const int measure = 60; //rozmiar pojedynczej kratki/najmniejszej dlugosci miary
const int shiftX = screen_width / measure * 0.5;
const int shiftY = screen_height/ measure * 0.5;
const int FPS = 60;


class Object
{
protected:
	int positionX;
	int positionY;
	ALLEGRO_BITMAP* texture;
public:
	//````````````````````````````````````````````````````
	int bitmap_start_x = 0;
	int bitmap_start_y = 0;
	//````````````````````````````````````````````````````
	virtual void draw(Object ***map, int max_x, int max_y) = 0;
	virtual void draw(int position_x, int position_y) = 0;
	int get_X();
	int get_Y();
	void change_position(int new_X, int new_Y);
	void change_texture(std::string new_texture_file);
	ALLEGRO_BITMAP* get_texture();
};

bool al_addon();
void intro();