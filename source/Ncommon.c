#include <Ncommon.h>

engine N;
TILESET CURSOR;
/* Dummy functions */
void __void0(void){	}
void __void1(int _1){_1++;}
void __void2(int _1, int _2){_1++;_2++;}
void __void3(int _1, int _2, int _3){_1++;_2++;_3++;}
int __int0(void){return 1;}
int __int1(int _1){return _1;}
int __int2(int _1, int _2){return _1|_2;}
int __int3(int _1, int _2, int _3){return _1|_2|_3;}

// randomize buffers
static int RND[256];
static int RND3[256];

int GetRand3(void)
{
	static int index=0;

	index++;
	index&=0xFF;

	return RND3[index];
}

int GetRand(void)
{
	static int index=0;

	index++;
	index&=0xFF;

	return RND[index];
}

static void RandInit(void)
{
	int i;

	srand(time(NULL));

	for(i=0; i<256; i++)
	{
		RND[i] = rand();
		RND3[i] = rand()%3;
		i++;
	}
}

/* DEINIT */

static void DeinitLayers(void)
{
	int i;
	Log(" > NGINE: Deinitialize layers...\n");
	for(i=0; i<4; i++)
	{
		free(N.mprio[i].data);
	}
	Log("\t  Deinit complete.\n");
}

static void __Deinit(void)
{
	Log("[ Deinitializing N ]-----------------------------------------------------------\n");

	DeinitLayers();
	DeinitSprites();

	DestroyTileset(N.mprio[0].tileset);
	DestroyTileset(N.mprio[1].tileset);
	DestroyTileset(N.mprio[2].tileset);
	DestroyTileset(N.mprio[3].tileset);
	
	DeinitAsset();
	
	MapicDeinit();
	
	NGDeinit();//deinit GUI
	
		
	Log(" >   GUI: Release cursor graphics...\n");	
	DestroyTileset(CURSOR);
	
	DeinitGlyphTable();
	
	N.renderer->deinit();
	N.input->deinit();
	#ifndef NDEBUG
		Log(" >  PROF: Deinitialize...\n");
		DeinitProfiler();
		Log("\t  Deinit complete.\n");
	#endif
	
	Log("\n[ Program terminated ]---------------------------------------------------------\n");
}

/* INIT */

static int InitLayer(int i /*index*/, int x, int y, int sx, int sy /*speed*/)
{
	#define bg N.mprio[i]
	int size;
	
	Log("%i...", i);

	
	bg.size_x = x;
	bg.size_y = y;

	bg.offset_x = 0;
	bg.offset_y = 0;

	bg.speed_x = sx;
	bg.speed_y = sy;
	
	bg.tileset = NULL;
	bg.scale = 1;
	
	size = x*y;

	bg.data = malloc(sizeof(int)*size);
		
	if(!bg.data)
	{
		__FAIL__
		return 0;
	}
	
	memset(bg.data, 0, size);
	return 1;
	#undef bg
}

static int InitLayers(int sx1, int sy1, int sx2, int sy2, int sx3, int sy3, int sx4, int sy4)
{
	Log(" > NGINE: Initialize layers...");

	/*reset global position*/
	N.x = 0;
	N.y = 0;
	
	InitLayer(0, sx1, sy1, 8, 8);	
	InitLayer(1, sx2, sy2, 8, 8);
	InitLayer(2, sx3, sy3, 8, 8);	
	InitLayer(3, sx4, sy4, 8, 8);
	
	Log("\n\t  Init complete.\n");
	return 1;
}



static int __Init(void)
{
	allegro_init();
	RandInit();

	N.frames= 0;
	N.fps 	= 0;
	N.peak  = 0;
	N.ticks = 0;
	N.ticksb= 0;
	N.redraw= 0;

	N.status = ES_INIT;
	N.geometry.x = SCR_WIDTH - 8;
	N.geometry.y = SCR_HEIGHT-24;
	N.geometry.z = 32;

	N.sprio[0] = NULL;
	N.sprio[1] = NULL;
	N.sprio[2] = NULL;
	N.sprio[3] = NULL;
	N.sprio[4] = NULL;

	N.clock[0] = 0;
	N.clock[1] = 0;
	N.clock[2] = 0;
	
	if(!N.input->init())return 0;
	
	InitAsset("mapic.pak");

	if(!N.renderer->init())return 0;
	InitPalette();

	
	InitLayers(32, 32, 64, 32, 4096, 32, 32, 64);
	
	
	N.renderer->setfont(FONT_HUD);
	
	Log(" >   GUI: Load cursor graphics...\n");
	CURSOR = N.renderer->loadtiles(ICON_CURSOR);
		
	InitProfiler();

	NGInit();//init GUI

	MapicInit(); //init MAPIC
		
	N.mprio[0].tileset = N.renderer->loadtiles(ICON_SPRICT);;
	N.mprio[1].tileset = N.mprio[0].tileset;
	N.mprio[2].tileset = N.mprio[0].tileset;
	N.mprio[3].tileset = N.mprio[0].tileset;
	
	N.mprio[0].active = 0;
	N.mprio[1].active = 0;
	N.mprio[2].active = 0;
	N.mprio[3].active = 0;
	
	N.selected = 0;
	return 1;
}

char *concat(char *str1, char *str2)
{
	static char buffer[1024];
	memset(buffer, 0, 1024);

	strcpy(buffer, str1);
	strcat(buffer, str2);

	return buffer;
}

static void __Tick(void)
{
	//N.ticksb = N.ticks;//tick clocks
				
	MapTick();
	NGTick();
	MapicTick();//Custom Call
	
	
	/*PROFILE INPUT*/
	StartProfiler(PROFILE_INPUTPOLL);
	if(KeyUp(R3))
	{
		Log(" > NGINE: Exitting main loop by user request\n");
		N.status = ES_FINISH;//DEBUG Exit method
	}
	N.input->poll();
	StopProfiler(PROFILE_INPUTPOLL);
	
	TickProfiler();
}

void Init(void)
{
	Log("\n------------------------------------");
	Log("\n ###\\   M4G33K(TM) Corporation");
	Log("\n # \\# · 2012 - 20xx");
	Log("\n #  #   N·Engine is copyright, 2012.");
	Log("\n------------------------------------");
	Log("\nInitializing N-----------------------------------------------------------------\n");	
	
	Log(" > NGINE: Binding hooks...");
	N.init = __Init;
	N.deinit = __Deinit;
	N.tick = __Tick;	
	
	Log("Render...");
	RendererBind();//Replace this call for whatever renderer you want to use (remember to update makefile too)
	
	Log("Input...");
	InputBind();//Replace this call for whatever renderer you want to use (remember to update makefile too)
	__OK__
}
