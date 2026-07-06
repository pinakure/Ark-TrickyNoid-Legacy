#include <Ncommon.h>

#define RENDERER_STRING  "Allegro 4.2"

renderer AllRenderer;//Allegro renderer

static BITMAP *VBuffer=NULL;
static BITMAP *SBuffer=NULL;
static BITMAP *MBMP = NULL;
static void __AllRendererRender(void)
{
	static int m_ox, m_oy;
	
	m_ox =  *N.input->device->A1X-9;
	m_oy =  *N.input->device->A1Y-11;
	
	blit(N.renderer->buffer, MBMP, m_ox, m_oy, 0, 0, 32, 32);//capture buffer under mouse
	
	masked_blit(CURSOR->tile[0], N.renderer->buffer, 0, 0, m_ox, m_oy, 32, 32);//blit mouse
	
	blit(N.renderer->buffer, screen, 0, 0, 0, 0, VWIDTH, VHEIGHT);
	
	blit(MBMP, N.renderer->buffer, 0, 0, m_ox, m_oy, 32, 32);//restore buffer under mouse
	//clear_bitmap(VBuffer);
}

static void __AllRendererDeinit(void)
{
	Log(" >RENDER: Deinitializing renderer (%s)...\n", RENDERER_STRING);
	
	destroy_bitmap(VBuffer);
	destroy_bitmap(SBuffer);
	destroy_bitmap(MBMP);
		
	Log("\t  Deinit complete.\n");
}

static void __AllSetTile(int x, int y, int c)
{
	int ox, oy;
	
	ox = N.mprio[N.renderer->activelayer].offset_x;
	oy = N.mprio[N.renderer->activelayer].offset_y;
	
	x<<=5;
	y<<=5;
	rect(MAP_FRAME->surf, x-(ox&31), y-(oy&31), 32 + x - (ox&31), 32 + y - (oy&31), Color(COLOR_GRAY50));
	masked_blit(N.mprio[N.renderer->activelayer].tileset->tile[c], MAP_FRAME->surf, 0, 0, x-(ox&31), y-(oy&31), 32, 32);	
}


static void __AllSetTileFast(int x, int y)
{
	masked_blit(N.mprio[N.renderer->activelayer].tileset->tile[N.renderer->activeindex], VBUFFER, 0, 0, x<<5, y<<5, 32, 32);
}

static void __AllSelectLayer(int layer)
{
	N.renderer->activelayer = layer;	
}


static void __AllDrawSprite(SPRITE spr)
{
	int x,y;
	
	x = spr->x;
	y = spr->y;
	
	x -= N.mprio[spr->priority].offset_x;
	y -= N.mprio[spr->priority].offset_y;
	
	x %= (N.mprio[spr->priority].size_x<<5);
	y %= (N.mprio[spr->priority].size_y<<5);
	
	if(x>-N.mprio[spr->priority].size_x)
	if(y>-N.mprio[spr->priority].size_y)
	masked_blit(((BITMAP**)spr->data)[spr->frame], VBUFFER,
				0, 0,
				x-1, y-1,
				spr->width, spr->height);
}

/*


Structure	Corresponding bytes
BITMAPFILEHEADER	0x00 0x0D
BITMAPINFOHEADER	0x0E 0x35
RGBQUAD array	0x36 0x75
Color-index array	0x76 0x275


*/
typedef unsigned char 	BYTE;
typedef unsigned short 	WORD;
typedef unsigned int	DWORD;
typedef signed int 		LONG;

typedef struct __attribute__((packed)) bmpheader
{
	WORD 	magic;
	DWORD 	size; //size in bytes
	WORD 	zero1;
	WORD 	zero2;
	DWORD 	offset;	
}BMPHEADER;

typedef struct __attribute__((packed)) bmpinfo 
{
	DWORD 	size;
	LONG	width;
	LONG	height;
	WORD 	planes;
	WORD 	bpp;
	DWORD	compressed;
	DWORD 	imagesize;
	LONG	xppm;
	LONG	yppm;
	DWORD 	color_used;
	DWORD	color_important;
}BMPINFO;

typedef struct __attribute__((packed)) rgbquad
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE reserved;
}RGBQUAD;

typedef struct __attribute__((packed)) rgbtriplet
{
	BYTE blue;
	BYTE green;
	BYTE red;
}RGBTRIPLET;

static void FillBitmap24(BITMAP *bmp, RGBTRIPLET *p)
{
	int x, y;

	for(y=bmp->h-1; y>= 0; y--)
	{
		for(x=0; x< bmp->w; x++)
		{
			putpixel(bmp, x, y, makecol(p->red, p->green, p->blue));
			p++;
		}
	}
}

