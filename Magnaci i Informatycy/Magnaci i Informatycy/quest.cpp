#include "quest.h"

Objective::Objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
{
	this->target_name = target_name;
	this->target_location_name = target_location_name;
	this->to_do = to_do;
	this->how_many = how_many;
}

bool Objective::check_objective(Location* location)
{
	if (location->get_name() == target_location_name)
		return true;
	else
		return false;
}

std::string Objective::get_target_name()
{
	return this->target_name;
}

bool Objective::is_it_done()
{
	how_many--;
	if (how_many == 0)
		return true;
	return false;
}

TYPE Objective::get_to_do()
{
	return to_do;
}

Character_objective::Character_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_name, target_location_name, to_do, how_many)
{
}

//void Character_objective::check_objective(Location* location)
//{
//	if (location->get_name() == target_location_name)
//		check = true;
//	else
//		check = false;
//}

Element_objective::Element_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_name, target_location_name, to_do, how_many)
{
}

//void Element_objective::check_objective(Location* location)
//{
//	if (location->get_name() == target_location_name)
//		check = true;
//	else
//		check = false;
//}

Location_objective::Location_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many)
	: Objective(target_name, target_location_name, to_do, how_many)
{
}

//void Location_objective::check_objective(Location* location)
//{
//	if (location->get_name() == target_location_name)
//		check = true;
//}

Quest::Quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	this->name = name;
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 1: // Element
		objective.push_back(new Element_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 2: // Location
		objective.push_back(new Location_objective(target_name, target_location_name, to_do, how_many));
		break;
	}
}

std::string Quest::get_name()
{
	return name;
}

std::vector<Objective*> Quest::get_objectives()
{
	return objective;
}

void Quest::add_objective(std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 1: // Element
		objective.push_back(new Element_objective(target_name, target_location_name, to_do, how_many));
		break;
	case 2: // Location
		objective.push_back(new Location_objective(target_name, target_location_name, to_do, how_many));
		break;
	}
}

Quest::~Quest()
{
	objective.clear();
}

Quest_line::Quest_line(std::string quest_line_name, std::string start_quest_name)
{
	this->name = quest_line_name;
	quest = nullptr;
	next_quest_name = "";
	quest_file_read(quest_line_name, start_quest_name);
	quest_bitmap = al_load_bitmap("bitmaps/items/quest_background.png");
	background = al_load_bitmap("bitmaps/items/inventory_background.png");
	player = al_load_bitmap("player/player_move.png");
}

void Quest_line::quest_file_read(std::string quest_line_name, std::string quest_name)
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

void Quest_line::add_quest(std::string name, std::string target_name, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	if (!quest)
		quest = new Quest(name, target_name, target_location_name, to_do, how_many, what_class);
	else if(quest->get_name() == name)
		quest->add_objective(target_name, target_location_name, to_do, how_many, what_class);
	else
	{
		//quest = nullptr;
		quest = new Quest(name, target_name, target_location_name, to_do, how_many, what_class);
	}
}

void Quest_line::show_quests()
{
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_bitmap(quest_bitmap, screen_width / 2 + measure, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(player, 0, measure * 6, measure * 1.5, measure * 2, al_map_rgb(255, 255, 255), 0, 0, 150, 60, 8, 8, 0, 0);
	for (int i = 0; i <quest->get_objectives().size(); i++) // wypisywanie questow
	{
		//std::cout << "dupa \n";
	}
}

bool Quest_line::take_next_quest()
{
	quest->~Quest();
	if (next_quest_name == "")
	{
		std::cout << "Koniec lini questow" << std::endl;
		return false;
	}
	quest_file_read(name, next_quest_name);
	std::cout << "Rozpoczales nowego qt" << std::endl;
	/*std::cout << quest->get_name() << std::endl;
	std::cout << quest->objective[0]->get_target_name() << std::endl;*/
	return true;
}

Quest* Quest_line::get_quest()
{
	return quest;
}

Quest_line::~Quest_line()
{
	al_destroy_bitmap(quest_bitmap);
	al_destroy_bitmap(background);
	al_destroy_bitmap(player);
	quest->~Quest();
}
