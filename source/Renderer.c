#include <Ncommon.h>


t_glyph FONTSET[256];

void Draw(void)
{
	int x, y, z, i;
	int ox, oy;
	
	int cleartag=0;
	
	BGLAYER bg=NULL;
	
	StartProfiler(PROFILE_GUIDRAW);
	
	//	if(MAP_FRAME->redraw)cleartag=1;
		NGDraw();
//		if(cleartag)clear_bitmap(MAP_FRAME->surf);	
	

	StopProfiler(PROFILE_GUIDRAW);
	
	StartProfiler(PROFILE_DRAW);
	
	i = 0;
	
	for(z=0; z<4; z++)
	{
		bg = &N.mprio[z];
		
		if(!bg->tileset)continue;
		
		N.renderer->select(z);
		
		for(y=0; y<(VHEIGHT/32)+2; y++)
		{
					
			for(x=0; x<(VWIDTH/32)+1; x++)
			{			
				ox = x + (bg->offset_x>>5);
				oy = y + (bg->offset_y>>5);
				
				ox &= bg->size_x-1;
				oy &= bg->size_y-1;
				
				i = (oy*bg->size_x) + ox;
								
				//N.renderer->settile(x, y, bg->data[i]);
			}
		}		
			
		SpriteQueueDraw(z);		
	}
		
	StopProfiler(PROFILE_DRAW);
	
	bg = &N.mprio[N.renderer->activelayer];
	/*	
	for(z=0; z<bg->tileset->count;z++)
	{
		rect(TILES_FRAME->surf, 2+((z%8)<<5), 2+((z/8)<<5), 33+((z%8)<<5), 33+((z/8)<<5), 0);
		rect(TILES_FRAME->surf, 2+((z%8)<<5), 2+((z/8)<<5), 33+((z%8)<<5), 33+((z/8)<<5), 
				((N.mprio[N.renderer->activelayer].active==z)*Color(COLOR_HRED)) + 
				((N.selected==z)*Color(COLOR_YELLOW)) );
		
		masked_blit(N.mprio[N.renderer->activelayer].tileset->tile[z], TILES_FRAME->surf, 0, 0, 2+((z%8)<<5), 2+((z/8)<<5), 32, 32);	
	}			
	
	Print(PROFILE_FRAME->surf, 8, PROFILE_FRAME->h-16, 0, VPAL[COLOR_GRAY75], "FPS:%04d", N.fps);
	DrawProfiler();
	
	StartProfiler(PROFILE_RENDER);
	
	*/
	//*! RENDER PROCEDURE !*//
	N.renderer->render();
	N.frames++;
	StopProfiler(PROFILE_RENDER);
	
	/* CLEAR */
	
	
	
}

void InitPalette(void)
{
	Log(" >RENDER: Presetting Palette...\n");
	VPAL[COLOR_PINK  ] = GETCOL(255,   0, 255);
	VPAL[COLOR_DRED  ] = GETCOL( 64,   0,   0);
	VPAL[COLOR_RED   ] = GETCOL(128,   0,   0);
	VPAL[COLOR_HRED  ] = GETCOL(255,   0,   0);
	VPAL[COLOR_YELLOW] = GETCOL(255, 255,   0);
	VPAL[COLOR_BLUE  ] = GETCOL(  0, 255, 255);
	VPAL[COLOR_GREEN ] = GETCOL(  0, 128,   0);
	VPAL[COLOR_BROWN ] = GETCOL(255, 128,   0);
	VPAL[COLOR_PURPLE] = GETCOL(128,  64, 128);
	VPAL[COLOR_HBLUE ] = GETCOL( 64,  32,  64);
	VPAL[COLOR_HGREEN] = GETCOL(  0, 255,   0);
	
	VPAL[COLOR_GRAY25] = GETCOL(0x40,0x40,0x40);
	VPAL[COLOR_GRAY50] = GETCOL(0x80,0x80,0x80);
	VPAL[COLOR_GRAY75] = GETCOL(0xC0,0xC0,0xC0);
	VPAL[COLOR_GRAY95] = GETCOL(0xF0,0xF0,0xF0);
	VPAL[COLOR_WHITE ] = GETCOL(0xFF,0xFF,0xFF);
	
	VPAL[COLOR_SYSTEM] = GETCOL(0xA0,0xA0,0xA0);
	VPAL[COLOR_DESKTOP]= GETCOL(44,52,60);
}

void ResetGlyphTable(void)
{
	DeinitGlyphTable();
	InitGlyphTable();
}
void DeinitGlyphTable(void)
{
	int x;
	Log(" > GLYPH: Deinitialize table...\n");
	
	for(x=0; x<256; x++)
	{
		if(&FONTSET[x]!=NULL)
		{
			if(FONTSET[x].data)N.renderer->destroy(FONTSET[x].data);
			FONTSET[x].data = NULL;
			FONTSET[x].w = 0;
			FONTSET[x].h = 0;	
		}
	}
	Log("\t  Deinit completed.\n");
}
void InitGlyphTable(void)
{
	int x;
	Log(" > GLYPH: Initialize table...\n");
	
	for(x=0; x<256; x++)
	{
		FONTSET[x].data = NULL;
		FONTSET[x].w = 0;
		FONTSET[x].h = 0;	
	}
	Log("\t  Init completed.\n");
}

static int Glyph_X = 0;
static int Glyph_Y = 0;
static int Glyph_W = 0;
static int Glyph_H = 0;
static int Glyph_C = 0;
static int Glyph_BG = 0;
static void *Glyph_BMP;


