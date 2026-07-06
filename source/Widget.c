#include <Ncommon.h>

WIDGET GUI;

//------------------------------------------------------------------------------
//		INITIALIZATION
//------------------------------------------------------------------------------
void WIDGET_FrameInit(WIDGET me){
	FRAMEWIDGETS fw;
	AllocWidgets(t_framewidgets, fw, me, >>FRAME);
	
	me->status = NSTATUS_DEAD;
}

void WIDGET_WindowInit(WIDGET me){
	WINWIDGETS ww;	
	AllocWidgets(t_winwidgets, ww, me, >WINDOW);
	
	ww->statusbar = CreateWidget(TYPE_FRAME,5, 19, me->w-12, me->h-49, "status", me, NULL);
	ww->title= CreateWidget(TYPE_FRAME,5, 19, me->w-12, me->h-49, "title", me, NULL);
	ww->frame = CreateWidget(TYPE_FRAME,5, 19, me->w-12, me->h-49, "frame",me, NULL);
	ww->menu=CreateWidget(TYPE_FRAME,5, 19, me->w-12, me->h-49, "menu", me, NULL);
			
	Log("\t  Init complete\n");
}

void WIDGET_ButtonInit(WIDGET me){
	
}

void WIDGET_ToolbarInit(WIDGET me){
	TOOLBARWIDGETS tw;
	AllocWidgets(t_toolbarwidgets, tw, me, TOOLBAR);
	
}

void WIDGET_DesktopInit(WIDGET me){
	GUIWIDGETS gw;
	AllocWidgets(t_guiwidgets, gw, me, DESKTOP);
}

//------------------------------------------------------------------------------
//		DEINITIALIZATION
//------------------------------------------------------------------------------
void WIDGET_FrameDeinit(WIDGET me){
	FRAMEWIDGETS fw = GetWidgets(FRAMEWIDGETS, me);
	
	Log(" > FRAME: %p destroying FRAMEWIDGETS %p...\n", me, fw);
	
	/*destroy binded data*/
	Log("\t  Destroying %p.FRAMEWIDGETS {%p} structure...\n", me, fw);
	
	free(me->data);
	me->data = NULL;
	
	Log("\t  Deinit complete.\n");
	
}

void WIDGET_WindowDeinit(WIDGET me){
	WINWIDGETS ww;
	if(me->data)
	{
		ww = GetWidgets(WINWIDGETS, me);
		Log(" >WINDOW: %p destroying WINWIDGETS %p...\n", me, ww);
		
		/*if(ww->title)DestroyWidget(ww->title);
		if(ww->frame)DestroyWidget(ww->frame);
		if(ww->statusbar)DestroyWidget(ww->statusbar);
		if(ww->menu)DestroyWidget(ww->menu);
		
		ww->menu = NULL;
		ww->frame = NULL;
		ww->statusbar = NULL;
		ww->menu = NULL;*/
		
		Log("\t  Releasing structure %p WINWIDGETS...\n", me->data);
		free(me->data);
		me->data = NULL;
		
		Log("\t  Destroyed WINWIDGETS.\n");
	}
}

void WIDGET_ButtonDeinit(WIDGET me){
	BUTTONWIDGETS bw = GetWidgets(BUTTONWIDGETS, me);
	
	Log(" >BUTTON: %p destroying BUTTONWIDGETS %p...\n", me, bw);
	
	N.renderer->destroy(bw->image[0]);
	N.renderer->destroy(bw->image[1]);
	N.renderer->destroy(bw->image[2]);
	me->surf=NULL;
	
	Log("\t  %p erasing DATA in %p...\n", me, bw);
	
	free(me->data);
	me->data = NULL;
	
	Log("\t  Deinit complete.\n");
	
}

void WIDGET_ToolbarDeinit(WIDGET me){
	TOOLBARWIDGETS tw = GetWidgets(TOOLBARWIDGETS, me);
	tw=NULL;
}

void WIDGET_DesktopDeinit(WIDGET me){
	Log(">DESKTOP: %p releasing DATA structure %p...\n", me, GetWidgets(GUIWIDGETS, me));	
	
	free(me->data);
	
	Log("\t  Releasing complete.\n");
}

//------------------------------------------------------------------------------
//		RENDER
//------------------------------------------------------------------------------
void WIDGET_FrameRender(WIDGET me){	
	DrawBevel(me->surf, 0, 0, me->w, me->h, 1);
}

