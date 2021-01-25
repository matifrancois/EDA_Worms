#ifndef EVENTGEN_H
#define EVENTGEN_H

#include<stdio.h>
#include <allegro5/allegro.h>
#include "Evento.h"
#include "Prototypes.h"

class EventGen
{
public:
	EventGen(ALLEGRO_DISPLAY* display);
	~EventGen();
	bool hayEvento(void);
	Evento getEvento(void);
	bool wasGood(void);


private:
	
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_TIMER* timer;
	//ALLEGRO_EVENT* ev;
	Evento evento;
	int type;
	bool ev_ok;
};

#endif  //EVENTOGEN_H
