#include <Ncommon.h>

WIDGET PROFILE_WINDOW;
WIDGET MAP_WINDOW;

WIDGET PROFILE_FRAME;
WIDGET MAP_FRAME;

WIDGET TILES_FRAME;
WIDGET SPRITE_FRAME;
WIDGET TOOLBAR;


static void __DATATick_Tileset(void);
static void(*DATATICK)(void) = __DATATick_Tileset;


static void MapicOpenSprite(int param)
{
	Log(" >BUTTON: Opening sprite #%i...\n",param);
}

void MapicInit(void)
{
	TOOLBARWIDGETS 	tw;
	GUIWIDGETS 		gw = GetWidgets(GUIWIDGETS,GUI);
	BUTTONWIDGETS	bw;
	WIDGET 			w;
	WIDGET 			*obutton;
	int i;
	
	Log(">TOOLBAR: Initialize...\n");
	
	gw->toolbar = CreateWidget(TYPE_TOOLBAR, 0, 0, GUI->w, BUTTON_SIZE+4, "Toolbar", GUI, gw);
	
	tw = GetWidgets(TOOLBARWIDGETS, gw->toolbar);
	
	tw -> count = TOOLBAR_MAX;
	tw -> icons = N.renderer->loadtiles(ICON_BUTTONS);
	tw -> button = malloc(sizeof(WIDGET)*tw->count);
	
	/* Create first button */
	*tw->button = CreateWidget(TYPE_BUTTON, 2, 2, BUTTON_SIZE, BUTTON_SIZE, NULL, gw->toolbar, MapicOpenSprite);
	w = *tw->button;
	/* Obtain pointer to structure buttonwidgets, generated in each button automatically */
	bw = GetWidgets(BUTTONWIDGETS, w);

	/* Blit icon to three button states (first button) */
	masked_stretch_blit(tw->icons->tile[0], bw->image[0],0,0,32,32,0,0,BUTTON_SIZE, BUTTON_SIZE);
	masked_stretch_blit(tw->icons->tile[0], bw->image[1],0,0,32,32,0,0,BUTTON_SIZE, BUTTON_SIZE);
	masked_stretch_blit(tw->icons->tile[0], bw->image[2],0,0,32,32,0,0,BUTTON_SIZE, BUTTON_SIZE);
	bw->value = 0;
	for(i=1;i<tw->count;i++)
	{
		tw->button++;
		
		/* Create button... */
		*tw->button = CreateWidget(TYPE_BUTTON,2+(i *BUTTON_SIZE),2,
								  BUTTON_SIZE,BUTTON_SIZE,NULL,gw->toolbar,MapicOpenSprite);
		/* Get pointer... */				
		w = *tw->button;
		bw = GetWidgets(BUTTONWIDGETS, w);
		/* Blit icons... */
		masked_stretch_blit(tw->icons->tile[i], bw->image[0],0,0,32,32,0,0,BUTTON_SIZE,BUTTON_SIZE);
		masked_stretch_blit(tw->icons->tile[i], bw->image[1],0,0,32,32,0,0,BUTTON_SIZE,BUTTON_SIZE);
		masked_stretch_blit(tw->icons->tile[i], bw->image[2],0,0,32,32,0,0,BUTTON_SIZE,BUTTON_SIZE);		
		bw->value = i;
	}
	*tw->button = w; //restore original pointer;
	
	Log(">PROFILE: Initialize Window...\n");
	// Create profiler window	
	gw->profiler = CreateWidget(TYPE_WINDOW, 260, BUTTON_SIZE+4, VWIDTH-260, VHEIGHT-(BUTTON_SIZE+4), "Profiler", GUI, NULL);

	Log(" > TILES: Initialize Panel...\n");
	/* Create tileset panel*/
	gw->tileset = CreateWidget(TYPE_FRAME, 0, (BUTTON_SIZE+4), 260, VHEIGHT-(BUTTON_SIZE+4), "Tiledata", GUI, NULL);

	Log(" >   MAP: Initialize Window...\n");
	/* Create map panel*/
	gw->map = CreateWidget(TYPE_WINDOW, 260, (BUTTON_SIZE+4), VWIDTH-260, VHEIGHT-(BUTTON_SIZE+4), "Untitled", GUI, NULL);

	Log(" >SPRITE: Initialize Panel...\n");
	/* Create sprite panel*/
	gw->sprites = CreateWidget(TYPE_FRAME, 0, (BUTTON_SIZE+4), 260, VHEIGHT-(BUTTON_SIZE+4), "Spritedata", GUI, NULL);
	
	/* Create sprite buttons */
	w = CreateWidget(TYPE_BUTTON,2, 2,85,BUTTON_SIZE,"Open" ,gw->sprites, MapicOpenSprite);
	w = CreateWidget(TYPE_BUTTON,87,2,85,BUTTON_SIZE,"Erase",gw->sprites, NULL);
	w = CreateWidget(TYPE_BUTTON, 172, 2, 85, BUTTON_SIZE,"Properties", gw->sprites, NULL);

	/* ASSIGN PUBLIC POINTERS !!! */
	FOCUS = NULL;
	GUI->status = NSTATUS_IDLE;

	MAP_WINDOW = gw->map;
	PROFILE_WINDOW = gw->profiler;
	SPRITE_FRAME = gw->sprites;
	TILES_FRAME = gw->tileset;
	TOOLBAR = gw->toolbar;

	MAP_FRAME = GetWidgets(WINWIDGETS, gw->map)->frame;
	PROFILE_FRAME = GetWidgets(WINWIDGETS, gw->profiler)->frame;
	
	MAP_FRAME->status = NSTATUS_DEAD;
	PROFILE_FRAME->status = NSTATUS_IDLE;
	
	TILES_FRAME->status = NSTATUS_DEAD;
	SPRITE_FRAME->status = NSTATUS_DEAD;
	
	PROFILE_WINDOW->redraw = 1;
	MAP_WINDOW->redraw = 0;
	
	//DisableWidget(gw->sprites, 0);
}

