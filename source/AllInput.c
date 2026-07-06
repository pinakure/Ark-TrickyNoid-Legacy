#include <Ncommon.h>

#define INPUTDRIVER_STRING  "Allegro 4.2"
inputdriver AllInput;

static void Tick(void)
{
	N.ticks++;

	if( !(N.ticks%FPS)) {
		N.fps = N.frames;
		N.frames = 0;
		N.redraw = 1;
		if(N.fps > N.peak) N.peak = N.fps;
	}
	N.clock[FPS_LOW]++;
	N.clock[FPS_MID]+=2;
	N.clock[FPS_HIGH]+=3;

	
	if(PROFILE_FRAME)if(PROFILE_FRAME->status!=NSTATUS_DEAD)PROFILE_FRAME->redraw = 1;
	if(MAP_FRAME)if(MAP_FRAME->status!=NSTATUS_DEAD)MAP_FRAME->redraw = 1;
}

END_OF_STATIC_FUNCTION(Tick)


static int MouseInit(void)
{
	int i;	
	i = install_mouse();
	if(i<=0)
	{
		Log("\t  No mouse found.\n");
		return 0;
	}
	
	AllInput.mouse = malloc(sizeof(t_mouse));
	
	AllInput.mouse->buttoncount = i;
	Log("\t  Mouse found, %i buttons!\n", i);
	return 1;

}

static int JoystickInit(void)
{
	Log("\t  No joystick found.\n");
	return 0;
}

static int KeyboardInit(void)
{
	if(install_keyboard()!=0)
	{
		Log("\t  No keyboard found.\n");
		return 0;
	}
	
	AllInput.keyboard = malloc(sizeof(char[256]));	
	
	memset(AllInput.keyboard, 0, 256);
	
	Log("\t  Keyboard found!\n");
	return 1;
}

static void MouseDeinit(void)
{
	Log("\t  Release mouse...");	
	if(AllInput.mouse!=NULL)free(AllInput.mouse);
	AllInput.mouse = NULL;
	remove_mouse();	
	
	__OK__
}

static void KeyboardDeinit(void)
{
	Log("\t  Release keyboard...");
	if(AllInput.keyboard!=NULL)free(AllInput.keyboard);
	AllInput.keyboard=NULL;
	remove_keyboard();
	Log("OK!\n");
}

static void JoystickDeinit(void)
{
	Log("\t  Release joystick...");
	Log("OK!\n");
}

#define input AllInput.device
static void AllInputBindDefaults(void)
{
	strcpy(input->name, "Mouse+Keyboard");		
	
	input->B = &AllInput.keyboard[K_SPACE];
	input->A = &AllInput.keyboard[K_PGUP];	
	input->A1X = &AllInput.mouse->x;
	input->A1Y = &AllInput.mouse->y;
	
	input->L1 = &AllInput.mouse->button[0];	
	input->L2 = &AllInput.keyboard[K_PGDN];	
	input->L3 = &AllInput.keyboard[K_F8];	
	input->R1 = &AllInput.mouse->button[1];
	input->R2 = &AllInput.mouse->button[2];	
	input->R3 = &AllInput.keyboard[K_F4];	
	input->SL = &AllInput.keyboard[K_TAB];	
	input->ST = &AllInput.keyboard[K_RETURN];	
	input->X  = &AllInput.keyboard[K_F1];
	input->Y  = &AllInput.keyboard[K_ESCAPE];
	
	input->RIGHT=&AllInput.keyboard[K_RIGHT];
	input->LEFT=&AllInput.keyboard[K_LEFT];
	input->DOWN=&AllInput.keyboard[K_DOWN];
	input->UP  =&AllInput.keyboard[K_UP];
}
#undef input

static void __AllInputDeinit(void)
{
	Log(" > INPUT: Deinitialize driver (%s)...\n", INPUTDRIVER_STRING);
 	
 	MouseDeinit();
	KeyboardDeinit();
	JoystickDeinit();
	
	free(AllInput.device);	

	remove_int(Tick);
	
	remove_timer();		
	
	Log("\t  Deinit complete.\n");
}

static void JoystickPoll(void)
{
	
}

static void MousePoll(void)
{
	AllInput.mouse->button[0] = InputCheckKeyStatus(AllInput.mouse->button[0],mouse_b&1);
	AllInput.mouse->button[1] = InputCheckKeyStatus(AllInput.mouse->button[1],mouse_b&2);
	AllInput.mouse->button[2] = InputCheckKeyStatus(AllInput.mouse->button[2],mouse_b&4);
	AllInput.mouse->button[3] = InputCheckKeyStatus(AllInput.mouse->button[3],mouse_b&8);
	AllInput.mouse->button[4] = InputCheckKeyStatus(AllInput.mouse->button[4],mouse_b&16);
	AllInput.mouse->button[5] = InputCheckKeyStatus(AllInput.mouse->button[5],mouse_b&32);
	AllInput.mouse->button[6] = InputCheckKeyStatus(AllInput.mouse->button[6],mouse_b&64);
	AllInput.mouse->button[7] = InputCheckKeyStatus(AllInput.mouse->button[7],mouse_b&128);
	
	AllInput.mouse->dx = mouse_x - AllInput.mouse->x;
	AllInput.mouse->dy = mouse_y - AllInput.mouse->y;
	AllInput.mouse->x  = mouse_x;
	AllInput.mouse->y  = mouse_y;
}