static void FillBitmap32(BITMAP *bmp, RGBQUAD *p)
{
	int x, y;
	
	for(y=bmp->h-1; y>= 0; y--)
	{
		for(x=0; x< bmp->w; x++)
		{
			putpixel(bmp, x, y, makecol(p->red, p->green, p->blue));
			p++;
		}
	}
}

static void FillBitmap8(BITMAP *bmp, unsigned char *p, RGBQUAD *pal)
{
	int x, y;
	
	for(y=0; y <  bmp->h; y++)
	{
		for(x=0; x< bmp->w; x++)
		{
			putpixel(bmp, x, y, makecol(pal[*p].red,pal[*p].green,pal[*p].blue));
			Log("%02x ", *p);
			p++;
		}
		Log("\n");
	}	
}

static BITMAP *ParseBitmap(void *rawdata)
{
	BITMAP *bmp = NULL;
	char *data = rawdata;
	//parse bitmap
	BMPHEADER *bh = rawdata;
	BMPINFO *bi;
	
	if(bh->magic!=0x4D42)
	{
		Log("\t  Bitmap signature is 0x%X...", bh->magic);
		Log(" >RENDER: Not a bitmap file!\n");
		return NULL;
	}
	
	//create bitmap structure
	bi = (BMPINFO*)(data + sizeof(BMPHEADER));
	
	if(bi->compressed != 0)
	{
		Log(" >RENDER: Can't handle compressed bitmaps! (%i)\n", bi->compressed);
		exit(8);
	}
	
	bmp = create_bitmap(bi->width, bi->height);
	if(!bmp)
	{
		Log(" >RENDER: Couldn't create bitmap with address %p\n", rawdata);
		return NULL;
	}
	
	//paint bitmap on structure
	switch(bi->bpp)
	{
		case  8:
			Log("CLR: %x\n", bi->color_used);
			FillBitmap8(bmp, (unsigned char *)data + bh->offset, (RGBQUAD*)((char*)bi +  bi->size));
			exit(1);
			break;
			
		case 24:
			FillBitmap24(bmp,(RGBTRIPLET*)(data + bh->offset));
			break;
			
		case 32:
			exit(1);
			FillBitmap32(bmp, (RGBQUAD*)(data + bh->offset));
			break;
			
		default:
			Log(" >RENDER: FATAL: Bitmap color depth %d is not supported!\n", bi->bpp);
			exit(0x666);
	}
	return bmp;
}

static TILESET __AllLoadTiles(int ident)//char *name)
{
	BITMAP *bmp;
	TILESET ts;
	
	Log(" > TILES: Load tileset #%x...\n", ident);

	bmp = ParseBitmap(GetAsset(ident));
	
	ts = TilesFromBitmap(bmp, 32);


	if(!ts)
	{
		Log("FATAL: Couldn't create tileset from bitmap '%i'!\n", ident);
		exit(0x666);
	}
	return ts;
}

static SPRITE __AllLoadSprite(int ident)
{
	SPRITE spr;
	BITMAP *bmp;
	//bmp = N.renderer->load(name);
	
	bmp = ParseBitmap(GetAsset(ident));
	
	if(!bmp)
	{
		Log("FATAL: Couldn't create bitmap from asset id #%i!\n", ident);
		exit(666);
	}
	
	spr = SpriteFromBitmap(bmp, 16, 32, GetRand()%3);
	
	destroy_bitmap(bmp);
	
	if(!spr)
	{		
	
		Log("FATAL: Can't create sprite!\n");
		exit(666);
	}
	
	return spr;	
}

static void __AllDestroyBitmap(BITMAP *bmp)
{
	destroy_bitmap(bmp);
}
static void *__AllCreateBitmap(int w, int h)
{
	BITMAP *b;
	
	b = create_bitmap(w, h);
	
	return b;
}
static void *__AllLoadBitmap(char *name)
{
	BITMAP *b;
	
	
	
	b = load_bitmap(concat(BASEPATH, name), NULL);
	if(!b)
	{
		Log("FATAL: Couldn't load bitmap \"%s\"!\n", concat(BASEPATH, name));
		exit(666);		
	}	
	return b;
}


