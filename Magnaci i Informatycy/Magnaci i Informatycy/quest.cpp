#include "quest.h"

Objective::Objective(int target_id, std::string target_location_name, TYPE to_do, int how_many)
{
	this->target_id = target_id;
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

int Objective::get_target_id()
{
	return this->target_id;
}

int Objective::get_how_many()
{
	return how_many;
}

std::string Objective::get_target_location()
{
	return target_location_name;
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

int Objective::get_to_do_int()
{
	return (int)to_do;
}

Character_objective::Character_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_id, target_location_name, to_do, how_many)
{
}

Element_objective::Element_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_id, target_location_name, to_do, how_many)
{
}

Location_objective::Location_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many)
	: Objective(target_id, target_location_name, to_do, how_many)
{
}

Item_objective::Item_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many)
	:Objective(target_id, target_location_name, to_do, how_many)
{
}

Quest::Quest(std::string name, int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	this->name = name;
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 1: // Element
		objective.push_back(new Element_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 2: // Location
		objective.push_back(new Location_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 3: // Item
		objective.push_back(new Item_objective(target_id, target_location_name, to_do, how_many));
		break;
	}
}

std::string Quest::get_name()
{
	return name;
}

void Quest::add_objective(int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	switch (what_class)
	{
	case 0: // Character
		objective.push_back(new Character_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 1: // Element
		objective.push_back(new Element_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 2: // Location
		objective.push_back(new Location_objective(target_id, target_location_name, to_do, how_many));
		break;
	case 3: // Item
		objective.push_back(new Item_objective(target_id, target_location_name, to_do, how_many));
		break;
	}
}

Quest::~Quest()
{
	if (objective.empty())
		return;
	for (int i = 0; i < objective.size(); i++)
		delete objective[i];
	//objective.clear();
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

Quest_line::Quest_line(std::string quest_line_name)
{
	this->name = quest_line_name;
	quest = nullptr;
	next_quest_name = "";
	quest_bitmap = al_load_bitmap("bitmaps/items/quest_background.png");
	background = al_load_bitmap("bitmaps/items/inventory_background.png");
	player = al_load_bitmap("player/player_move.png");
}

void Quest_line::quest_file_read(std::string quest_line_name, std::string quest_name_s)
{
	try
	{
		std::string file_name = "Quest/" + quest_line_name + "/" + quest_name_s + ".txt";
		std::fstream file;
		file.open(file_name);
		if (file)
		{
			std::string trash;
			std::getline(file, trash);
			std::getline(file, trash);
			std::getline(file, trash);
			std::string class_type, enum_type, how_many, location_name;
			while (file)
			{
				std::getline(file, class_type);
				std::getline(file, location_name);
				std::getline(file, enum_type);
				std::getline(file, how_many);
				switch (stoi(class_type))
				{
				case 0: // Character 
					add_quest(quest_name_s, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 0);
					break;
				case 1: // Element
					add_quest(quest_name_s, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 1);
					break;
				case 2: // Location
					add_quest(quest_name_s, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 2);
					break;
				case 3: // Item
					add_quest(quest_name_s, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 3);
					break;
				}
				std::getline(file, trash);
				if (trash == "}")
					break;
			}
			std::getline(file, trash);
			next_quest_name = trash;
			quest->name = quest_name_s;
			std::cout << "Actual " << this->quest->get_name() << std::endl;
			std::cout << "Next " << next_quest_name << std::endl;
		}
	}
	catch (std::ifstream::failure e) 
	{
		std::cerr << "Exception opening/reading/closing file\n";
	}
}

void Quest_line::add_quest(std::string name, int target_id, std::string target_location_name, TYPE to_do, int how_many, int what_class)
{
	if (!quest)
		quest = new Quest(name, target_id, target_location_name, to_do, how_many, what_class);
	else
		quest->add_objective(target_id, target_location_name, to_do, how_many, what_class);
}

void Quest_line::show_quests(ALLEGRO_FONT* setting_font, int &j)
{	
	al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * j, 0, quest->get_name().c_str());
	for (int i = 0; i < quest->objective.size(); i++) // wypisywanie objectivow
	{
		std::string tmp;
		switch (quest->objective[i]->get_to_do_int())
		{
		case 0: //KILL
			if (quest->objective[i]->get_how_many() == 1)
			{
				tmp = "    -Zabij " + std::to_string(quest->objective[i]->get_target_id()) + " w " + quest->objective[i]->get_target_location();
				al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			}
			else
			{
				tmp = "    -Zabij " + std::to_string(quest->objective[i]->get_how_many()) + " " + std::to_string(quest->objective[i]->get_target_id()) + " w " + quest->objective[i]->get_target_location();
				al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			}
			break;
		case 1://SPEAK TO
			tmp = "    -Porozmawiaj z " + std::to_string(quest->objective[i]->get_target_id()) + " w " + quest->objective[i]->get_target_location();
			al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			break;
		case 2: //GO_TO
			tmp = "    -Idz do " +  quest->objective[i]->get_target_location();
			al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			break;
		case 3: //TAKE
			if (quest->objective[i]->get_how_many() == 1)
			{
				tmp = "    -Zdobadz " + std::to_string(quest->objective[i]->get_target_id()) + " w " + quest->objective[i]->get_target_location();
				al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			}
			else
			{
				tmp = "    -Zdobadz " + std::to_string(quest->objective[i]->get_how_many()) + " " + std::to_string(quest->objective[i]->get_target_id()) + " w " + quest->objective[i]->get_target_location();
				al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (1 + i + j), 0, tmp.c_str());
			}
			break;
		}
	}
	j += quest->objective.size();
}

bool Quest_line::take_next_quest()
{
	//quest->~Quest();
	if (next_quest_name == "")
		return false;
	quest_file_read(name, next_quest_name);
	return true;
}

void Quest_line::save_status(std::fstream& file)
{
	file << name << std::endl << "{" << std::endl;
	file << quest->get_name() << std::endl << "{" << std::endl;
	for (int i = 0; i < quest->objective.size(); i++)
	{
		file << quest->objective[i]->get_target_id() << std::endl;
		if(typeid(Character_objective) == typeid(*quest->objective[i]))
			file << "0" << std::endl;
		else if (typeid(Element_objective) == typeid(*quest->objective[i]))
			file << "1" << std::endl;
		else if (typeid(Location_objective) == typeid(*quest->objective[i]))
			file << "2" << std::endl;
		else if (typeid(Item_objective) == typeid(*quest->objective[i]))
			file << "3" << std::endl;
		file << quest->objective[i]->get_target_location() << std::endl;
		file << quest->objective[i]->get_to_do() << std::endl;
		file << quest->objective[i]->get_how_many() << std::endl;
	}
	file << "}" << std::endl;
}

std::string Quest_line::get_name()
{
	return name;
}

void Quest_line::load_status(std::fstream& file, std::string quest_name)
{
	std::string trash, class_type, location_name, enum_type, how_many;
	std::getline(file, trash);
	while (true)
	{
		std::getline(file, class_type);
		std::getline(file, location_name);
		std::getline(file, enum_type);
		std::getline(file, how_many);
		switch (stoi(class_type))
		{
		case 0: // Character 
			add_quest(quest_name, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 0);
			break;
		case 1: // Element
			add_quest(quest_name, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 1);
			break;
		case 2: // Location
			add_quest(quest_name, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 2);
			break;
		case 3: // Item
			add_quest(quest_name, stoi(trash), location_name, (TYPE)(stoi(enum_type)), stoi(how_many), 3);
			break;
		}
		std::getline(file, trash);
		if (trash == "}")
			break;
	}
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
	delete quest;
}
