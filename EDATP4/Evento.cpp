#include "Evento.h"

Evento::Evento()
{
	type = 0;
	keycode = 0;
}

/*******************************************
*			SETTERS						*
********************************************/
void Evento::setType(int type)
{
	this->type = type;
}

void Evento::setKeycode(int keycode)
{
	this->keycode = keycode;
}

/*******************************************
*			GETTERS							*
********************************************/
int Evento::getKey(void)
{
	return keycode;
}

int Evento::getType(void)
{
	return type;
}