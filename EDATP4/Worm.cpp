#include "Worm.h"

Worm::Worm()
{
	state = STILL;
	prevState = STILL;
	pos.x = 0;
	pos.prev_x = 0;
	pos.y = 0;
	pos.dxdt = 0;
	pos.dxdtdt = 0;
	pos.wormDir = 0;
	keyJump = '0';
	keyLeft = '0';
	keyRight = '0';
	ticks = 0;
	preWalkticks = 0;
	iveBeenWalking4 = 0;
	iveBeenJumping4 = 0;
}

Worm::Worm(char keyJump_, char keyLeft_, char keyRight_)
{
	pos.x = MIN_POSITION_X + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX) / (MAX_POSITION_X - MIN_POSITION_X));
	pos.prev_x = pos.x;
	pos.y = MIN_POSITION_Y; 
	keyJump = keyJump_;
	keyLeft = keyLeft_;
	keyRight = keyRight_;
	pos.dxdt = (float)0.54;			//Se mueve de a 27 pixeles por segundo -> 0,54 pixeles por 0,02 segundos (refresh)
	pos.dxdtdt = (float)ACELERACION;				//Aceleracion por fuerza de gravedad es positiva por los ejes que impone alegro
	pos.wormDir = 0;
	pos.angle = PI / 3;				//60 grados
	state = STILL;
	prevState = STILL;
	ticks = 0;
	preWalkticks= 0; 
	iveBeenWalking4 = 0;
	iveBeenJumping4 = 0;
}


/*******************************************
*			GETTERS							*
********************************************/
unsigned int Worm::getState(void) { return state; }
unsigned int Worm::getTick(void) { return ticks; }
double Worm::getPosX(void) {return pos.x;}
double Worm::getPosY(void) { return pos.y; }
int Worm::getDireccion(void) { return pos.wormDir; }

/************************
*   Funciones publicas	*
*************************/
void Worm::fsm(int keycode, int UP_OR_DOWN)
{
	EVENTO key_evento = turn_keycode_to_key(keycode);
	if (key_evento!=-1 && UP_OR_DOWN == DOWN)			//En este caso tambien puede llegar a ser key refresh, ya que tomamos a este evento como un key down 
	{
		switch (this->state)
		{
		case STILL:
			printf("STILL\n");
			if (key_evento == KEYJUMP)
			{
				this->ticks = 0;
				this->pos.dxdt = 4.5;
				this->state = PREJUMP;
				WormPreJump();

			}
			else if (key_evento == KEYLEFT || key_evento == KEYRIGHT)
			{
				if (key_evento == KEYLEFT)
				{
					this->pos.wormDir = LEFT;
				}
				else if (key_evento == KEYRIGHT)
				{
					this->pos.wormDir = RIGHT;
				}
				WormPreWalk();				//en preWalk set timer to 0 y camibio a estado premove
			}
			else if(key_evento == REFRESH)
				this->ticks = 0;		//Nada se mueve
			break;

		case PREJUMP:
			WormPreJump();
			break;

		case JUMPING:
			//Se ingora la tecla si se aprieta denuevo
			//Se ignoran todas las teclas si estamos saltando ya que hasta
			//no aterrizar el estado de jumping no hace nada mas que saltar
			WormJump();
			break;

		case LANDING:
			WormLanding();
			break;

		case PREWALK:
			if (key_evento == KEYJUMP)
			{
				this->ticks = 0;
				this->pos.dxdt = 4.5;
				this->state = PREJUMP;
				WormPreJump();		//Si estaba por moverse pero salto, cambio de estado a saltar
			}
			else if (key_evento == REFRESH || key_evento == KEYLEFT || key_evento == KEYRIGHT)
			{
				printf("PRE WALK\n");

				if (this->preWalkticks >= 4)				//Cada tick vale 0,02 segundos --> 5 ticks son 0,1 segundos (ticks empiezan en 0)
				{
					this->state = WALKING;
					this->ticks = 0;
					this->preWalkticks = 0;
				}

			}
			break;

		case WALKING:
			if (key_evento == KEYJUMP)
			{
				this->ticks = 0;
				this->pos.dxdt = 4.5;
				this->state = PREJUMP; 
				WormPreJump();
			}
			else if (key_evento == KEYLEFT || key_evento == KEYRIGHT || key_evento == REFRESH)
			{
				WormWalk();					//En worm walk vemos: -si tenemos q rotar o no 		
				if (this->iveBeenWalking4 >= 45)			//Cada tick mientras camina incrementa en 0,02 segundos, 45 ticks son 0,9 segunods
				{
					this->preWalkticks += 1;			//Los ticks que cuentan tiempo para decididir si seguimos caminando o no					
				}
				if (iveBeenWalking4 >= 50)			//Si es mayor o igual a 1000 ms significa que ya paso el tiempo de pregunta (900ms) y ya verifico 
				{										//si la flecha estuvo apretada el tiempo necesario para moverse (100ms)
					this->iveBeenWalking4 = 0;											//por lo que tengo que volver a esperar 900ms
				}
			}
			break;

		default:
			break;
		}
	}

	if (key_evento != -1 && UP_OR_DOWN == UP)
	{
		switch (this->state)
		{
		case WALKING:
			if (key_evento == KEYLEFT || key_evento == KEYRIGHT)	 //Si estoy en walking solo me importa si se movieron estas dos teclas
			{
				if (this->iveBeenWalking4 < 45)					//Si se levanto antes de los 0.9 milisegundos
				{
					this->state = STILL;			//vuelve a estar quieto con la misma direccion q antes
				}
				if (this->iveBeenWalking4 > 45)
				{
					WormPreWalk();
				}
				this->iveBeenWalking4 = 0;			//IveBeenWalking4 vuelve a cero cvuando se levanta la tecla de caminar
			}
			break;

		case PREWALK:
			if (key_evento == KEYLEFT || key_evento == KEYRIGHT)	//Si estoy en prewalk solo me importa si se movieron estas dos teclas
			{
				this->preWalkticks = 0;			//descarto la cantidad de ticks q espere
				this->state = STILL;				//vuelvo a estar quieta con la misma direccion que antes
				if ((this->pos.wormDir == LEFT && key_evento == KEYRIGHT) || (this->pos.wormDir == RIGHT && key_evento == KEYLEFT))
				{
					this->pos.wormDir *= -1;   //Lo roto
				}
			}
			break;

		default:
			break;
		}
	}
}

