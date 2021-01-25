#include "EventGen.h"

EventGen::EventGen(ALLEGRO_DISPLAY* display)
{
	event_queue = NULL;
	timer = NULL;
	ev_ok = true;
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	//si el event queue o el timer no pudieron ser inicializados error
	if (!event_queue || !timer)	
	{
		ev_ok = false;
		printf("ERROR con event_queue o timer\n");
	}
	else
	{
		//tomo todos los eventos que me interesan.
		al_register_event_source(event_queue, al_get_display_event_source(display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_start_timer(timer); 
	}
}

/*destruyo todo lo creado en esta clase por parte de allegro.*/
EventGen::~EventGen()
{
	if (event_queue)
	{
		al_destroy_event_queue(event_queue);
		event_queue = NULL;
	}
		
	if (timer)
	{
		al_destroy_timer(timer);
		timer = NULL;
	}
				
}

//esta funcion evalua el evento siguiente en la cola de eventos, lo configura en el objeto evento y devuelve dicho evento ya configurado segun el ultimo elemento de la pila.
Evento EventGen::getEvento()
{
	ALLEGRO_EVENT ev;
	al_get_next_event(event_queue, &ev);
	switch (ev.type)
	{
	case ALLEGRO_EVENT_KEY_DOWN:
		evento.setType(KEY_DOWN);
		evento.setKeycode(ev.keyboard.keycode);
		break;
	case ALLEGRO_EVENT_KEY_UP:
		evento.setType(KEY_UP);
		evento.setKeycode(ev.keyboard.keycode);
		break;
	case ALLEGRO_EVENT_TIMER:
		evento.setType(TIMER);
		break;
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		evento.setType(CLOSE);
		break;
	}
	return evento;
}

/*******************************************
*			GETTERS							*
********************************************/

//getter que indica si todo fue bien 
bool EventGen::wasGood(void)
{
	return ev_ok;
}

//Es un getter que retorna el estado de la pila, si hay elemento devuelve true sino false.
bool EventGen::hayEvento()
{
	return !al_is_event_queue_empty(event_queue);
}
