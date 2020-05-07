#include "quest.h"

Objective::Objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
{
	this->target_name = target_name;
	this->target_location_name = target_location_name;
	this->to_do = to_do;
	this->how_many = how_many;
	check = false;
}

Character_objective::Character_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_name, target_location_name, to_do, how_many)
{
}

void Character_objective::check_objective(Location* location)
{
	if (location->get_name() == target_location_name)
		for (int i = 0; i < location->mobs.size(); i++)
			if (target_name == location->mobs[i]->get_name())
				check = true;
}

Element_objective::Element_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_name, target_location_name, to_do, how_many)
{
}

void Element_objective::check_objective(Location* location)
{
	if (location->get_name() == target_location_name)
		for (int i = 0; i < location->elements.size(); i++)
			if (target_name == location->elements[i]->get_name())
				check = true;
}

Location_objective::Location_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	: Objective(target_name, target_location_name, to_do, how_many)
{
}

void Location_objective::check_objective(Location* location)
{
	if (location->get_name() == target_location_name)
		check = true;
}

Quest::Quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	this->name = name;
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 1: // Element
		
		break;
	case 2: // Location
		
		break;
	}
}

std::string Quest::get_name()
{
	return name;
}

void Quest::add_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 1: // Element

		break;
	case 2: // Location

		break;
	}
}

Quest_list::Quest_list(std::string quest_line_name, std::string start_quest_name)
{
	this->name = quest_line_name;
	quest = nullptr;
	next_quest_name = "";
	quest_file_read(quest_line_name, start_quest_name);
}

void Quest_list::quest_file_read(std::string quest_line_name, std::string quest_name)
{
	std::string file_name = "Quest/" + quest_line_name + "/" + quest_name + ".txt";
	std::fstream file;
	file.open(file_name);
	if (file)
	{
		std::string trash;
		std::getline(file, trash);
		std::getline(file, trash);
		std::string object_name, class_type, enum_type, how_many, location_name;
		while (file)
		{
			std::getline(file, object_name);
			std::getline(file, class_type);
			std::getline(file, location_name);
			std::getline(file, enum_type);
			std::getline(file, how_many);
			std::getline(file, trash);
			switch (stoi(class_type))
			{
			case 0: // Character 
				add_quest(quest_name, object_name, location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 0);
				break;
			case 1: // Element
				add_quest(quest_name, object_name, location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 1);
				break;
			case 2: // Location
				add_quest(quest_name, object_name, location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 2);
				break;
			}
			if (trash == "}")
				break;
		}
		std::getline(file, trash);
		next_quest_name = trash;
	}
}

void Quest_list::add_quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	if (!quest)
		quest = new Quest(name, target_name, target_location_name, to_do, how_many, what_class);
	else
		quest->add_objective(target_name, target_location_name, to_do, how_many, what_class);
}

void Quest_list::take_next_quest()
{
	delete quest;
	if (next_quest_name == "")
		return;
	quest_file_read(name, next_quest_name);
}