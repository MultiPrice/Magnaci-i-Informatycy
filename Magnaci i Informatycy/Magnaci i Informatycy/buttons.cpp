#include "buttons.h"



Button::Button(int pposition_x, int pposition_y, int psize_x, int psize_y, bool pis_fader)
	:position_x(pposition_x), position_y(pposition_y), size_x(psize_x), size_y(psize_y), is_fader(pis_fader)
{
	is_selected = false;
	selected = nullptr;
	unselected = nullptr;
	visible_bitmap = nullptr;
}

void Button::select()		//zmiana aktualnej bitmapki
{
	is_selected = true;
	visible_bitmap = selected;
}

void Button::unselect()		//zmiana aktualnej bitmapki
{
	is_selected = false;
	visible_bitmap = unselected;
}

void Button::draw()
{
	al_draw_scaled_bitmap(visible_bitmap, 0, 0, 188, 43, position_x, position_y, size_x, size_y, 0);
}

Functional_Button::Functional_Button(int pposition_x, int pposition_y, int psize_x, int psize_y, BUTTON_FUNCTION pfunction)
	:Button(pposition_x, pposition_y, psize_x, psize_y, false), function(pfunction)
{
	switch (function)		//ustalenie wygl¹du przycisków w zale¿noœci od funkcji przycisku
	{
	case 0:
		selected = al_load_bitmap("bitmaps/buttons/selected_start.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_start.bmp");
		break;
	case 1:
		selected = al_load_bitmap("bitmaps/buttons/selected_pause.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_pause.bmp");
		break;
	case 2:
		selected = al_load_bitmap("bitmaps/buttons/selected_exit.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_exit.bmp");
		break;
	case 3:
		selected = al_load_bitmap("bitmaps/buttons/selected_resume.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_resume.bmp");
		break;
	case 4:
		selected = al_load_bitmap("bitmaps/buttons/selected_settings.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_settings.bmp");
		break;
	case 5:
		selected = al_load_bitmap("bitmaps/buttons/selected_load.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_load.bmp");
		break;
	case 6:
		selected = al_load_bitmap("bitmaps/buttons/selected_menu.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_menu.bmp");
		break;
	case 7:
		selected = al_load_bitmap("bitmaps/buttons/selected_audio.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_audio.bmp");
		break;
	case 8:
		selected = al_load_bitmap("bitmaps/buttons/selected_graphic.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_graphic.bmp");
		break;
	case 9:
		selected = al_load_bitmap("bitmaps/buttons/selected_move.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_move.bmp");
		break;
	case 10:
		selected = al_load_bitmap("bitmaps/buttons/selected_screen_mode.bmp");
		unselected = al_load_bitmap("bitmaps/buttons/unselected_screen_mode.bmp");
		break;
	}
	visible_bitmap = unselected;
}

int Functional_Button::click(int* mapping_tab)
{
	return function;
}

bool Functional_Button::check_position(int mouse_position_x, int mouse_position_y)
{
	if (mouse_position_x >= position_x && mouse_position_x < position_x + size_x)
	{
		if (mouse_position_y >= position_y && mouse_position_y < position_y + size_y)
			return true;
	}
	return false;
}

Mapping_Button::Mapping_Button(int pposition_x, int pposition_y, int psize_x, int psize_y, int pbutton_number)
	:Button(pposition_x, pposition_y, psize_x, psize_y, false), button_number(pbutton_number)
{
}

int Mapping_Button::click(int* mapping_tab)
{
	return 0;
}

bool Mapping_Button::check_position(int mouse_position_x, int mouse_position_y)
{
	if (mouse_position_x >= position_x && mouse_position_x < position_x + size_x)
	{
		if (mouse_position_y >= position_y && mouse_position_y < position_y + size_y)
			return true;
	}
	return false;
}

Fader::Fader(int pposition_x, int pposition_y, int psize_x, int psize_y, int back_size_x, int back_size_y, FADER_FUNCTION pfunction, int* mapping_tab)
	:Button(pposition_x, pposition_y, psize_x, psize_y, true), back_size_x(back_size_x), back_size_y(back_size_y), function(pfunction)
{
	pointer_position = ((float)(mapping_tab[function]) / 100) * (back_size_x - size_x);
	selected = al_load_bitmap("bitmaps/buttons/selected_fader.bmp");
	unselected = al_load_bitmap("bitmaps/buttons/unselected_fader.bmp");
	fader = al_load_bitmap("bitmaps/buttons/fader.bmp");
	visible_bitmap = unselected;
}

