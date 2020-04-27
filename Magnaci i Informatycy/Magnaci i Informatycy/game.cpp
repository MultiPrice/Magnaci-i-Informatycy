#include <iostream>

#include <typeinfo>
#include "window.h"

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

bool window::pause_game()
{
	al_draw_filled_rectangle(0, 0, screen_width, screen_height, al_map_rgba(0, 0, 0, 69));
	al_flip_display();
	al_stop_timer(timer);
	al_stop_timer(move_timer);
	while (true)
	{
		al_get_keyboard_state(&keyboard);
		if (al_key_down(&keyboard, ALLEGRO_KEY_R))
		{
			al_start_timer(timer);
			al_start_timer(move_timer);
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

bool window::player_movement() // ruch gracza na planszy
{
	al_get_keyboard_state(&keyboard);
	Character* tmp = dynamic_cast<Character*>(player);	

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE))
	{
		tmp->change_attack_type(1);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		tmp->basic_attack(map, location->mobs);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_1))
	{
		tmp->change_attack_type(2);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new Wind(tmp->get_X(), tmp->get_Y(), "player/player_action1.png", "fireball", 20, 20, 5, 8, 10, false, 10, player, tmp->direction));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_2))
	{
		tmp->change_attack_type(3);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new Self(tmp->get_X(), tmp->get_Y(), "player/player_action2.png", "heal", 5, 20, 5, 10, 10, false, 18, player, true));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_3))
	{
		tmp->change_attack_type(4);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new AoE(tmp->get_X(), tmp->get_Y(), "player/player_action3.png", "heal", 5, 20, 5, 10, 10, false, 18, player, tmp->direction, -3, -3));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT))
	{
		tmp->direction = RIGHT;
		if (map[player->get_X() + 1][player->get_Y()] == nullptr)
		{
			tmp->is_moving = true;
			tmp->what_move_should_I_draw();
			change_position(player, player->get_X(), player->get_Y(), player->get_X() + 1, player->get_Y());
		}
		else if (typeid(Element) == typeid(*map[player->get_X() + 1][player->get_Y()]))
		{
			if (dynamic_cast<Element*>(map[player->get_X() + 1][player->get_Y()])->get_ghosted() == true)
			{
				if (dynamic_cast<Element*>(map[player->get_X() + 1][player->get_Y()])->get_teleport() == true)
					restart(location->search_travel(player->get_X() + 1, player->get_Y()));
				else
				{
					tmp->is_moving = true;
					tmp->what_move_should_I_draw();
					change_position(player, player->get_X(), player->get_Y(), player->get_X() + 1, player->get_Y());
				}
			}
		}
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))
	{
		tmp->direction = LEFT;
		if (map[player->get_X() - 1][player->get_Y()] == nullptr)
		{
			tmp->is_moving = true;
			tmp->what_move_should_I_draw();
			change_position(player, player->get_X(), player->get_Y(), player->get_X() - 1, player->get_Y());
		}
		else if (typeid(Element) == typeid(*map[player->get_X() - 1][player->get_Y()]))
		{
			if (dynamic_cast<Element*>(map[player->get_X() - 1][player->get_Y()])->get_ghosted() == true)
			{
				if (dynamic_cast<Element*>(map[player->get_X() - 1][player->get_Y()])->get_teleport() == true)
					restart(location->search_travel(player->get_X() - 1, player->get_Y()));
				else
				{
					tmp->is_moving = true;
					tmp->what_move_should_I_draw();
					change_position(player, player->get_X(), player->get_Y(), player->get_X() - 1, player->get_Y());
				}
			}
		}
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_UP))
	{
		tmp->direction = UP;
		if (map[player->get_X()][player->get_Y() - 1] == nullptr)
		{
			tmp->is_moving = true;
			tmp->what_move_should_I_draw();
			change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() - 1);
		}
		else if (typeid(Element) == typeid(*map[player->get_X()][player->get_Y() - 1]))
		{
			if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() - 1])->get_ghosted() == true)
			{
				if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() - 1])->get_teleport() == true)
					restart(location->search_travel(player->get_X(), player->get_Y() - 1));
				else
				{
					tmp->is_moving = true;
					tmp->what_move_should_I_draw();
					change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() - 1);
				}
			}
		}
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN))
	{
		tmp->direction = DOWN;
		if (map[player->get_X()][player->get_Y() + 1] == nullptr)
		{
			tmp->is_moving = true;
			tmp->what_move_should_I_draw();
			change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() + 1);
		}
		else if (typeid(Element) == typeid(*map[player->get_X()][player->get_Y() + 1]))
		{
			if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() + 1])->get_ghosted() == true)
			{
				if (dynamic_cast<Element*>(map[player->get_X()][player->get_Y() + 1])->get_teleport() == true)
					restart(location->search_travel(player->get_X(), player->get_Y() + 1));
				else
				{
					tmp->is_moving = true;
					tmp->what_move_should_I_draw();
					change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() + 1);
				}
			}
		}
	}
	else if(al_key_down(&keyboard, ALLEGRO_KEY_I))
		tmp->get_inventory()->show_inventory();
	else if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
		return false;
	else if (al_key_down(&keyboard, ALLEGRO_KEY_H))
		tmp->change_hp(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_M))
		tmp->change_mana(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_P))
	{
		Item* new_armour = new Armour(110001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_armour);
	}
	else
	{
		tmp->is_moving = false;
		tmp->what_move_should_I_draw();
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
	}
}

