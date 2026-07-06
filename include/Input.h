#ifndef __INPUT_H
#define __INPUT_H

#define KeyStatus(k)	(*N.input->device->k)
#define KeyOff(k)		(*N.input->device->k == KEYOFF)
#define KeyDown(k)		(*N.input->device->k == KEYDOWN)
#define KeyHold(k)		(*N.input->device->k == KEYHOLD)
#define KeyUp(k)		(*N.input->device->k == KEYUP)

enum E_InputCodes
{
	KEYOFF,
	KEYDOWN,
	KEYHOLD,
	KEYUP
};

enum E_KeyCodes
{
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_ESCAPE,
	K_RETURN,
	K_SPACE,
	K_CTRL,
	K_ALT,
	K_SHIFT,
	K_TAB,
	K_UP,
	K_DOWN,
	K_LEFT,
	K_RIGHT,
	K_PGDN,	
	K_PGUP,
	K_END,
	K_HOME,
	K_H
};

typedef struct s_inputdevice* INPUTDEVICE;
typedef struct s_inputdevice
{	
	char name[16];	
	short *A1X;
	short *A1Y;
	short *A2X;
	short *A2Y;
	char *UP;
	char *DOWN;
	char *LEFT;
	char *RIGHT;
	char *X1;
	char *X2;
	char *Y1;
	char *Y2;
	char *A;
	char *B;
	char *X;
	char *Y;
	char *L1;
	char *L2;
	char *L3;
	char *R1;
	char *R2;
	char *R3;
	char *ST;
	char *SL;
}t_inputdevice;

typedef struct s_mouse* MOUSE;
typedef struct s_mouse
{
	char buttoncount;
	char button[8];
	short dx;
	short dy;
	short x;
	short y;
}t_mouse;

typedef struct s_inputdriver* INPUTDRIVER;
typedef struct s_inputdriver
{
	int (*init)(void);
	void (*deinit)(void);
	void (*poll)(void);
		
	MOUSE mouse;
	char *keyboard;
	//JOYSTICK joy;
	INPUTDEVICE device;
	int id; //player id	
}inputdriver;

int CursorOver(int x, int y, int w, int h);
void InputBind(void);
char InputCheckKeyStatus(char dest, int source);

#endif
