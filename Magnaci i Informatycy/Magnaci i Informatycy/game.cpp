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

void window::inventory()
{
	al_stop_timer(timer);
	al_stop_timer(move_timer);
	int mouse_inv_x = 0;
	int mouse_inv_y = 0;
	Character* tmp = dynamic_cast<Character*>(player);
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
						holding_item = tmp->get_inventory()->I_want_swap_this_item(mouse_inv_x, mouse_inv_y, holding_item, prev_x, prev_y);
				}
				else
				{
					holding_item = tmp->get_inventory()->I_want_take_this_equipment(mouse_inv_x, mouse_inv_y);
					if(!holding_item)
						holding_item = tmp->get_inventory()->I_want_take_this_item(mouse_inv_x, mouse_inv_y, prev_x, prev_y);
				}
			}
			else if (events.mouse.button & 2)
				break;
		}
		else if(events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			std::cout << "1";
		}
		tmp->get_inventory()->show_inventory();
		if(holding_item)
			holding_item->draw_in_inventory();
		al_draw_bitmap(cursor, mouse_inv_x, mouse_inv_y, 0);
		al_flip_display();
	}
	if (holding_item)
		tmp->get_inventory()->add_item_to_inventory(holding_item);
	al_destroy_bitmap(cursor);
	al_show_mouse_cursor(display);
	al_start_timer(timer);
	al_start_timer(move_timer);
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
		tmp->basic_attack(map, location->mobs, location->dead_mobs);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_1))
	{
		tmp->change_attack_type(2);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new Wind(tmp->get_X(), tmp->get_Y(), "player/player_action1.png", "fireball", 20, 10, 5, 8, 0, false, 6, player, tmp->direction));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_2))
	{
		tmp->change_attack_type(3);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new Self(tmp->get_X(), tmp->get_Y(), "player/player_action2.png", "heal", 5, 10, 5, 10, 0, false, 18, player, true));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_3))
	{
		tmp->change_attack_type(4);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new AoE(tmp->get_X(), tmp->get_Y(), "player/player_action3.png", "AoE Dmg", 5, 10, 5, 10, 0, false, 18, player, tmp->direction, -3, -3));
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_4))
	{
		tmp->change_attack_type(5);
		tmp->bitmap_start_x = 0;
		tmp->change_texture("player/player_attack.png");
		action.push_back(new Breath(tmp->get_X(), tmp->get_Y(), "player/player_action4", "Breath", 20, 10, 5, 8, 0, false, 6, player, tmp->direction));
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
		inventory();
	else if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE))
		return false;
	else if (al_key_down(&keyboard, ALLEGRO_KEY_H))
		tmp->change_hp(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_M))
		tmp->change_mana(-10);
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F1))
	{
		Item* new_weapon = new Armour(110001, "items/weapon_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_weapon);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F2))
	{
		Item* new_helmet = new Armour(210001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_helmet);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F3))
	{
		Item* new_chestplate = new Armour(220001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_chestplate);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F4))
	{
		Item* new_boots = new Armour(230001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_boots);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F5))
	{
		Item* new_amulet = new Armour(240001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_amulet);
	}
	else if (al_key_down(&keyboard, ALLEGRO_KEY_F6))
	{
		Item* new_ring = new Armour(250001, "items/armour_file.txt");
		tmp->get_inventory()->add_item_to_inventory(new_ring);
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
				location->mob_movement(player, map);
			}
			else if (events.timer.source == move_timer)
			{
				for (int i = 0; i < action.size(); i++)
				{
					if (!action[i]->make_action(map, location->get_sizeX(), location->get_sizeY(), player->get_X(), player->get_Y(), location->mobs, player, location->dead_mobs))
					{
						action.erase(action.begin() + i);
						i--;
					}
				}
				al_clear_to_color(al_map_rgb(0, 0, 0));
				location->draw(player->get_X(), player->get_Y());
				draw_actions(player->get_X(), player->get_Y());
				location->draw_mobs(player->get_X(), player->get_Y(), map);
				location->draw_dead_mobs(player->get_X(), player->get_Y(), map);
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
	for (int i = 0; i < action.size(); i++)
		action[i]->~Action();
	action.clear();
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