void Worm::WormNewFrame()
{
	if (this->state == PREWALK)
	{
		this->preWalkticks += 1;
	}
	if (this->state == JUMPING || this->state == PREJUMP)
	{
		this->iveBeenJumping4 += 1;
	}

	this->ticks += 1;
}


/************************
*   funciones privadas	*
*************************/
void Worm::WormPreJump()
{
	if (this->ticks >= 0 && this->ticks <= 4)
	{
		WormJumpCalculations();
		printf("PRE JUMP, tick:%u, y:%f \n", this->ticks, this->pos.y);
	}
	else //Go to jump
	{
		this->state = JUMPING;
		printf("END PREJUMP\n");
	}
}

void Worm::WormJump() //Update caida
						
{
	if (this->pos.y <= MIN_POSITION_Y)
	{
		WormJumpCalculations();
		printf("JUMP, tick ive been jumping:%u,x:%f  y:%f \n",this->iveBeenJumping4, this->pos.x, this->pos.y);
	}
	else //Go to landing
	{
		this->ticks = 6;
		this->iveBeenJumping4 = 0;
		this->state = LANDING;
		this->pos.y = MIN_POSITION_Y;
		printf("END JUMP\n");
	}
}

void Worm::WormJumpCalculations()
{
	if (check4motion())		//si esta en el borde salta para arriba  o cae
	{
		this->pos.wormDir = 0;
	}

	this->pos.x = this->pos.prev_x + this->pos.wormDir * cos(pos.angle) * this->pos.dxdt;			//4,5 es la velocidad de salto 
	this->pos.prev_x = this->pos.x;
	if (this->pos.y <= MIN_POSITION_Y)		//Eje en alegro es alreves por eso si es mas chico q base esta saltando
	{
		this->pos.y = MIN_POSITION_Y - this->pos.dxdt * sin(pos.angle) * this->iveBeenJumping4 + this->pos.dxdtdt * 0.5 * this->iveBeenJumping4 * this->iveBeenJumping4;
	}								//velocidad en y es negativa
}
void Worm::WormLanding()
{
	if (this->ticks > 5 && this->ticks <= 10)
	{
		printf("LANDING, tick:%u, y:%f \n", this->ticks, this->pos.y);
	}
	else //Go to still
	{
		this->ticks = 0;
		this->state = STILL;
		this->pos.dxdt = 0.54;
		this->pos.y = MIN_POSITION_Y;
		printf("END JUMP\n");
	}
	
}

void Worm::WormWalk()			//Aca verificar si tengo q cambiar sentido de direccion o no
{
	if (this->ticks >= CANT_IMAGES_WALK)
	{
		this->ticks = 3;
	}

	if (check4motion())
	{
		this->pos.wormDir *= -1;
	}

	this->pos.x = this->pos.prev_x + this->pos.wormDir * this->pos.dxdt;
	this->pos.prev_x = this->pos.x;

	printf("WALK, ticks:%u, %f\n",this->ticks, this->pos.x);
	this->iveBeenWalking4 += 1;
}

void Worm::WormPreWalk()
{
	if (check4motion())
	{
		this->pos.wormDir *= -1;
	}

	this->state = PREWALK;
	this->ticks = 0;
	this->preWalkticks = 0;
	//SET TIMER TO 0 
}

int Worm::turn_keycode_to_key(int keycode)
{
	int key = -1;
	if (keycode == keyJump)
		key = KEYJUMP;
	else if (keycode == keyLeft)
		key = KEYLEFT;
	else if (keycode == keyRight)
		key = KEYRIGHT;
	else if (keycode == REFRESH)
		key = REFRESH;

	return key;
}

bool Worm::check4motion()
{
	bool isOk = false;			//Si devuelve false NO tenemos q cambiar direccion de worm
	if (this->pos.wormDir == RIGHT && this->pos.x > MAX_POSITION_X)
		isOk = true;
	else if (this->pos.wormDir == LEFT && this->pos.x < MIN_POSITION_X)
		isOk = true;
	else if (this->pos.wormDir == 0)
		isOk = true;

	return isOk;
}

