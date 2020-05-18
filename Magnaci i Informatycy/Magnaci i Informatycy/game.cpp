#include <iostream>

#include <typeinfo>
#include "window.h"
//
//void wypisz_kurde_wszytsko(Object*** twoja_stara)
//{
//	system("cls");
//	for (int i = 0; i < 36; i++)
//	{
//		for (int j = 0; j < 64; j++)
//		{
//			if (twoja_stara[j][i] == nullptr)
//				std::cout << "O";
//			else if (typeid(Magnat)==typeid(*twoja_stara[j][i]))
//				std::cout << " ";
//			else if (typeid(Element) == typeid(*twoja_stara[j][i]))
//				std::cout << "W";
//		}
//			
//		std::cout << std::endl;
//	}
//}

void wypisz_kurde_wszytsko(Object*** twoja_stara)
{
	system("cls");
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (twoja_stara[j][i] == nullptr)
				std::cout << "O";
			else if (typeid(Magnat)==typeid(*twoja_stara[j][i]))
				std::cout << " ";
			else if (typeid(Element) == typeid(*twoja_stara[j][i]))
				std::cout << "W";
		}
			
		std::cout << std::endl;
	}
}

void window::change_position(Object*& who, int prevX, int prevY, int nextX, int nextY)
{
	who->change_position(nextX, nextY);
	map[nextX][nextY] = who;
	map[prevX][prevY] = nullptr;
}

void window::draw_actions(int position_x, int position_y)
{
	for(int i = 0; i < action.size(); i++)
		if(action[i]->get_cast_time() <= 0)
			action[i]->get_representation()->draw(position_x - shiftX, position_y - shiftY - 1);
}

void window::inventory(Inventory* droped)
{
	al_stop_timer(movement_timer);
	al_stop_timer(draw_timer);
	int mouse_inv_x = 0;
	int mouse_inv_y = 0;
	Character* tmp = dynamic_cast<Character*>(player);
	tmp->remove_bonuses();
	ALLEGRO_BITMAP* cursor = al_load_bitmap("bitmaps/background/mouse_cursor.png");
	Item* holding_item = nullptr;
	int prev_x;
	int prev_y;
	Item* swap_item;
	al_hide_mouse_cursor(display);
	while (true)
	{
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_inv_x = events.mouse.x;
			mouse_inv_y = events.mouse.y;
			if (holding_item)
			{
				holding_item->change_inventory_x(mouse_inv_x - 0.5 * measure);
				holding_item->change_inventory_y(mouse_inv_y - 0.5 * measure);
			}
		}
		else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (events.mouse.button & 1)
			{
				if (holding_item)
				{
					holding_item = tmp->get_inventory()->I_want_equip_this_item(mouse_inv_x, mouse_inv_y, holding_item);
					if(holding_item)
						holding_item = tmp->get_inventory()->I_want_swap_this_item(mouse_inv_x, mouse_inv_y, holding_item, prev_x, prev_y, droped);
				}
				else
				{
					holding_item = tmp->get_inventory()->I_want_take_this_equipment(mouse_inv_x, mouse_inv_y);
					if(!holding_item)
						holding_item = tmp->get_inventory()->I_want_take_this_item(mouse_inv_x, mouse_inv_y, prev_x, prev_y, 1);
					if (droped && !holding_item)
						holding_item = droped->I_want_take_this_item(mouse_inv_x, mouse_inv_y, prev_x, prev_y, 2);
				}
			}
			else if (events.mouse.button & 2)
				break;
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));
		tmp->get_inventory()->show_inventory();
		if (droped)
			droped->show_drop();
		if(holding_item)
			holding_item->draw_in_inventory();
		al_draw_bitmap(cursor, mouse_inv_x, mouse_inv_y, 0);
		al_flip_display();
	}
	if (holding_item)
		tmp->get_inventory()->add_item_to_inventory(holding_item, 1);
	tmp->add_bonuses();
	check_quest_item(tmp->get_inventory());
	al_destroy_bitmap(cursor);
	al_show_mouse_cursor(display);
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
}

