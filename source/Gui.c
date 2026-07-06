/*

Bautist said:
	
	From now, this module may be know as the NG, or N·Gui

*/
#include <Ncommon.h>
// #include <Widget.h>

WIDGET FOCUS;

char *S_NGWTYPE[] = {
	"DESKTOP",
	"FRAME",
	"WINDOW",
	"BUTTON",
	"TEXTBOX",
	"TOOLBAR"	
};

/**-----------------------------------------------------
			  	   WIDGET HANDLER CODE
-----------------------------------------------------**/
static void TickWidget(WIDGET w)
{
	if(w->retick)
	{
		if(w->status!=NSTATUS_DEAD)
		{	
			w->tick(w);
		}
	}
	if(w->child)
	{
		TickWidget(w->child);
	}	
	if(w->next)
	{
		TickWidget(w->next);
	}
	//w->retick = 0;
}

static void DrawWidget(WIDGET w)
{
	if(w->redraw)
	{
		//HACK 
		blit(w->surf, VBUFFER, 0, 0, w->x, w->y, w->w, w->h);		
		w->redraw = 0;		
	}
	
	if(w->child)
	{
		DrawWidget(w->child);
	}
	
	if(w->next)
	{
		DrawWidget(w->next);
	}	
}

static void WidgetDeleteChild(WIDGET parent, WIDGET child)
{
	Log("\t  Removing child from parent %p...\n", parent);
			
	parent->childcount--;	
}

static void WidgetAppendChild(WIDGET parent, WIDGET child)
{
	WIDGET w;
	int i;
	
	Log("\t  Appending child to parent %p...\n", parent);
	
	w = malloc( (parent->childcount+1)*sizeof(WIDGET));
	
	for(i=0;i<parent->childcount; i++)
	{
		w[i] = parent->child[i];
	}
	parent->childcount++;
	
	w[parent->childcount] = child[0];
	
	free(parent->child);
	
	parent->child = w;
}

static void WidgetDeleteData(WIDGET w)
{
	Log("\t  Destroying %p.data {%p}...\n", w, w->data);
	switch(w->type)
	{
		default:
			break;
	}
}

static void WidgetRender(WIDGET w)
{
	Log("\t  Rendering %p...\n", w);
	switch(w->type)
	{
		default:
			break;		
	}
}

static void WidgetDelete(WIDGET w)
{
	if(w)
	{
		Log(" >WIDGET: Destroying %s WIDGET structure...\n", S_NGWTYPE[w->type]);
		
		while(w->next)
		{
			Log("\t  Destroying sibiling %p...\n", w->next);
			DestroyWidget(w->next);
			w->next = NULL;
		}
		
		if(w->parent)WidgetDeleteChild(w->parent, w);
			
		WidgetDeleteData(w);
		
		
		if(w->surf)
		{
			Log("\t  Destroying %p.surf {%p}...\n", w, w->surf);
			N.renderer->destroy((BITMAP*)w->surf);
		}
		
		if(w->caption)
		{
			Log("\t  Releasing %p.caption \"%s\"...\n", w, w->caption);
			free(w->caption);
		}

		Log("\t  Releasing memory %p...\n",w);
		free(w);
		
	} else {
		Log("\t  PANIC: WIDGET IS NULL (%i)!", __LINE__);
	}
}

void DestroyWidget(WIDGET w)
{
	if(w)
	{		
		while(w->child)
		{
			Log("\t  Deleting %p.child...\n", w->child);
			WidgetDelete(w->child);
		}
		
		WidgetDelete(w);
		
	} else {
		Log("\t  No widget to release!\n");
	}
}

