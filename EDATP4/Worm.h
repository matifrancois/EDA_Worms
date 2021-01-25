#ifndef WORM_H
#define WORM_H

#include "Prototypes.h"
#define LEFT -1
#define RIGHT 1
#define PI 3.14159265

typedef struct
{
	double x;
	double y;
	double prev_x;
	float dxdt;		//velocidad
	float dxdtdt;			//aceleracion
	double wormDir;				//direccion
	double angle;
}position_t;

typedef unsigned int EVENTO;
typedef unsigned int ESTADO;

enum States {STILL, JUMPING, WALKING, PREWALK, LANDING, PREJUMP};
enum Keys {KEYJUMP, KEYLEFT, KEYRIGHT, REFRESH, NOKEY=-1};
#define UP 1
#define DOWN 2

class Worm
{
public:
	/****************
	*  Constructores*
	*****************/
	Worm();
	Worm(char keyup, char keyleft, char keyright);			//El constructor recibe los codigos para las tres teclas 
										//que usara el worm.

	/************************
	*   Funciones publicas	*
	*************************/
	void fsm(int newevent, int UP_OR_DOWN);		//la maquina de estados
	void WormNewFrame();		//el actualizador de estados que debe ser llamado por refresh

	/****************
	*   getters		*
	*****************/
	unsigned int getState(void);		//devuelve estado de worm: STILL, JUMPING, WALKING, PREWALK, LANDING o PREJUMP
	unsigned int getTick(void);			//devuelve ticks de worm
	double getPosX(void);				//devuelve posicion en x 
	double getPosY(void);				//devuelve posicion en y
	int getDireccion(void);				//devuelve direccion: LEFT, RIGHT o 0 (no direccion)

protected:
	unsigned int state;
	unsigned int prevState;
	char keyJump;
	char keyLeft;
	char keyRight;
	position_t pos;
	unsigned int ticks;


private:
/************************
*   funciones de worms	*
*************************/
	//WormPreJump: Verifica direccion del salto, comienza a saltar hasta estar el worm estirado en posicion de salto
				//cambia estado a JUMPING cuando termina etapa de despegue.
	void WormPreJump();
	//WormJump: Ya estando en el aire actualiza movimiento del worm en salto vertical o en tiro oblicuo
				//cambia estado a LANDING cuando termino de saltar.
	void WormJump();
	//WormLanding: Espera hasta terminar de aterrizar en 5 ticks. 
				//cambia estado a STILL cuando termino de aterrizar.
	void WormLanding();
	//WormWalk: Worm camina. 
				//No cambia estado.
	void WormWalk();
	//WormPreWalk: actualiza direccion de movimiento e inicia contador de preWalkTicks. 
				//Cambia estado a PREWALK.
	void WormPreWalk();

	//turn_keycode_to_key: traduce la tecla evento recivido a las teclas del worm. 
				//devuelve -1 si tecla evento no pertenece al conjunto teclas del worm.
				// devuelve codigo de tecla si pertenece al conjunto. Codigos: KEYJUMP KEYLEFT KEYRIGHT
	int turn_keycode_to_key(int keycode);
	//check4motion: verifica si se debe rotar al worm al llegar a los limites de movimiento en eje x
				//devuelve true si debemos cambiar direccion del worm
	bool check4motion(void);
	//WormPhysics: calcula el tiro oblicuo del worm. Se llama en WormPreJump y WormJump
	void WormJumpCalculations(void);

/************************
*  variables privadas	*
*************************/
	unsigned int preWalkticks;			//contador pre caminata. Lo incrementa WormNewFrame() y lo analiza fsm(). Si llega a 4 comienza a moverse worm.
	unsigned int iveBeenWalking4;		//contador druante caminata. Verifica los segundos que se mantiene presionada tecla durante caminata.
	unsigned int iveBeenJumping4;		//contador durante salto para calcular posicion de worm en tiro oblicuo. 
};

#endif


/*
//TABLAS DE ESTADOS

//MAQUINA DE ESTADOS CON CADA RUTINA DE ACCION
	STATE array[5][5] =
	{
		//state: STILL  [0]
		{	{KEYJUMP,JUMPING,WormJump},
			{KEYLEFT,PREMOVE,this->WormPreWalk},
			{KEYRIGHT,PREMOVE,&(this->WormPreWalk)},
			{NOKEY, STILL, &(WormNewFrame)},

			{FIN, STILL, WormNewFrame} //sigue en mismo estado solo actualiza el frame
		}
		//state: JUMPING
		{	{KEYJUMP,JUMPING,WormJump},
			{KEYLEFT, JUMPING,WormJump},
			{KEYRIGHT, JUMPING, &(this->WormJump)},
			{NOKEY, JUMPING, &(this->WormNewFrame)},  //sigue en mismo estado solo actualiza el frame
			{FIN, JUMPING, &(this->WormNewFrame)}
		}
		//state: WALKING
		{
			{KEYJUMP,JUMPING,& (this->WormJump)},
			{KEYLEFT,WALKING, &(this->WormWalk)},	//wormwalk verifica si hay q cambiar orientacion
			{KEYRIGHT,WALKING, &(this->WormWalk)},
			{NOKEY,STILL, &(this->WormStop)},
			{FIN,WALKING, &(this->WormNewFrame)}		//sigue en mismo estado solo actualiza el frame
		}
		//state: PREMOVE
		{
			{KEYJUMP,JUMPING,& (this->WormJump)},
			{KEYLEFT, WALKING, &(this->WormWalk)},			//WormWalk verifica si pasaron los frames
			{KEYRIGHT, WALKING, &(this->WormWalk)},				//Si no pasaron todavcia devuelve estado PREMOVE
			{NOKEY, STILL, &(this->WormStop)},
			{FIN, PREMOVE, &(this->WormNewFrame)} 		//sigue en mismo estado solo actualiza el frame
		}
		//state: LANDING
		{	{KEYJUMP,LANDING,& (this->nothing2do)},			//Si estoy aterrizando y vuelvo a saltar espero a terminar de aterrizar
			{KEYLEFT, LANDING, &(this->nothing2do)},
			{KEYRIGHT, e_landing, &(this->nothing2do)},
			{NOKEY, LANDING, &(this->nothing2do)},
			{FIN, LANDING, &(this->WormNewFrame)}
		}

	}
	
intento de maquina

ESTADO Worm::StateMachine(ESTADO estado_actual, EVENTO evento_actual)
{
	//Recorro tablas de estado hasta encontrar rutina de accion

	while (p_tabla_estado->evento != evento_actual && p_tabla_estado->evento != FIN)
		++p_tabla_estado;

	//Cuando ya encuentro lo que tengo que hacer

	(*p_tabla_estado->p_rut_accion)();	//ejecuto accion

	p_tabla_estado= p_tabla_estado->proximoEstado;
}
*/