void Fader::fader_draw()
{
	al_draw_scaled_bitmap(fader, 0, 0, 210, 29, position_x, position_y, back_size_x, back_size_y, 0);
	al_draw_scaled_bitmap(visible_bitmap, 0, 0, 10, 29, position_x + pointer_position, position_y, size_x, size_y, 0);
	al_flip_display();
}

int Fader::click(int* mapping_tab)
{
	ALLEGRO_EVENT_QUEUE* temp_queue = al_create_event_queue();
	ALLEGRO_EVENT temp_event;
	al_register_event_source(temp_queue, al_get_mouse_event_source());
	al_wait_for_event(temp_queue, &temp_event);

	int mouse_position_x = temp_event.mouse.x;
	while (temp_event.type != ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		if (temp_event.mouse.x < position_x)
			pointer_position = 0;
		else if (temp_event.mouse.x > position_x + back_size_x - size_x)
			pointer_position = back_size_x - size_x;
		else
			pointer_position = temp_event.mouse.x - position_x;
		mapping_tab[function] = ((float)(pointer_position) / (back_size_x - size_x)) * 100;
		fader_draw();
		al_flip_display();
		al_wait_for_event(temp_queue, &temp_event);
	}
	al_destroy_event_queue(temp_queue);
	return 11;
}

bool Fader::check_position(int mouse_position_x, int mouse_position_y)
{
	if (mouse_position_x >= position_x + pointer_position && mouse_position_x < position_x + size_x + pointer_position)
	{
		if (mouse_position_y >= position_y && mouse_position_y < position_y + size_y)
			return true;
	}
	return false;
}

ComboBox::ComboBox(int pposition_x, int pposition_y, int psize_x, int psize_y)
	:Button(pposition_x, pposition_y, psize_x, psize_y, false)
{
	selected = al_load_bitmap("bitmaps/buttons/selected_combo.bmp");
	unselected = al_load_bitmap("bitmaps/buttons/unselected_combo.bmp");
	visible_bitmap = unselected;
}

int ComboBox::click(int* mapping_tab)
{
	for (int i = 10; i < size_x; i += 1)
	{
		al_draw_scaled_bitmap(visible_bitmap, 0, 0, 188, 43, position_x + i, position_y, size_x, size_y, 0);
		this->draw();
		al_flip_display();
	}
	return 20;
}

bool ComboBox::check_position(int mouse_position_x, int mouse_position_y)
{
	if (mouse_position_x >= position_x && mouse_position_x < position_x + size_x)
	{
		if (mouse_position_y >= position_y && mouse_position_y < position_y + size_y)
			return true;
	}
	return false;
}

cont_list* add_cont(Button* el, cont_list* buttons)
{
	if (!buttons)
	{
		cont_list* new_list = new cont_list();
		new_list->element = el;
		return new_list;
	}
	else
	{
		cont_list* new_list = new cont_list();
		new_list->element = el;
		new_list->next = buttons;
		buttons->prev = new_list;
		return new_list;
	}
}

cont_list* remove_cont(Button* el, cont_list* buttons)
{
	cont_list* new_list = buttons, * ret = buttons;
	while (new_list)
	{
		if (new_list->element == el)
			break;
		else
			new_list = new_list->next;
	}
	cont_list* temp;
	if (new_list->prev)
	{
		temp = new_list->prev;
		temp->next = nullptr;
		if (new_list->next)
		{
			temp->next = new_list->next;
			temp = new_list->next;
			temp->prev = new_list->prev;
		}
		delete new_list->element;
		delete new_list;
		return ret;
	}
	else
	{
		if (!new_list->next)
		{
			delete new_list->element;
			delete new_list;
			return nullptr;
		}
		temp = new_list->next;
		temp->prev = nullptr;
		delete new_list->element;
		delete new_list;
		return temp;
	}
}
