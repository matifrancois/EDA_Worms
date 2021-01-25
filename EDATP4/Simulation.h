#ifndef SIMULATION_H
#define SIMULATION_H

#include "Prototypes.h"
#include "Worm.h"
#include "Graph.h"

class Simulation : public Worm
{
public:
	/****************
	*  Constructor  *
	*****************/
	Simulation();		

	/************************
	*   Funciones publicas	*
	*************************/
	//startMoving: Recibe evento de tecla presionada y lo lleva a la maquina de estados de los worms
	void startMoving(int, Worm*);	
	//stopMoviong: Recive evento de tecla levantada y lo lleva a la maquina de estados de los worms
	void stopMoving(int, Worm*);
	//refresh: avanza un tick en simulacion cada 0,02 segundos e imprime en display la situacion actual
	void refresh(Worm*, Graph*);
	//quit: finaliza simulacion cuando se cierra display
	void quit(void);
	//running: devuelve la simulacion continua corriendo 
	bool running(void);

private:
	bool continueRunning;
};

#endif //SIMULATION_H