WIDGET CreateWidget(NGWTYPE type, int x, int y, int w, int h, char *caption, WIDGET parent, void *data)
{
	WIDGET W;
	
	if(type>TYPE_NONE)
	{
		Log(" >WIDGET: PANIC: Invalid type %i specified!!!\n", type);
	}
	
	Log(" >WIDGET: Allocating %s WIDGET structure...\n", S_NGWTYPE[type]);
	W = malloc(sizeof(t_widget));
	if(!W)
	{		
		Log("\t  FATAL: Not enough memory!\n");
		exit(666);
	}
	
	if(!parent)
	{
		W->parent   = NULL;
	}
	else
	{
		W->x += parent->x;
		W->y += parent->y;
		W->parent   = parent;
		WidgetAppendChild(parent, W);
	} 
	return W;
	
	if(caption == NULL)
	W->caption = NULL;
	else 
	W->caption = strdup(caption);
		
	W->w = w;
	W->h = h;	
	if(W->w<0)W->w*=-1; //let's not became mad because negative values here ;)
	if(W->h<0)W->h*=-1;
		
	W->redraw = 1;
	W->retick = 1;
	
	W->child    = NULL;			
	W->data 	= NULL;
	
	W->type   = type;
	W->status = NSTATUS_DEAD;
	
	W->x = x;
	W->y = y;
	
	W->next = NULL;
	
	W->surf = N.renderer->createsurf(W->w, W->h);
	
	Log("\t  Perform init process...\n");
		
	switch(type)
	{
		BUTTONWIDGETS bw;
			
		case TYPE_DESKTOP:			
			W->tick = WIDGET_DesktopTick;
			
			/*init*/
			
			break;

		case TYPE_FRAME:
			W->tick	= WIDGET_FrameTick;

						
			/*init*/
			
			break;

		case TYPE_WINDOW:
			W->tick	= WIDGET_WindowTick;

						
			/*init*/
			
			break;
			
		case TYPE_BUTTON:
			
			W->tick	= WIDGET_ButtonTick;

						
			/*init*/
			
			
			AllocWidgets(t_buttonwidgets, bw, W, >BUTTON);	
	
			N.renderer->destroy(W->surf);//Destroy the surface, we don't need it to exist, only the pointer is needed here
	
			bw->image[0] = N.renderer->createsurf(W->w, W->h);
			bw->image[1] = N.renderer->createsurf(W->w, W->h);
			bw->image[2] = N.renderer->createsurf(W->w, W->h);
	
			if(data)bw->hook = data;
			else bw->hook = NULL;
			
			W->surf = bw->image[0]; //Make surf point to the active bitmap
	
			W->status = NSTATUS_IDLE; //Enable button
			
			break;

		case TYPE_TOOLBAR:
			W->tick	= WIDGET_ToolbarTick;

						
			/*init*/
			
			break;
				
		case TYPE_TEXTBOX:
		case TYPE_NONE:
		default:
			Log("\t  FATAL: Unknown widget type %i requested!\n", type);
			exit(666);
	}
	
	WidgetRender(W);
	
	Log("\t  WIDGET %p created.\n", W);
	
	return W;
}

/**-----------------------------------------------------
			  	        N·GUI API
-----------------------------------------------------**/
void SetFocus(WIDGET focus)
{
	if(FOCUS!=focus)
	{
		//if(focus->z>FOCUS->z)
		FOCUS = focus;
	}
}
void NGTick(void)
{
	GUI->retick=1;//FORCE ALWAYS DESKTOP TICK
	
	TickWidget(GUI);
}

void NGDraw(void)
{
	//GUI->redraw=1;//FORCE ALWAYS DESKTOP REDRAW
	
	DrawWidget(GUI);
}

void NGInit(void)
{
	Log(" >   GUI: Initializing...\n");
	GUI = CreateWidget(TYPE_DESKTOP, 0, 0, N.geometry.x, N.geometry.y, "M4G·Mapic", NULL, NULL);
	Log(" >   GUI: Created father widget $GUI.\n");
}

void DisableWidget(WIDGET w, int recurse)
{
	if((recurse)&&(w->next))
	{
		DisableWidget(w->next, 1);
	}
	
	if(w->child)
	{
		DisableWidget(w->child, 1);
	}
	
	w->status = NSTATUS_DEAD;
	w->redraw = 0;
}
void EnableWidget(WIDGET w, int recurse)
{
	if((recurse)&&(w->next))
	{
		EnableWidget(w->next, 1);
	}
	
	if(w->child)
	{
		EnableWidget(w->child, 1);
	}
	w->status = NSTATUS_IDLE;
	w->redraw = 1;
}

void NGDeinit(void)
{
	Log(" >   GUI: Deinitializing...\n");	
	DestroyWidget(GUI);	
	
	Log("\t  Destroyed GUI widget.\n");	
	
}