void window::quests()
{
	al_stop_timer(movement_timer);
	al_stop_timer(draw_timer);
	int mouse_inv_x = 0;
	int mouse_inv_y = 0;
	ALLEGRO_BITMAP* cursor = al_load_bitmap("bitmaps/background/mouse_cursor.png");
	al_hide_mouse_cursor(display);
	while (true)
	{
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_inv_x = events.mouse.x;
			mouse_inv_y = events.mouse.y;
		}
		else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			if (events.mouse.button & 2)
				break;
		if(quest_line.empty())
			break;
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(quest_line[0]->background, 0, 0, 0);
		al_draw_bitmap(quest_line[0]->quest_bitmap, screen_width / 2 + measure, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(quest_line[0]->player, 0, measure * 6, measure * 1.5, measure * 2, al_map_rgb(255, 255, 255), 0, 0, 150, 60, 8, 8, 0, 0);
		int j = 2;
		for (int i = 0; i < quest_line.size(); i++)
		{
			quest_line[i]->show_quests(setting_font, j);
			j ++;
		}
		al_draw_bitmap(cursor, mouse_inv_x, mouse_inv_y, 0);
		al_flip_display();
	}
	al_destroy_bitmap(cursor);
	al_show_mouse_cursor(display);
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
}

bool window::dialogue_file_read(int character_id, std::string file_name)
{
	std::fstream file;
	file.open(file_name);
	if (file.good())
	{
		std::string linia, linia2;
		std::getline(file, linia);
		while (linia != std::to_string(character_id))
		{
			std::getline(file, linia);
			if (file.eof())
			{
				file.close();
				return false;
			}
		}
		std::getline(file, linia);
		std::getline(file, linia);
		bool check = false;
		int tmp_index = 0;
		while (linia != "}")
		{
			std::getline(file, linia2);
			if (linia[0] == '#')
			{
				check = false;
				for (int i = 0; i < dialogue.size(); i++)
					if (dialogue[i]->question == linia)
					{
						check = true;
						tmp_index = i;
					}
				if (!check)
					dialogue.push_back(new Question(linia, stoi(linia2)));
			}
			else
			{
				if (!check)
					dialogue[dialogue.size() - 1]->add_answer(linia, stoi(linia2));
				else
					dialogue[tmp_index]->add_answer(linia, stoi(linia2));
			}
			std::getline(file, linia);
		}
		file.close();
	}
	else
	{
		file.close();
		return false;
	}
	return true;
}

void window::check_quest_item(Inventory* inventory)
{
	for (int i = 0; i < quest_line.size(); i++)
		for (int j = 0; j < quest_line[i]->get_quest()->objective.size(); j++)
			if (quest_line[i]->get_quest()->objective[j]->get_to_do() == TAKE)
				if(quest_line[i]->get_quest()->objective[j]->get_target_location() == location->get_name())
					for (int k = 0; k < inventory->get_inventory_size(); k++)
						if(inventory->get_item(k))
							if (inventory->get_item(k)->get_item_id() == quest_line[i]->get_quest()->objective[j]->get_target_id())
								if (quest_line[i]->get_quest()->objective[j]->is_it_done())
								{
									quest_line[i]->get_quest()->objective.erase(quest_line[i]->get_quest()->objective.begin() + j);
									if (quest_line[i]->get_quest()->objective.empty())
										if (!quest_line[i]->take_next_quest())
										{
											quest_line[i]->~Quest_line();
											quest_line.erase(quest_line.begin() + i);
											i--;
											break;
										}
								}
}

bool window::pause_game()
{
	al_draw_filled_rectangle(0, 0, screen_width, screen_height, al_map_rgba(0, 0, 0, 69));
	al_flip_display();
	al_stop_timer(movement_timer);
	al_stop_timer(draw_timer);
	while (true)
	{
		al_get_keyboard_state(&keyboard);
		if (al_key_down(&keyboard, ALLEGRO_KEY_R))
		{
			al_start_timer(movement_timer);
			al_start_timer(draw_timer);
			return true;
		}
		else if (al_key_down(&keyboard, ALLEGRO_KEY_E))
			return false;
	}
}

