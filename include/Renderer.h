#ifndef __RENDERER_H
#define __RENDERER_H

#define GETCOL makecol

//Enumerable colors
enum E_Colors{
	COLOR_BLACK=0,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_DRED,
	COLOR_RED,
	COLOR_PURPLE,
	COLOR_BROWN,
	COLOR_GRAY,
	COLOR_HBLACK,
	COLOR_HBLUE,
	COLOR_HGREEN,
	COLOR_HCYAN,
	COLOR_HRED,
	COLOR_YELLOW,
	COLOR_WHITE,
	COLOR_GRAY25,
	COLOR_GRAY50,
	COLOR_GRAY75,
	COLOR_GRAY95,
	COLOR_PINK,
	COLOR_SYSTEM,
	COLOR_DESKTOP,
	COLOR_MAX=255
};


typedef struct s_glyph* GLYPH;
typedef struct s_glyph
{
	void *data;
	int w;
	int h;	
}t_glyph;

//Renderer class
typedef struct s_renderer* RENDERER;
typedef struct s_renderer
{
	void (*render)(void);
	void *buffer;//drawing backbuffer
	
	//init / deinit
	int (*init	  )(void);
	void (*deinit)(void);
	
	//bitmap handlers
	void *(*createsurf)(int w, int h);
	void *(*load)(char *file);
	void (*destroy)(BITMAP *bmp);
	
	//font handlers
	void (*printat)(int x, int y, int bg, int fg, char *format, ...);
	void (*setfont)(int ident);
	
	//tile handlers
	//TILESET (*loadtiles)(char *file);
	TILESET (*loadtiles)(int ident);
	
	void (*settilefast)(int x, int y);
	void (*settile)(int x, int y, int c);
	void (*select)(int layer);
	
	//sprite handlers
	void (*drawsprite)(SPRITE spr);
	SPRITE (*loadsprite)(int ident);
	
	TILESET tileset[4];
	int activeindex; //active tile index
	int activelayer; //active layer index
	int palette[256];
}renderer;

void DrawLayers(void);
void InitPalette(void);
void RendererBind(void);
void Draw(void);
void LoadAssetBitmap(int ident);
void DrawGradient(BITMAP *bmp,	int _x, int _y, int _w, int _h, float r, float g, float b, float r2, float g2, float b2);
void DrawBevel(BITMAP *bmp, int x, int y, int w, int h, int dir);
void Print(void *bmp, int x, int y, int color, int bg, char *format, ...);

void ResetGlyphTable(void);
void InitGlyphTable(void);
void DeinitGlyphTable(void);
unsigned int GlyphWidth(char *str);


extern renderer AllRenderer;

#endif