bool window::game_working()// odœwierzenie planszy 
{
	bool draw = true;
	ALLEGRO_EVENT_QUEUE* event_q = al_create_event_queue();
	al_register_event_source(event_q, al_get_keyboard_event_source());
	al_register_event_source(event_q, al_get_timer_event_source(timer));
	al_register_event_source(event_q, al_get_timer_event_source(move_timer));
	al_start_timer(timer);
	al_start_timer(move_timer);
	while (true)
	{
		al_wait_for_event(event_q, &events);

		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			if (events.timer.source == timer)
			{
				if (dynamic_cast<Character*>(player)->get_attack_type())
				{
					for (int i = 0; i < action.size(); i++)
						action[i]->prepare_action();
					player_attack();
				}
				else if (dynamic_cast<Character*>(player)->movement_cooldown < al_get_time() + 5)
				{
					if (!player_movement())
						if (!pause_game())
						{
							al_destroy_timer(timer);
							al_destroy_timer(move_timer);
							al_destroy_event_queue(event_q);
							return false;
						}
				}
			}
			else if (events.timer.source == move_timer)
			{
				for (int i = 0; i < action.size(); i++)
				{
					if (!action[i]->make_action(map, location->get_sizeX(), location->get_sizeY(), player->get_X(), player->get_Y(), location->mobs, player))
					{
						action.erase(action.begin() + i);
						i--;
					}
				}
				al_clear_to_color(al_map_rgb(0, 0, 0));
				location->draw(player->get_X(), player->get_Y());
				draw_actions(player->get_X(), player->get_Y());
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				location->draw_portals(player->get_X(), player->get_Y(), map);
				player->draw(map, location->get_sizeX(), location->get_sizeY());
				draw_hud(dynamic_cast<Character*>(player));
				draw_buttons();
				al_flip_display();
			}
		}
	}
}

void window::start() // pierwsze uruchomienie planszy
{
	srand(time(NULL));
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);
	player = new Magnat(2,2, 110000, "player/player.txt");
	location = new Location("Plains1", 0, 0, this->map);
	map[player->get_X()][player->get_Y()] = player;
	test = false;
}

void window::restart(std::string location_name)
{
	al_stop_timer(timer);
	al_stop_timer(move_timer);
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);
	map[player->get_X()][player->get_Y()] = nullptr;
	map_clear();
	int toX = location->get_pTravel()->toX;
	int toY = location->get_pTravel()->toY; 
	location->~Location();

	player->change_position(toX, toY);
	location = new Location(location_name, 0, 0, this->map);
	map[player->get_X()][player->get_Y()] = player;
	test = true;
	al_start_timer(timer);
	al_start_timer(move_timer);
}

void window::map_clear() // mapa jest powalona i trzeba stestowac
{
	for (int i = 0; i < location->get_sizeX(); i++)
	{
		for (int j = 0; j < location->get_sizeY(); j++)
				delete map[i][j];
		delete[] map[i];
	}
	delete[] map;
}