void window::draw_hud(Character* player)
{
	al_draw_bitmap(HUD,0,1020,0);
	float actual_hp = (float)player->get_hp() / (float)player->get_max_hp();
	float actual_mana = (float)player->get_mana() / (float)player->get_max_mana();
	al_draw_bitmap_region(HP, 0, 0, 600 * actual_hp, 40, 40, 1030, 0);
	al_draw_bitmap_region(MANA, 600-(600*actual_mana), 0, 600 * actual_mana, 40, 1280 + (600 - (600 * actual_mana)), 1030, 0);
}

void window::attack_in_movement(Character*& tmp)
{
	tmp->change_attack_type(1);
	tmp->bitmap_start_x = 0;
	tmp->change_texture("player/player_attack.png");
	tmp->basic_attack(map, location, quest_line);
}

void window::action_in_movement(Character*& tmp, Action* new_action, int type)
{
	tmp->change_attack_type(type);
	tmp->bitmap_start_x = 0;
	tmp->change_texture("player/player_attack.png");
	action.push_back(new_action);
}

void window::go_up(Character*& tmp)
{
	tmp->direction = UP;
	if (map[player->get_X()][player->get_Y() - 1] == nullptr)
	{
		tmp->is_moving = true;
		tmp->what_move_should_I_draw(8);
		change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() - 1);
	}
	else if (typeid(Element) == typeid(*map[player->get_X()][player->get_Y() - 1]))
	{
		if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() - 1])->get_ghosted() == true)
		{
			if (typeid(*map[player->get_X()][player->get_Y() - 1]) == typeid(Teleport))
				restart(location->search_travel(player->get_X(), player->get_Y() - 1));
			else
			{
				tmp->is_moving = true;
				tmp->what_move_should_I_draw(8);
				change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() - 1);
			}
		}
	}
}

void window::go_down(Character*& tmp)
{
	tmp->direction = DOWN;
	if (map[player->get_X()][player->get_Y() + 1] == nullptr)
	{
		tmp->is_moving = true;
		tmp->what_move_should_I_draw(8);
		change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() + 1);
	}
	else if (typeid(Element) == typeid(*map[player->get_X()][player->get_Y() + 1]))
	{
		if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() + 1])->get_ghosted() == true)
		{
			if (typeid(*map[player->get_X()][player->get_Y() + 1]) == typeid(Teleport))
				restart(location->search_travel(player->get_X(), player->get_Y() + 1));
			else
			{
				tmp->is_moving = true;
				tmp->what_move_should_I_draw(8);
				change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() + 1);
			}
		}
	}
}

void window::go_right(Character*& tmp)
{
	tmp->direction = RIGHT;
	if (map[player->get_X() + 1][player->get_Y()] == nullptr)
	{
		tmp->is_moving = true;
		tmp->what_move_should_I_draw(8);
		change_position(player, player->get_X(), player->get_Y(), player->get_X() + 1, player->get_Y());
	}
	else if (typeid(Element) == typeid(*map[player->get_X() + 1][player->get_Y()]))
	{
		if (dynamic_cast<Element*>(map[player->get_X() + 1][player->get_Y()])->get_ghosted() == true)
		{
			if (typeid(*map[player->get_X() + 1][player->get_Y()]) == typeid(Teleport))
				restart(location->search_travel(player->get_X() + 1, player->get_Y()));
			else
			{
				tmp->is_moving = true;
				tmp->what_move_should_I_draw(8);
				change_position(player, player->get_X(), player->get_Y(), player->get_X() + 1, player->get_Y());
			}
		}
	}
}

