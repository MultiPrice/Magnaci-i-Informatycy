#pragma once
#include <string>
#include <fstream>
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

struct Answer
{
	std::string answer;
	int next_question_id;
};

class Question
{
public:
	int id;
	std::string question;
	std::vector <Answer*> all_answers;
	Question(std::string question, int id);
	void add_answer(std::string answer, int next_qt_id);
};