#ifndef GRAPH_H
#define GRAPH_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Prototypes.h"
#include <string>


using namespace std;
enum graphStates { STILL_G, JUMPING_G, WALKING_G, PREMOVE_G, LANDING_G, PREJUMP_G, MAXSTATES_G };

class Graph
{
public:
	Graph();
	~Graph();
	bool wasGood(void);
	bool AllegroInit(void);
	void printState(int state_flag, int tick, double pos_X, double pos_Y, int direccion);
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* background;

protected:
	
	ALLEGRO_EVENT_QUEUE* eventqueue;
	ALLEGRO_BITMAP* imJump[CANT_IMAGES_JUMP];
	ALLEGRO_BITMAP* imWalk[CANT_IMAGES_WALK];
	bool all_ok;
	bool createDisplay(void);
	bool loadImages(void);

private:
	int checkOrientacion(int direccion);
};

#endif //GRAPH_H