void window::go_left(Character*& tmp)
{
	tmp->direction = LEFT;
	if (map[player->get_X() - 1][player->get_Y()] == nullptr)
	{
		tmp->is_moving = true;
		tmp->what_move_should_I_draw(8);
		change_position(player, player->get_X(), player->get_Y(), player->get_X() - 1, player->get_Y());
	}
	else if (typeid(Element) == typeid(*map[player->get_X() - 1][player->get_Y()]))
	{
		if (dynamic_cast<Element*>(map[player->get_X() - 1][player->get_Y()])->get_ghosted() == true)
		{
			if (typeid(*map[player->get_X() - 1][player->get_Y()]) == typeid(Teleport))
				restart(location->search_travel(player->get_X() - 1, player->get_Y()));
			else
			{
				tmp->is_moving = true;
				tmp->what_move_should_I_draw(8);
				change_position(player, player->get_X(), player->get_Y(), player->get_X() - 1, player->get_Y());
			}
		}
	}
}

void window::inventory_interaction(Character*& tmp)
{
	switch (tmp->direction)
	{
	case UP:
		inventory(location->find_dead_mob(player->get_X(), player->get_Y() - 1)->get_drop());
		break;
	case DOWN:
		inventory(location->find_dead_mob(player->get_X(), player->get_Y() + 1)->get_drop());
		break;
	case RIGHT:
		inventory(location->find_dead_mob(player->get_X() + 1, player->get_Y())->get_drop());
		break;
	case LEFT:
		inventory(location->find_dead_mob(player->get_X() - 1, player->get_Y())->get_drop());
		break;
	}
}

void window::portal_interaction(Character*& tmp)
{
	switch (tmp->direction)
	{
	case UP:
		restart(location->search_travel(player->get_X(), player->get_Y() - 1));
		break;
	case DOWN:
		restart(location->search_travel(player->get_X(), player->get_Y() + 1));
		break;
	case RIGHT:
		restart(location->search_travel(player->get_X() + 1, player->get_Y()));
		break;
	case LEFT:
		restart(location->search_travel(player->get_X() - 1, player->get_Y()));
		break;
	}
}

void window::dialogue_interaction(Character*& tmp, Object*& obj)
{
	bool check = false;
	for (int i = 0; i < quest_line.size(); i++)
		for (int j = 0; j < quest_line[i]->get_quest()->objective.size(); j++)
			if (quest_line[i]->get_quest()->objective[j]->get_to_do() == SPEAK_TO)
				if (quest_line[i]->get_quest()->objective[j]->get_target_id() == dynamic_cast<Character*>(obj)->get_id())
				{
					std::cout << dynamic_cast<Character*>(obj)->get_id() << " " << quest_line[i]->get_name() << " " << quest_line[i]->get_quest()->get_name() << std::endl;
					if (dialogue_file_read(dynamic_cast<Character*>(obj)->get_id(), "Quest/" + quest_line[i]->get_name() + "/" + quest_line[i]->get_quest()->get_name() + "_dialogues.txt"))
					{
						std::cout << "DUPA";
						check = true;
					}
				}
	if (dialogue_file_read(dynamic_cast<Character*>(obj)->get_id(), "Locations/" + location->get_terrain_name() + "/" + location->get_name() + "/" + "dialogues.txt"))
		check = true;
	if(check)
		show_dialogue(dynamic_cast<Character*>(obj)->get_id());
}

void window::any_interaction(Character*& tmp)
{
	switch (tmp->interact(map, location, player))
	{
	case 0:
		break;
	case 1:
		portal_interaction(tmp);
		break;
	case 2:
		inventory_interaction(tmp);
		break;
	case 3:
		switch (tmp->direction)
		{
		case UP:
			dialogue_interaction(tmp, map[player->get_X()][player->get_Y() - 1]);
			break;
		case DOWN:
			dialogue_interaction(tmp, map[player->get_X()][player->get_Y() + 1]);
			break;
		case RIGHT:
			dialogue_interaction(tmp, map[player->get_X() + 1][player->get_Y()]);
			break;
		case LEFT:
			dialogue_interaction(tmp, map[player->get_X() - 1][player->get_Y()]);
			break;
		}
		dialogue.clear();
		break;
	}
}

