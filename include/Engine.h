#ifndef __ENGINE_H
#define __ENGINE_H

#define SCR_WIDTH 		1280
#define SCR_HEIGHT 		1024
#define BASEPATH 		"/home/offsetzero/m4g/N/"
#define FPS				60

typedef enum E_STATUS{//Engine status modes
	ES_INIT,
	ES_DEINIT,
	ES_MENU,
	ES_GAME,
	ES_SRAM,
	ES_FINISH//Code to exit	
}STATUS;

//Map layer structure
typedef struct s_bglayer* BGLAYER;
typedef struct s_bglayer{
	int offset_x; 	// Horizontal offset
	int size_x; 	// Horizontal size
	int speed_x; 	// Horizontal parallax
	
	int offset_y; 	// Vertical offset
	int size_y; 	// Vertical size
	int speed_y; 	// Vertical parallax
	
	TILESET tileset;	// memory pointer to tile data
	
	int scale;

	int active;
	
	//layer effects
	unsigned int *data;
}bglayer;

//Engine class
typedef struct s_engine
{
	int (*init	  )(void);
	void (*deinit)(void);
	void (*tick)(void);
	int x; //global X
	int y; //global Y
	volatile unsigned int frames;
	volatile unsigned int fps;
	volatile unsigned int peak;
	volatile unsigned int ticks;
	volatile unsigned int ticksb;//Ticks before
	volatile unsigned int redraw;
	
	int selected; //selected tile value
		
	envunit environment;
	RENDERER renderer;
	INPUTDRIVER input;
	STATUS status;
	pt3d geometry;

	int clock[3];//Three different speeds
	SPRITE sprio[5];	//Sprite layers (4 priority levels)(5th level = idle)
	bglayer mprio[4];		//Map layers (4 priority levels)
}engine;
extern engine N;


//Engine macros
#define VWIDTH		N.geometry.x
#define VHEIGHT		N.geometry.y
#define VDEPTH		N.geometry.z
#define WIDTH		size.x
#define HEIGHT		size.y
#define FRAMECOUNT	size.z
#define VPAL		N.renderer->palette
#define ENVCTRL		N.environment
#define VBUFFER		N.renderer->buffer

#endif
