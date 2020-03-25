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
			else if (typeid(Berserk)==typeid(*twoja_stara[j][i]))
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

void window::player_movement() // ruch gracza na planszy
{
	al_get_keyboard_state(&keyboard);
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT))
	{
		if (map[player->get_X() + 1][player->get_Y()] == nullptr)
		{
			change_position(player, player->get_X(), player->get_Y(), player->get_X() + 1, player->get_Y());
			//location->change_coordinates(location->get_X() - 1, location->get_Y());
			//location->change_mob_coordinates(-1, 0);
			//which_x_in_animation(player->bitmap_start_x);
			//player->bitmap_start_y = 0;
		}
		/*if (typeid(Element) == typeid(*map[player->get_X + 1][player->get_Y]))
		{
			if(dynamic_cast<Element*>(map[player->get_X + 1][player->get_Y])->czy_ghosted() == false)

		}*/
		/*if (typeid(personel) == typeid(*lista[i]))
			suma_pensji += dynamic_cast<personel*>(lista[i])->pracuj();*/
			//suma_pensji += dynamic_cast<personel*>(lista[i])->pracuj();
			//gracz->X += 1;
		wypisz_kurde_wszytsko(map);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))
	{
		if (map[player->get_X() - 1][player->get_Y()] == nullptr)
		{
			change_position(player, player->get_X(), player->get_Y(), player->get_X() - 1, player->get_Y());
			//location->change_coordinates(location->get_X() + 1, location->get_Y());
			//location->change_mob_coordinates(+1, 0);
			//which_x_in_animation(player->bitmap_start_x);
			//player->bitmap_start_y = 0;
		}
		wypisz_kurde_wszytsko(map);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_UP))
	{
		if (map[player->get_X()][player->get_Y() - 1] == nullptr)
		{
			change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() - 1);
			//location->change_coordinates(location->get_X(), location->get_Y() + 1);
			//location->change_mob_coordinates(0, +1);
			//which_x_in_animation(player->bitmap_start_x);
			//player->bitmap_start_y = 0;
		}
		wypisz_kurde_wszytsko(map);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN))
	{
		if (map[player->get_X()][player->get_Y() + 1] == nullptr)
		{
			change_position(player, player->get_X(), player->get_Y(), player->get_X(), player->get_Y() + 1);
			//location->change_coordinates(location->get_X(), location->get_Y() - 1);
			//location->change_mob_coordinates(0, -1);
			//which_x_in_animation(player->bitmap_start_x);
			//player->bitmap_start_y = 0;
		}
		wypisz_kurde_wszytsko(map);
	}
}

void window::game_working()// od�wierzenie planszy 
{
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
	if (dynamic_cast<Character*>(player)->movement_cooldown < al_get_time() + 0.5)
	{
		player_movement();
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	location->draw(player->get_X(), player->get_Y());
	//location->draw_mobs();
	//player->draw(screen_width / 60, screen_height / 60);
	player->draw();
	//std::cout << player->get_X() << " " << player->get_Y() << std::endl;
	draw_buttons();

	al_flip_display();
	//update_positions();
}

void window::start() // pierwsze uruchomienie planszy
{
	clear();
	al_clear_to_color(al_map_rgb(0, 150, 0));
	add_functional_button(10, 10, MENU);

	player = new Berserk(2,2, 110000, "player/player.txt");
	location = new Location("Plains1", 0, 0, this->map);
}