static BITMAP *CropGlyph(BITMAP *src)
{
	int x;
	int width;
	BITMAP *dst;
	int *pixel;
	
	if(!src)return NULL;
		
	width = src->w;
	
	for(x=0; x < src->w; x++)
	{
		pixel = &((int*)src->line[0])[x];
		
		if(*pixel == Color(COLOR_YELLOW))
		{
			width = x;//limit found
			break;
		}
	}
	
	dst = create_bitmap(width, src->h);
	
	blit(src, dst, 0, 0, 0, 0, width, src->h);
	
	destroy_bitmap(src);
	
	return dst;
}

static void __AllSetFont(int ident)
{
	BITMAP *bmp;
	BITMAP *pbmp;
	int w, h;
	int x, y;
	int i;

	Log(" >RENDER: Load font #%i...\n", ident);
		
	bmp = ParseBitmap(GetAsset(ident));
	
	w = bmp->w;
	h = bmp->h;
	
	pbmp = create_bitmap(w+17, h+7);
	clear_to_color(pbmp, makecol(255,255,0));
	
	w = bmp->w / 16;
	h = bmp->h / 6;
	
	
	ResetGlyphTable();
	i = 0;
	
	for(y=0; y<6;y++)
	{
		for(x=0; x<16; x++)
		{
			FONTSET[i].data = N.renderer->createsurf(w, h);
			blit(bmp, FONTSET[i].data, x*w, y*h, 0, 0, w, h);
			FONTSET[i].data = CropGlyph(FONTSET[i].data);
			FONTSET[i].w = ((BITMAP*)FONTSET[i].data)->w;
			FONTSET[i].h = ((BITMAP*)FONTSET[i].data)->h;			
			i++;
		}
	}
	N.renderer->destroy(bmp);
	
	Log(" >RENDER: Loaded font #%i sucessfully...\n", ident);	
}

static void __AllPrintAt(int x, int y, int fg, int bg, char *format, ...)
{
	va_list pl;
	static char buffer[1024];
	
	va_start(pl, format);
	
	vsprintf(buffer, format, pl);
	
	va_end(pl);
	
	textprintf_ex(N.renderer->buffer, font, x, y, fg, bg, "%s", buffer);
}
		
static int __AllRendererInit(void)
{
	Log(" >RENDER: Initialize driver (%s)...\n", RENDERER_STRING);
	
	set_color_depth(VDEPTH);

	#ifdef FULLSCREEN
		#ifdef LINUX
			if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, VWIDTH, VHEIGHT, 0, 0)<0)
			{
				Log(" >RENDER: PANIC: Failed to switch to %ix%i fullscreen!\n", VWIDTH<<1, VHEIGHT<<1);
				exit(1);
			}
		#else
			if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, VWIDTH, VHEIGHT, 0, 0)<0)
			{
				Log(" >RENDER: PANIC: Failed to switch to %ix%i fullscreen!\n", VWIDTH<<1, VHEIGHT<<1);
				exit(1);
			}	
		#endif
	#else
		if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, VWIDTH, VHEIGHT, 0, 0)<0)
		{
			Log(" >RENDER: PANIC: Failed to switch to %ix%i windowed!\n", VWIDTH<<1, VHEIGHT<<1);
			exit(1);
		}
	#endif

	VBuffer = create_bitmap(VWIDTH, VHEIGHT);
	SBuffer = create_bitmap(VWIDTH, VHEIGHT);
	MBMP = create_bitmap(32, 32);
		
	if((!VBuffer)||(!SBuffer)) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		Log(" >RENDER: FATAL: Couln't find a matching video mode!\n");
		exit(66);
	}
	clear_bitmap(VBuffer);
	clear_bitmap(SBuffer);
	
	AllRenderer.settilefast = __AllSetTileFast;
	
	AllRenderer.drawsprite = __AllDrawSprite;
	AllRenderer.loadsprite = __AllLoadSprite;
	AllRenderer.loadtiles = __AllLoadTiles;

	AllRenderer.setfont = __AllSetFont;
	AllRenderer.printat = __AllPrintAt;
	AllRenderer.settile = __AllSetTile;
	AllRenderer.deinit = __AllRendererDeinit;
	AllRenderer.render = __AllRendererRender;
	AllRenderer.buffer = VBuffer;
	AllRenderer.select = __AllSelectLayer;
	AllRenderer.init = __AllRendererInit;
	
	AllRenderer.load = __AllLoadBitmap;
	AllRenderer.destroy= __AllDestroyBitmap;
	AllRenderer.createsurf = __AllCreateBitmap;	
	AllRenderer.activelayer = 0;
	AllRenderer.activeindex = 0;	
	
	return 1;
}

/*--------------------------------*/

void RendererBind(void)
{
	N.renderer = &AllRenderer;
	N.renderer->init = __AllRendererInit;	
}
