#pragma once

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

#include "general_functions.h"


enum TYPE
{
	kill,
	speak_to,
	go_to,
	interct
};

class Quest
{
protected:
	std::string name;
	std::vector <Object*, TYPE> objective;
public:

};

struct Quest_list
{
	Quest* quest;
	Quest_list* pNext;
};