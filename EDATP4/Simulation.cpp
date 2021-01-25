#include "Simulation.h"

Simulation::Simulation()
{
	continueRunning = true;
}

void Simulation::startMoving(int keycode, Worm* wormArray)
{
	int i;
	for (i = 0; i < NUMBER_OF_WORMS ; i++)
	{
		wormArray[i].fsm(keycode,DOWN);
	}
	
}

void Simulation::stopMoving(int keycode,Worm* wormArray)
{
	int i;
	for (i = 0; i < NUMBER_OF_WORMS; i++)
	{
		wormArray[i].fsm(keycode,UP);
	}

}

void Simulation::refresh(Worm * wormArray, Graph* grapher)
{
	int i;
	for (i = 0; i < NUMBER_OF_WORMS; i++)
	{
		wormArray[i].WormNewFrame();
		wormArray[i].fsm(REFRESH, DOWN);
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap(grapher->background, 0, 0, al_get_bitmap_width(grapher->background), al_get_bitmap_height(grapher->background), 0, 0, SIZE_SCREEN_X, SIZE_SCREEN_Y, 0);
	for (i = 0; i < NUMBER_OF_WORMS; i++)
	{
		grapher->printState(wormArray[i].getState(), wormArray[i].getTick(), wormArray[i].getPosX(), wormArray[i].getPosY(), wormArray[i].getDireccion());
	}
	al_flip_display();

}

void Simulation::quit(void)
{
	printf("QUIT\n");
	continueRunning = false;
}

bool Simulation::running(void)
{
	return continueRunning;
}