static void KeyboardPoll(void)
{
	AllInput.keyboard[K_PGDN  ]=InputCheckKeyStatus(AllInput.keyboard[K_PGDN  ],key[KEY_PGDN  ]);
	AllInput.keyboard[K_PGUP  ]=InputCheckKeyStatus(AllInput.keyboard[K_PGUP  ],key[KEY_PGUP  ]);
	AllInput.keyboard[K_END   ]=InputCheckKeyStatus(AllInput.keyboard[K_END   ],key[KEY_END   ]);
	AllInput.keyboard[K_HOME  ]=InputCheckKeyStatus(AllInput.keyboard[K_HOME  ],key[KEY_HOME  ]);
	
	AllInput.keyboard[K_UP    ]=InputCheckKeyStatus(AllInput.keyboard[K_UP    ],key[KEY_UP    ]);
	AllInput.keyboard[K_DOWN  ]=InputCheckKeyStatus(AllInput.keyboard[K_DOWN  ],key[KEY_DOWN  ]);
	AllInput.keyboard[K_LEFT  ]=InputCheckKeyStatus(AllInput.keyboard[K_LEFT  ],key[KEY_LEFT  ]);
	AllInput.keyboard[K_RIGHT ]=InputCheckKeyStatus(AllInput.keyboard[K_RIGHT ],key[KEY_RIGHT ]);
		
	AllInput.keyboard[K_RETURN]=InputCheckKeyStatus(AllInput.keyboard[K_RETURN],key[KEY_ENTER ]);
	AllInput.keyboard[K_ESCAPE]=InputCheckKeyStatus(AllInput.keyboard[K_ESCAPE],key[KEY_ESC   ]);
	AllInput.keyboard[K_SPACE ]=InputCheckKeyStatus(AllInput.keyboard[K_SPACE ],key[KEY_SPACE ]);
	AllInput.keyboard[K_CTRL  ]=InputCheckKeyStatus(AllInput.keyboard[K_CTRL  ],key[KEY_LCONTROL]);
	AllInput.keyboard[K_ALT   ]=InputCheckKeyStatus(AllInput.keyboard[K_ALT   ],key[KEY_ALT   ]);
	AllInput.keyboard[K_SHIFT ]=InputCheckKeyStatus(AllInput.keyboard[K_SHIFT ],key[KEY_LSHIFT]);
	AllInput.keyboard[K_TAB   ]=InputCheckKeyStatus(AllInput.keyboard[K_TAB   ],key[KEY_TAB   ]);
	
	AllInput.keyboard[K_F1    ]=InputCheckKeyStatus(AllInput.keyboard[K_F1    ],key[KEY_F1    ]);
	AllInput.keyboard[K_F2    ]=InputCheckKeyStatus(AllInput.keyboard[K_F2    ],key[KEY_F2    ]);
	AllInput.keyboard[K_F3    ]=InputCheckKeyStatus(AllInput.keyboard[K_F3    ],key[KEY_F3    ]);
	AllInput.keyboard[K_F4    ]=InputCheckKeyStatus(AllInput.keyboard[K_F4    ],key[KEY_F4    ]);
	AllInput.keyboard[K_F5    ]=InputCheckKeyStatus(AllInput.keyboard[K_F5    ],key[KEY_F5    ]);
	AllInput.keyboard[K_F6    ]=InputCheckKeyStatus(AllInput.keyboard[K_F6    ],key[KEY_F6    ]);
	AllInput.keyboard[K_F7    ]=InputCheckKeyStatus(AllInput.keyboard[K_F7    ],key[KEY_F7    ]);
	AllInput.keyboard[K_F8    ]=InputCheckKeyStatus(AllInput.keyboard[K_F8    ],key[KEY_F8    ]);
	AllInput.keyboard[K_F9    ]=InputCheckKeyStatus(AllInput.keyboard[K_F9    ],key[KEY_F9    ]);
	AllInput.keyboard[K_F10   ]=InputCheckKeyStatus(AllInput.keyboard[K_F10   ],key[KEY_F10   ]);
	AllInput.keyboard[K_F11   ]=InputCheckKeyStatus(AllInput.keyboard[K_F11   ],key[KEY_F11   ]);
	AllInput.keyboard[K_F12   ]=InputCheckKeyStatus(AllInput.keyboard[K_F12   ],key[KEY_F12   ]);	
}

static void __AllInputPoll(void)
{
	JoystickPoll();
	MousePoll();
	KeyboardPoll();
}

static int __AllInputInit(void)
{
	int control = 3;//number of device types to look for
	Log(" > INPUT: Initialize driver (%s)...\n", INPUTDRIVER_STRING);
 	
 	Log("\t  Create device interface...");

	AllInput.device = malloc(sizeof(t_inputdevice));
	Log("OK!\n");
 	
 	
 	Log(" > INPUT: Searching for bindable devices...\n");
 	
 	if(!KeyboardInit())control--;
	if(!MouseInit())control--;
	if(!JoystickInit())control--;
	
 	if(!control)
 	{
	 	Log(" > INPUT: FATAL: ACQUIRING FAILED!\n");
	 	exit(666);
 	}
		
 	install_timer();
	install_int_ex(Tick, BPS_TO_TIMER(FPS)); 	
 	
	Log(" > INPUT: Binding engine controls...\n");
 	AllInputBindDefaults();
 	
	Log(" > INPUT: Init complete.\n");
 	return 1;
}


void InputBind(void)
{
	N.input = &AllInput;
	N.input->init = __AllInputInit;
	AllInput.init = __AllInputInit;
	AllInput.deinit = __AllInputDeinit;
	AllInput.poll = __AllInputPoll;	
}

