#ifndef __MAP_H
#define __MAP_H
//Point structures
typedef struct s_pt2d* PT2D;
typedef struct s_pt2d
{
	int x;
	int y;
}pt2d;

typedef struct s_pt3d* PT3D;
typedef struct s_pt3d 
{
	int x;
	int y;
	int z;
}pt3d;

typedef struct s_map* MAP;
typedef struct s_map{
	pt2d size[4];	
}t_map;

void MapTick(void);
void MapInit(void);
void MapClear(void);
void MapSet(int x,  int y, int c);

#endif
