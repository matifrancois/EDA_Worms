#include "Graph.h"

Graph::Graph()
{
	display = NULL;
	eventqueue = NULL;
	all_ok = true;
	//llamamos a una funcion que inicializa e instala las cosas necesarias de allegro y si no pudo inicializarlas lo indica con all_ok
	if (!AllegroInit())
	{
		all_ok = false;
	}
}


/* El destructor de graph se encarga de eliminar todas las imagenes y displays creados, al salir del programa*/
Graph::~Graph()
{
	if (background)
		al_destroy_bitmap(background);	
	if(display)
		al_destroy_display(display);
	for (int i = 0; i < CANT_IMAGES_JUMP; i++)
	{
		if (imJump[i])
			al_destroy_bitmap(imJump[i]);
	}
	for (int i = 0; i < CANT_IMAGES_WALK; i++)
	{
		if (imWalk[i])
			al_destroy_bitmap(imWalk[i]);
	}
}

//es un getter que te devuelve el estado de all_ok
bool Graph::wasGood(void)
{
	return all_ok;
}

//se encarga de instalar e inicializar todas las imagenes y cosas de allegro.
bool Graph::AllegroInit()
{
	if(al_init())
	{
		if (al_init_image_addon())
		{
			if (createDisplay())
			{
				if (loadImages())
				{
					if (al_install_mouse())
					{
						if (al_install_keyboard())
						{
							if (al_init_primitives_addon())
							{
								return true;
							}
							else
								printf("ERROR al_init_primitives_addon");
						}
						else
							printf("ERROR al_instal_keyboard\n");
					}
					else
						printf("ERROR al_install_mouse\n");
				}
				else
					printf("ERROR load Images\n");
			}
			else
				printf("ERROR create display\n");
		}
		else
			printf("ERROR al_init_image_addon\n");
	}
	else
		printf("ERROR al_init\n");
	return false;
}

bool Graph::createDisplay(void)
{
	display = al_create_display(SIZE_SCREEN_X, SIZE_SCREEN_Y);
	if(!display)
		return false;
	return true;
}

/*Esta funcion carga todas las imagenes en memoria */
bool Graph::loadImages(void)
{
	//Cargamos el fondo
	if (!(background = al_load_bitmap("Scenario.png")))				
	{																	
		printf("ERROR loading background");
		return false;
	}
	//imprime la imegen.
	al_draw_scaled_bitmap(background,0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),0 ,0 ,SIZE_SCREEN_X, SIZE_SCREEN_Y ,0);


	//Cargamos las imagenes de salto, para eso en direccion ponemos el nombre de la direccion segun el numero de imagen a cargar.
	string str = "wjump\\wjump-F";
	string direccion;
	for (int i = 0; i < CANT_IMAGES_JUMP; i++)
	{
		direccion = str + to_string(i+1) + ".png";
		imJump[i] = al_load_bitmap(direccion.c_str());
		if (!imJump[i])
		{
			printf("ERROR loading jumping bitmaps\n");
			return false;
		}
	}

	//Cargamos las imagenes de caminata, para eso en direccion ponemos el nombre de la direccion segun el numero de imagen a cargar.
	string direct = "wwalking\\wwalk-F";
	string completeDireccion;
	for (int i = 0; i < CANT_IMAGES_WALK; i++)
	{
		completeDireccion = direct + to_string(i+1) + ".png";
		imWalk[i] = al_load_bitmap(completeDireccion.c_str());
		if (!imWalk[i])
		{
			printf("ERROR loading moving bitmap");
			return false;
		}
	}
	al_flip_display();
	return true;
}

/*En cada refresh (cada 20ms) imprimiremos el estado de los worms*/
void Graph::printState(int state_flag, int tick, double pos_X, double pos_Y, int direccion)
{
	int flag = checkOrientacion(direccion);
	switch (state_flag)
	{
	case STILL_G:											//Tanto quieto o moviendose se imprime una imagen del arreglo wwalk														
		al_draw_bitmap(imWalk[tick], pos_X, pos_Y, flag);		//separamos los trees estados para mayor claridad
		break;

	case WALKING_G:
		al_draw_bitmap(imWalk[tick], pos_X, pos_Y, flag);
		break;

	case PREMOVE_G:
		al_draw_bitmap(imWalk[tick], pos_X, pos_Y, flag);
		break;

	case PREJUMP_G:				//En prejump se imprimiran las primeras 4 imagenes del salto
		al_draw_bitmap(imJump[tick], pos_X, pos_Y, flag);
		break;

	case JUMPING_G:				//En el salto el worm esta estirado por lo que siempre se imprime la misma imagen
		al_draw_bitmap(imJump[5], pos_X, pos_Y, flag);			
		break;

	case LANDING_G:				//En landing se imprimen las ultimas 5 imagenes (se comprime al aterrizar)
		al_draw_bitmap(imJump[tick], pos_X, pos_Y, flag);
		break;
	default:
		break;
	}

}

int Graph::checkOrientacion(int direccion)
{
	if (direccion > 0)		//si va para la derecha
	{
		return ALLEGRO_FLIP_HORIZONTAL;		//damos vuelta la imagen
	}
	else
		return 0;
}