void MapicDeinit(void)
{
	GUIWIDGETS		gw = GetWidgets(GUIWIDGETS,GUI);
	TOOLBARWIDGETS  tw = GetWidgets(TOOLBARWIDGETS, gw->toolbar);
		
	Log(">TOOLBAR: Deinitializing TOOLBARWIDGET structure...\n");
	
	//Log("\t  Destroying %p.icons...\n", tw->icons);
	//DestroyTileset(tw->icons);
	
	//Log("\t  Releasing %p structure...\n", tw->icons);
	//free(tw);
}

static void __DATATick_Tileset(void)
{
	static int x=0, y=0;
	
	x = *N.input->device->A1X - TILES_FRAME->x;
	y = *N.input->device->A1Y - TILES_FRAME->y;
	
	x >>= 5;
	y >>= 5;
	
	if( (y<<3)+x < N.mprio[N.renderer->activelayer].tileset->count)
	{
		N.selected = (y<<3)+x;
	
		if(KeyDown(L1))
		{
			N.mprio[N.renderer->activelayer].active = N.selected;			
		}
		TILES_FRAME->redraw = 1;
	}
}

static void __DATATick_Sprite(void)
{
	if(KeyDown(L1))
	{
		
	}
}


void DATASwitch(void)
{
	switch(TILES_FRAME->status)
	{
		case NSTATUS_IDLE:
			DisableWidget(TILES_FRAME, 0);
			EnableWidget(SPRITE_FRAME, 0);
			DATATICK = __DATATick_Sprite;
			break;				
			
		case NSTATUS_DEAD:
			DisableWidget(SPRITE_FRAME, 0);
			EnableWidget(TILES_FRAME, 0);
			DATATICK = __DATATick_Tileset;
			break;
		
		default:
			break;
	}	
}
	
static void DATATick(void)
{
	if(FOCUS==TILES_FRAME)
	{
		if(CursorOver(TILES_FRAME->x,TILES_FRAME->y, TILES_FRAME->w, TILES_FRAME->h))
		{
			DATATICK();
		}
	}
}

void MapicTick(void)
{
	//DATATick();
	if(KeyDown(ST))
	{
		DATASwitch();
	}
	
	if(KeyDown(SL))
	{
		switch(MAP_FRAME->status)
		{
			case NSTATUS_DEAD:
				PROFILE_FRAME->status   = NSTATUS_DEAD;
				MAP_FRAME->status = NSTATUS_IDLE;
				
				MAP_FRAME->redraw = 1;
				MAP_WINDOW->redraw = 1;		
				break;
				
			case NSTATUS_IDLE:
				MAP_FRAME->status   = NSTATUS_DEAD;
				PROFILE_FRAME->status = NSTATUS_IDLE;
				
				PROFILE_FRAME->redraw = 1;
				PROFILE_WINDOW->redraw = 1;
				break;
				
			default:
				break;
		}
	}
	
	//DATATick();
}



