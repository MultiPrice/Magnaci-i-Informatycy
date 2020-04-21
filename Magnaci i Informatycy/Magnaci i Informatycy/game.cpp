#include <iostream>

#include <typeinfo>
#include "window.h"

const int FPS = 60;

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

bool window::pause_game()
{
	al_draw_filled_rectangle(0, 0, screen_width, screen_height, al_map_rgba(0, 0, 0, 69));
	al_flip_display();
	while (true)
	{
		al_get_keyboard_state(&keyboard);
		if (al_key_down(&keyboard, ALLEGRO_KEY_R))
		{
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
	al_draw_bitmap_region(HP, 0, 0, 640 * actual_hp, 40, 40, 1030, 0);
	al_draw_bitmap_region(MANA, 640-(640*actual_mana), 0, 640 * actual_mana, 40, 1240 + (640 - (640 * actual_mana)), 1030, 0);
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
	else if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
		return false;
	else if (al_key_down(&keyboard, ALLEGRO_KEY_H))
		tmp->change_hp(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_M))
		tmp->change_mana(-10);
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
	}
}

bool window::game_working()// odœwierzenie planszy 
{
	bool draw = true;
	ALLEGRO_TIMER* timer = al_create_timer(6.0 / FPS);
	ALLEGRO_TIMER* move_timer = al_create_timer(1.0 / FPS);
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
					player_attack();
				}
				else if (dynamic_cast<Character*>(player)->movement_cooldown < al_get_time() + 5)
				{
					if (!player_movement())
						if(!pause_game())
							return false;
				}
			}
			else if (events.timer.source == move_timer)
			{
				/*if (test == true)
					wypisz_kurde_wszytsko(map);*/
				al_clear_to_color(al_map_rgb(0, 0, 0));
				location->draw(player->get_X(), player->get_Y());
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				location->draw_portals(player->get_X(), player->get_Y(), map);
				player->draw(map);
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
	test = false;
}

void window::restart(std::string location_name)
{
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
