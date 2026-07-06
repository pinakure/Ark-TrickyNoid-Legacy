#ifndef __SPRITE_H
#define __SPRITE_H

enum E_SPRITESTATUS
{
	STATUS_DEAD,
	STATUS_HIDDEN,
	STATUS_IDLE,
	STATUS_TICK
};

typedef enum E_FPSTYPES
{
	FPS_LOW=0,
	FPS_MID,
	FPS_HIGH
}FPSTYPE;

//Sprite Class
typedef struct s_sprite* SPRITE;
typedef struct s_sprite{
	
	//pt2d vposition;//virtual position on the map
	int x;//position on the screen
	int y;
	int width;
	int height;
	void *data;		//BITMAP data
	int *fps;		//pointer to clock (3 speeds)
	
	int frames;
	int frame;
	int animation;
	int reverseanim;
	
	int hflip;
	int vflip;

	int status;
	SPRITE next;
	SPRITE prev;
	int priority; //helper to remind where the sprite was before.
}t_sprite;

void SpriteQueueDraw(int z);
SPRITE SpriteFromBitmap(BITMAP *source, int hsize, int vsize, FPSTYPE index);
void DeinitSprites(void);
void SpriteSetPriority(SPRITE spr, int prio);

#endif