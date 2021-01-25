#include <stdio.h>
#include <iostream>
#include "Prototypes.h"
#include "EventGen.h"
#include "Simulation.h"
#include "Graph.h"
#include "Worm.h"

//Funciones de main
bool initialize(Graph** grapher, EventGen** eventgen);
void deinit(Graph** grapher, EventGen** eventgen);
void dispatch(Evento evento, Simulation* sim, Worm* wormArray, Graph* grapher);

int main(void)
{
	//creo los punteros a los objetos y los objetos en si/
	Graph* grapher = NULL;
	EventGen* eventGen = NULL;
	Simulation sim;
	Worm worm[NUMBER_OF_WORMS];

	//inicializo los comandos para cada worm creado, si se quisiera trabajar con mas worms aqui deberian inicializarse tambien con dif teclas
	worm[0] = Worm(ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT);
	worm[1] = Worm(ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_D);

	//inicializo todas las cosas de allegro
	if (!(initialize(&grapher, &eventGen)))
	{
		printf("Error de inicializacion");
		return -1;
	}
	//siempre que no se detecte el evento de quit (al presionar x en el display)
	while (sim.running())
	{
		if (eventGen->hayEvento())
		{
			// dispatch llama a la funcion correspondiente de sim segun el tipo de evento que recibe
			dispatch(eventGen->getEvento(), &sim,worm, grapher); 
		}
	}
	//borra todas las cosas de alegro relacionadas a la parte grapher y a la parte eventGen
	deinit(&grapher, &eventGen);
	return 0;
}


/*Esta funcion se encarga de inicializar allegro tanto la parte grafica como la parte de eventos,
se le pasa un puntero doble ya que quiero modificar la variable a la que apunta el puntero*/
bool initialize(Graph** grapher,EventGen ** eventGen)
{

	*grapher = new Graph();
	if (!*grapher)
		return false;
	*eventGen = new EventGen((*grapher)->display);
	if (!*eventGen)
		return false;

	//llamo a las funciones que corroboran que en la inicializacion de los componentes todo estuvo bien
	if (!(*grapher)->wasGood() || !(*eventGen)->wasGood())
	{
		return false;
	}
	return true;
}

/*Esta funcion se encarga de borrar todas las cosas inicializadas por allegro*/
void deinit(Graph** grapher, EventGen** eventgen)
{
	if (*eventgen)
	{
		delete* eventgen;
		*eventgen = NULL;
	}
	if (*grapher)
	{
		delete* grapher;
		*grapher = NULL;
	}
}

/*Esta funcion se encarga de llamar a la funcion correspondiente de sis segun el tipo de evento que recibe de la cola de eventos */
void dispatch(Evento evento, Simulation* sim, Worm* wormArray, Graph* grapher)
{
		switch (evento.getType())
		{
			case KEY_DOWN:
				sim->startMoving(evento.getKey(),wormArray);		
			break;

			case KEY_UP:
				sim->stopMoving(evento.getKey(),wormArray);
			break;

			case TIMER:
				sim->refresh(wormArray, grapher);	
			break;

			case CLOSE:
				sim->quit();
			break;
		}
}






















/*notas de la clase*/

/*
init();
while(simulation.running())
{
	if(eventGen.hayEvento())
	{
		dispatch(eventGen.getEvento(),simulation);
	}

}
deinit();

hayEvento()
{
	keyboard.keycode.key 

	return al_getNextEvent(&event.eventQueue);
}

dispatch()
{
	switch(evento)
	case AL_KEY_DOWN :
		simulate.StartMoving(evento.getkey());		//clase evento
		break;

	case AL_KEY_UP:
		simulate.stopMoving(evento.getKey());
		break;

	case ALLEGRO_TIMER:
		simulate.refresh();				//aca guardo timer. despende del estado del worm actualizo movimienot
		break;

	case ALLEGROQUIT?
		simulate.Quit(event.getKey());
		break;

}

Simulation::startMoving(int )
{
	//tengo que primero ver que paso el tiempo, el tiempo es inherente a cada worm
	for(i=0 < i<wormsArr.count() ; i++)
		wormsArr[i].move(key);	

	//donde acumulo tiempo para saber que tengo que seguir moviendo poruqe no 
	//llego ningun key up: (como cuento esos 5 frames) 
		// --> a cada 20ms se llama a refresh, a la quintaq vexz q llaman me muevo (cambio mi estado) 
		//armar mini fsm en cada worm

	//tiempo empieza a contarse cuando recibis keydown. 

}


que worm herede de clase graphic.

CLASES:
	EVENTO 
	-GRAFICA [ine]	
	-WORMS	//arma mini fsm. usa funciones de grafica dependiendo de su estado anterior y timer
	-SIMULAITION [bren]

*/
