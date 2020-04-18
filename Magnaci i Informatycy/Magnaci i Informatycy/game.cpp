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
	who->change_coordinates(nextX, nextY);
	map[nextX][nextY] = who;
	map[prevX][prevY] = nullptr;
}

bool window::player_movement() // ruch gracza na planszy
{
	al_get_keyboard_state(&keyboard);
	Character* tmp = dynamic_cast<Character*>(player);	

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE))
	{
		tmp->change_attack_type(1);
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
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
		return false;
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
	case 1:
		tmp->change_texture("dupa/player_attack.png");
		tmp->what_attack_should_I_draw(5);
		if (!tmp->get_attack_type())
			tmp->change_texture("dupa/player_move.png");
		break;
	}
}

bool window::game_working()// odœwierzenie planszy 
{
	bool draw = true;
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* move_timer = al_create_timer(1.0 / FPS / 400);
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
				if (dynamic_cast<Character*>(player)->movement_cooldown < al_get_time() + 5)
				{
					if (!player_movement())
						return false;
				}
			}
			else if (events.timer.source == move_timer)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				location->draw(player->get_X(), player->get_Y());
				location->draw_mobs(player->get_X(), player->get_Y());
				player->draw();
				draw_buttons();
				al_flip_display();
			}
		}
	}
	//al_draw_scaled_bitmap(bitmap, 0, 0, rozmiar_dla_full_HD_szerokosc, rozmiar_dla_full_HD_wysokosc, pozycja_po_X, pozycja_po_y, przeskalowana_szerokosc, przeskalowana_wyskosc)
	//switch (events.type)
	//{
	//case ALLEGRO_EVENT_TIMER:
	//	break;
	//case ALLEGRO_EVENT_KEY_DOWN:
	//	break;
	//default:
	//	break;
	//}
	
}

void window::start() // pierwsze uruchomienie planszy
{
	srand(time(NULL));
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);
	player = new Magnat(2,2, 110000, "player/player.txt");
	location = new Location("Plains1", 0, 0, this->map);
}