unsigned int GlyphWidth(char *str)
{
	unsigned int a=0,i;
	
	for(i=0; i<strlen(str); i++)
	{
		a+= FONTSET[str[i]-' '].w;		
	}	
	return a;
}

void DrawGlyph(int index)
{
	int x,y;
	int *d,*c;
	int c1, c2;
	int r, g, b;
	
	//Log("%c", index);
	for(y=0; y < Glyph_H; y++)
	{
		for(x=0; x < Glyph_W; x++)
		{
			//masked_blit(FONTSET[index].data, Glyph_BMP, 0, 0, Glyph_X, Glyph_Y, Glyph_W, Glyph_H);
			c = &((int*)((BITMAP*)(FONTSET[index].data))->line[y])[x];
			d = &((int*)((BITMAP*)(Glyph_BMP))->line[Glyph_Y+y])[Glyph_X+x];
			
			if(*c == 0)
			{
				//paint glyph with color if detecting zero
				 *d = Glyph_C;
			} else {
				if(*c != Color(COLOR_PINK))*d = *c;
				else 
				{
					if(Glyph_BG != Color(COLOR_PINK))
					{
						*d = Glyph_BG;	
					}
				}
			}

//			DO NOT USE
//			//antialias
//			c1 = getpixel(Glyph_BMP, Glyph_X+x, Glyph_Y+y);
//			if((c1!=Color(COLOR_PINK))&&(c1!=0))
//			{	
//				c2 = getpixel(Glyph_BMP, Glyph_X+x-1, Glyph_Y+y);
//				
//				r = (getr(c1)+getr(c2))/2;
//				g = (getg(c1)+getg(c2))/2;
//				b = (getb(c1)+getb(c2))/2;
//				putpixel(Glyph_BMP, Glyph_X+x, Glyph_Y+y, GETCOL(r, g, b));
//			}
//			
			
		}
		
		
//			DO NOT USE
//			//antialias
//			c1 = getpixel(Glyph_BMP, Glyph_X+x, Glyph_Y+y);
//			if((c1!=Color(COLOR_PINK))&&(c1!=0))
//			{	
//				c2 = getpixel(Glyph_BMP, Glyph_X+x, Glyph_Y+y-1);
//				
//				r = (getr(c1)+getr(c2))/2;
//				g = (getg(c1)+getg(c2))/2;
//				b = (getb(c1)+getb(c2))/2;
//				putpixel(Glyph_BMP, Glyph_X+x, Glyph_Y+y, GETCOL(r, g, b));
//			}
			
	}
	
		
}

void Print(void *bmp, int x, int y, int color, int bg, char *format, ...)
{
	char text[256];
	int i, z;
	
	va_list ap;
	va_start(ap, format);
	vsprintf(text, format, ap);
	va_end(ap);
	
	/*!*/text[255]=0;
	
	i = 0;
	
	Glyph_C = color;
	Glyph_BMP = bmp;
	Glyph_BG = bg;
	
	while(text[i]!=0)
	{
		z = text[i]-' ';
		//if(FONTSET[z].data != NULL)
		{
			Glyph_W = FONTSET[z].w;
			Glyph_H = FONTSET[z].h;
			Glyph_X = x;
			Glyph_Y = y;
			
			DrawGlyph(z);
			
			x += Glyph_W;
		}
		i++;		
	}	
}


/*
*
*
* NON PORTABLE STUFF *-------------------------------------------------------------------------------------------------
*
*
*/
void DrawGradient(BITMAP *bmp,	int _x, int _y, int _w, int _h, 
				float r, float g, float b, float r2, float g2, float b2)
{
	float ir, ig, ib;
	float dr, dg, db;
	float ih;
	int iy;

	float x, y, w, h;
	
	x = (float)_x;
	y = (float)_y;
	w = (float)_w;
	h = (float)_h;

	ir = r;
	ig = g;
	ib = b;

	ih = 1.0f / h;
	dr = (r2-r) * ih;
	dg = (g2-g) * ih;
	db = (b2-b) * ih;

	for(iy=y; iy<y+h; iy++)
	{
		hline(bmp, x, iy, x+w-1, makecol((int)ir,(int)ig,(int)ib));

		ir += dr;
		ig += dg;
		ib += db;
	}
}

/*
void DrawBevel(BITMAP *bmp, int x, int y, int w, int h, int dir)
{
	
	rectfill(bmp,x, y, x+w, y+h, Color(COLOR_PINK));
	
	rect(bmp,x, y, x+w, y+h, Color(COLOR_HGREEN));
}
*/

void DrawBevel(BITMAP *bmp, int x, int y, int w, int h, int dir)
{
	w--;
	h--;
	
	switch(dir)
	{	
		case 1: //PRESSED
			rectfill(bmp,  x,   y,   x+w,   y+h, Color(COLOR_GRAY25));
			rectfill(bmp,x+1, y+1,   x+w,   y+h, Color(COLOR_GRAY50));
			rectfill(bmp,x+2, y+2, x+w-1, y+h-1, Color(COLOR_WHITE));
			rectfill(bmp,x+2, y+2, x+w-2, y+h-2, Color(COLOR_GRAY75));
			
			break;
		default://RELEASED
			rectfill(bmp,  x,   y,   x+w,   y+h, Color(COLOR_GRAY25));
			rectfill(bmp,  x,   y, x+w-1, y+h-1, Color(COLOR_GRAY50));
			rectfill(bmp,x+1, y+1, x+w-2, y+h-2, Color(COLOR_GRAY75));
			rectfill(bmp,x+1, y+1, x+w-3, y+h-3, Color(COLOR_WHITE));
			rectfill(bmp,x+2, y+2, x+w-3, y+h-3, Color(COLOR_GRAY75));			
			break;
	}
	
}