void WIDGET_WindowRender(WIDGET me){
	// gray backdrop
	DrawBevel(me->surf, 0, 0, me->w, me->h, 0);
	DrawGradient(me->surf, 0+2, 0+2, me->w-4, me->h-4, 0xA0, 0x80, 0xB0, 0xC8, 0x86, 0xCA);
	
	// title bar	
	rectfill(me->surf, 3, 3, me->w - 4, 14, GETCOL(0xd0, 0x40, 0xf0));
	rectfill(me->surf, 4, 4, me->w - 4, 14, 0);
	DrawGradient(me->surf, 4, 4, me->w - 8,5, 0x64, 0x20, 0x95, 0xd0, 0x40, 0xf0);
	DrawGradient(me->surf, 4, 9, me->w - 8,5, 0xd0, 0x60, 0xe0, 0x45, 0x20, 0x85);	
	
	//draw main frame
	DrawBevel(me->surf, 3, 17, me->w-7, me->h-21-((GetWidgets(WINWIDGETS, me)->statusbar!=NULL)*24), 1);
	
	//print caption	
	if(me->caption)
	{
		Print(me->surf, 5+ (me->w/2) - (text_length(font, me->caption)/2), 5, Color(COLOR_DRED), Color(COLOR_PINK), "%s", me->caption);
		Print(me->surf, 4+ (me->w/2) - (text_length(font, me->caption)/2), 4, Color(COLOR_WHITE), Color(COLOR_PINK), "%s", me->caption);
	}
	
	if(GetWidgets(WINWIDGETS, me)->statusbar!=NULL)
	{
		DrawBevel(me->surf, 3, me->h-26, me->w-7, 22, 1);
	}
}

void WIDGET_ButtonRender(WIDGET me){	
	BUTTONWIDGETS bw = GetWidgets(BUTTONWIDGETS, me);

	DrawBevel(bw->image[0], 0, 0, me->w, me->h, 0);
	DrawBevel(bw->image[1], 0, 0, me->w, me->h, 0);
	DrawBevel(bw->image[2], 0, 0, me->w, me->h, 1);

	DrawGradient(bw->image[0], 0+2, 0+2, me->w-4, me->h-4,  0xC8, 0xC6, 0xC2, 0x94, 0x93, 0x91);
	DrawGradient(bw->image[1], 0+2, 0+2, me->w-4, me->h-4,  0xB8, 0xBA, 0xF0, 0x64, 0x63, 0x91);
	DrawGradient(bw->image[2], 0+2, 0+2, me->w-4, me->h-4,  0xB4, 0xB3, 0xC1, 0xE8, 0xE6, 0xF2);

	if(me->caption)
	{
		#define GX 2+(((me->w-4)/2)-(GlyphWidth(me->caption)/2))
		#define GY ((me->h/2)-(FONTSET[0].h/2))
		Print(bw->image[0], GX, GY, 0, Color(COLOR_PINK), "%s", me->caption);
		Print(bw->image[1], GX, GY, Color(COLOR_WHITE), Color(COLOR_PINK), "%s", me->caption);
		Print(bw->image[2], GX, GY, 0, Color(COLOR_PINK), "%s", me->caption);
		#undef GY
	}
}

void WIDGET_ToolbarRender(WIDGET me){
	TOOLBARWIDGETS tw = GetWidgets(TOOLBARWIDGETS, me);
	DrawBevel(me->surf, 0, 0, me->w, me->h, 0);
	
	tw=NULL;
}

void WIDGET_DesktopRender(WIDGET me){
	Log(">DESKTOP: Rendering...\n");
	
	clear_to_color(me->surf, Color(COLOR_DESKTOP));
	DrawGradient(me->surf, 0, 0, me->w, me->h, 44, 52, 60, 22, 26, 30);
	
	Log("\t  Render finished.\n");
}

//------------------------------------------------------------------------------
//		TICK 
//------------------------------------------------------------------------------
void WIDGET_FrameTick(WIDGET me){
	if(CursorOver(me->x, me->y, me->w, me->h))
	{
		SetFocus(me);
	}
	
}
void WIDGET_DesktopTick(WIDGET me){
	GUIWIDGETS gw = GetWidgets(GUIWIDGETS, me);
	
	gw=NULL;
}

void WIDGET_WindowTick(WIDGET me){
	WINWIDGETS ww = GetWidgets(WINWIDGETS, me);
	
	ww=NULL;
}

void WIDGET_ToolbarTick(WIDGET me){
	TOOLBARWIDGETS tw = GetWidgets(TOOLBARWIDGETS, me);
	
	tw=NULL;
}

void WIDGET_ButtonTick(WIDGET me){
	BUTTONWIDGETS bw = GetWidgets(BUTTONWIDGETS, me);
	if(CursorOver(me->x, me->y, me->w, me->h))
	{
		//if(me->status!=NSTATUS_MOUSEOVER)me->redraw=1;
		me->redraw=1;	
		me->surf = GetWidgets(BUTTONWIDGETS, me)->image[1];
		me->status = NSTATUS_MOUSEOVER;
		
		if(KeyDown(L1))
		{
			me->status = NSTATUS_MOUSEDOWN;			
			me->surf = GetWidgets(BUTTONWIDGETS, me)->image[2];		
			me->redraw=1;
		} 
		else 
		if(KeyHold(L1))
		{
			me->surf = GetWidgets(BUTTONWIDGETS, me)->image[2];
			me->status = NSTATUS_MOUSEHOLD;
		}
		else
		if(KeyUp(L1))
		{
			me->redraw=1;
			me->status = NSTATUS_MOUSEUP;
			if(bw->hook)
			{
				bw->hook(bw->value);
			}
		} 		
	} else {
		if(me->surf!=GetWidgets(BUTTONWIDGETS, me)->image[0])
		{
			me->surf = GetWidgets(BUTTONWIDGETS, me)->image[0];
			me->redraw=1;
		}
	}
}	
