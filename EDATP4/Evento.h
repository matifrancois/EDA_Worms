#ifndef EVENTO_H
#define EVENTO_H

class Evento
{
public:
	Evento();
	void setType(int type);
	void setKeycode(int keycode);
	int getKey(void);
	int getType(void);
private:
	int type;
	int keycode;
};

#endif