bool window::player_movement() // ruch gracza na planszy
{
	al_get_keyboard_state(&keyboard);
	Character* tmp = dynamic_cast<Character*>(player);	

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE))
		attack_in_movement(tmp);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_1))
		action_in_movement(tmp, new Wind(tmp->get_X(), tmp->get_Y(), "player/player_action1.png", "fireball", 20, 10, 5 * 3, 8, 0, false, 6, player, tmp->direction), 2);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_2))
		action_in_movement(tmp, new Self(tmp->get_X(), tmp->get_Y(), "player/player_action2.png", "heal", 5, 10, 5 * 3, 10, 0, false, 18, player, true), 2);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_3))
		action_in_movement(tmp, new AoE(tmp->get_X(), tmp->get_Y(), "player/player_action3.png", "AoE Dmg", 5, 10, 5 * 3, 10, 0, false, 18, player, tmp->direction, -3, -3), 2);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_4))
		action_in_movement(tmp, new Breath(tmp->get_X(), tmp->get_Y(), "player/player_action4", "Breath", 20, 10, 5 * 3, 8, 0, false, 6, player, tmp->direction), 2);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT))
		go_right(tmp);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))
		go_left(tmp);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_UP))
		go_up(tmp);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN))
		go_down(tmp);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_I))
		inventory(nullptr);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_Q))
		quests();
	else if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
		return false;
	else if (al_key_down(&keyboard, ALLEGRO_KEY_H))
		tmp->change_hp(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_M))
		tmp->change_mana(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F11))
		std::cout << tmp->get_X() << " " << tmp->get_Y() << std::endl;
	/*else if (al_key_down(&keyboard, ALLEGRO_KEY_F1))
		tmp->get_inventory()->add_item_to_inventory(new Weapon(110001, "items/weapon_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F2))
		tmp->get_inventory()->add_item_to_inventory(new Armour(210001, "items/armour_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F3))
		tmp->get_inventory()->add_item_to_inventory(new Armour(220001, "items/armour_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F4))
		tmp->get_inventory()->add_item_to_inventory(new Armour(230001, "items/armour_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F5))
		tmp->get_inventory()->add_item_to_inventory(new Armour(240001, "items/armour_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F6))
		tmp->get_inventory()->add_item_to_inventory(new Armour(250001, "items/armour_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F7))
		tmp->get_inventory()->add_item_to_inventory(new Weapon(130001, "items/weapon_file.txt"), 1);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_BACKSPACE))
		quest_line.push_back(new Quest_line("Testowa_linia", "qt1"));*/
	else if (al_key_down(&keyboard, ALLEGRO_KEY_E))
		any_interaction(tmp);
	else
	{
		tmp->is_moving = false;
		tmp->what_move_should_I_draw(8);
	}
	return true;
}

void window::player_attack()
{
	Character* tmp = dynamic_cast<Character*>(player);
	switch (tmp->get_attack_type())
	{
	case 1: //basic attack
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("player/player_move.png");
		break;
	case 2: // fireball
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("player/player_move.png");
		break;
	case 3: // heal
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("player/player_move.png");
		break;
	case 4: // AoE Dmg
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("player/player_move.png");
		break;
	case 5: // Breath Dmg
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("player/player_move.png");
		break;
	}
}

