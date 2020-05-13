#include "dialogue.h"

Question::Question(std::string question, int id)
{
	this->question = question;
	this->id = id;
}

void Question::add_answer(std::string answer, int next_qt_id)
{
	all_answers.push_back(new Answer{ answer, next_qt_id });
}