bool window::game_working()// odœwierzenie planszy 
{
	bool draw = true;
	ALLEGRO_EVENT_QUEUE* event_q = al_create_event_queue();
	al_register_event_source(event_q, al_get_keyboard_event_source());
	al_register_event_source(event_q, al_get_timer_event_source(movement_timer));
	al_register_event_source(event_q, al_get_timer_event_source(draw_timer));
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
	while (true)
	{
		al_wait_for_event(event_q, &events);

		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			if (events.timer.source == movement_timer)
			{
				if (dynamic_cast<Character*>(player)->get_attack_type())
				{
					for (int i = 0; i < action.size(); i++)
						action[i]->prepare_action();
					player_attack();
				}
				else if (dynamic_cast<Character*>(player)->movement_cooldown < al_get_time() + 5.0)
				{
					if (!player_movement())
						if (!pause_game())
						{
							save_game();
							clear_all();
							al_destroy_event_queue(event_q);
							return false;
						}
				}
				if (!location->mobs.empty())
					if (dynamic_cast<Character*>(location->mobs[0])->movement_cooldown + 9.0 / FPS <= al_get_time())
					{
						location->mob_movement(player, map);
						if (dynamic_cast<Character*>(player)->get_hp() == 0)
						{
							clear_all();
							al_destroy_event_queue(event_q);
;							return false;
						}
					}
				if(dynamic_cast<Character*>(player)->regenertion_cooldown + 30.0 / FPS <= al_get_time())
					dynamic_cast<Character*>(player)->resources_regeneration();
			}
			if (events.timer.source == draw_timer)
			{
				for (int i = 0; i < action.size(); i++)
				{
					if (!action[i]->make_action(map, location->get_sizeX(), location->get_sizeY(), player->get_X(), player->get_Y(), player, location, quest_line))
					{
						action.erase(action.begin() + i);
						i--;
					}
				}
				al_clear_to_color(al_map_rgb(0, 0, 0));
				location->draw(player->get_X(), player->get_Y());
				location->draw_dead_mobs(player->get_X(), player->get_Y(), map);
				draw_actions(player->get_X(), player->get_Y());
				location->draw_portals(player->get_X(), player->get_Y(), map);
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				player->draw(map, location->get_sizeX(), location->get_sizeY());
				draw_hud(dynamic_cast<Character*>(player));
				draw_buttons();
				al_flip_display();
			}
		}
	}
}

void window::check_location_quest(std::vector<Quest_line*> quest_line, int location_id)
{
	for (int i = 0; i < quest_line.size(); i++)
		for (int j = 0; j < quest_line[i]->get_quest()->objective.size(); j++)
			if (quest_line[i]->get_quest()->objective[j]->get_to_do() == GO_TO)
				if (location_id == quest_line[i]->get_quest()->objective[j]->get_target_id())
					if (quest_line[i]->get_quest()->objective[j]->is_it_done())
					{
						quest_line[i]->get_quest()->objective.erase(quest_line[i]->get_quest()->objective.begin() + j);
						if (quest_line[i]->get_quest()->objective.empty())
							if (!quest_line[i]->take_next_quest())
							{
								quest_line[i]->~Quest_line();
								quest_line.erase(quest_line.begin() + i);
								i--;
								break;
							}
					}
}

void window::clear_all()
{
	al_stop_timer(movement_timer);
	al_stop_timer(draw_timer);
	map_clear();
	location->~Location();
	if(!action.empty())
		for (int i = 0; i < action.size(); i++)
			action[i]->~Action();
	if(!quest_line.empty())
		for (int i = 0; i < quest_line.size(); i++)
			quest_line[i]->~Quest_line();
	quest_line.clear();
	action.clear();
	al_destroy_timer(movement_timer);
	al_destroy_timer(draw_timer);
}

void window::start()
{
	//add_login();
	movement_timer = al_create_timer(6.0 / FPS);
	draw_timer = al_create_timer(1.0 / FPS);
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
	srand(time(NULL));
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);
	player = new Magnat(67,40, 110000, "player/player.txt");
	location = new Location("Forest2", 0, 0, this->map);
	map[player->get_X()][player->get_Y()] = player;
	quest_line.push_back(new Quest_line("Stachu_Jones", "Poczatek przygody"));
}

void window::restart(std::string location_name)
{
	al_stop_timer(movement_timer);
	al_stop_timer(draw_timer);
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);
	map[player->get_X()][player->get_Y()] = nullptr;
	map_clear();
	int toX = location->get_pTravel(location_name)->toX;
	int toY = location->get_pTravel(location_name)->toY; 
	int location_id = location->get_id();
	location->~Location();
	for (int i = 0; i < action.size(); i++)
		action[i]->~Action();
	action.clear();
	player->change_position(toX, toY);
	location = new Location(location_name, 0, 0, this->map);
	map[player->get_X()][player->get_Y()] = player;
	check_location_quest(quest_line, location_id);
	save_game();
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
}

int window::check_answer(int mouse_x, int mouse_y, Question * question)
{
	if (mouse_x < screen_width / 2 + measure * 3)
		return -1;
	if (mouse_y < measure * 2)
		return -1;
	for (int i = 0; i < question->all_answers.size(); i++)
		if (mouse_y >= measure * (2 + i) && mouse_y < measure * (3 + i))
			return question->all_answers[i]->next_question_id;
	return -1;
}



void window::show_dialogue(int character_id)
{
	al_stop_timer(draw_timer);
	al_stop_timer(movement_timer);
	int mouse_x = 0;
	int mouse_y = 0;
	int actual_question = 0;
	int actual_index = 0;
	bool quest_check = false;
	Character* tmp = dynamic_cast<Character*>(player);
	ALLEGRO_BITMAP* cursor = al_load_bitmap("bitmaps/background/mouse_cursor.png");
	al_hide_mouse_cursor(display);
	while (true)
	{
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_x = events.mouse.x;
			mouse_y = events.mouse.y;
		}
		else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (events.mouse.button & 1) //LPM
			{
				actual_question = check_answer(mouse_x, mouse_y, dialogue[actual_index]);
				if (actual_question == 0)
					break;
				if (actual_question > 0)
					for (int i = 0; i < dialogue.size(); i++)
						if (actual_question == dialogue[i]->id)
							actual_index = i;
				if (actual_question > 100)
					quest_check = true;
			}
			else if (events.mouse.button & 2) //PPM
				break;
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));
		location->draw(player->get_X(), player->get_Y());
		location->draw_dead_mobs(player->get_X(), player->get_Y(), map);
		draw_actions(player->get_X(), player->get_Y());
		location->draw_mobs(player->get_X(), player->get_Y(), map);
		location->draw_portals(player->get_X(), player->get_Y(), map);
		player->draw(map, location->get_sizeX(), location->get_sizeY());
		draw_hud(dynamic_cast<Character*>(player));
		draw_dialogue(dialogue[actual_index]);
		al_draw_bitmap(cursor, mouse_x, mouse_y, 0);
		al_flip_display();
	}
	check_dialogue_quest(quest_check, quest_line, character_id);
		
	al_destroy_bitmap(cursor);
	al_show_mouse_cursor(display);
	al_start_timer(movement_timer);
	al_start_timer(draw_timer);
}

void window::check_dialogue_quest(bool quest_check, std::vector<Quest_line*>& quest_line, int character_id)
{
	if (quest_check)
		for (int i = 0; i < quest_line.size(); i++)
			for (int j = 0; j < quest_line[i]->get_quest()->objective.size(); j++)
				if (quest_line[i]->get_quest()->objective[j]->get_to_do() == SPEAK_TO)
					if (quest_line[i]->get_quest()->objective[j]->get_target_id() == character_id)
						if (quest_line[i]->get_quest()->objective[j]->get_target_location() == location->get_name())
							if (quest_line[i]->get_quest()->objective[j]->is_it_done())
							{
								quest_line[i]->get_quest()->objective.erase(quest_line[i]->get_quest()->objective.begin() + j);
								if (quest_line[i]->get_quest()->objective.empty())
									if (!quest_line[i]->take_next_quest())
									{
										quest_line[i]->~Quest_line();
										quest_line.erase(quest_line.begin() + i);
										i--;
										return;
									}
							}
}

void window::draw_dialogue(Question* question)
{
	al_draw_bitmap(dialogue_window, screen_width / 2 + measure * 3, 0, 0);
	al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure, 0, question->question.c_str());
	for(int i = 0; i < question->all_answers.size(); i++)
		al_draw_text(setting_font, al_map_rgb(0, 0, 0), screen_width / 2 + measure * 4, measure * (2 + i), 0, question->all_answers[i]->answer.c_str());
}

void window::map_clear()
{
	for (int i = 0; i < location->get_sizeX(); i++)
	{
		for (int j = 0; j < location->get_sizeY(); j++)
				delete map[i][j];
		delete[] map[i];
	}
	delete[] map;
}

