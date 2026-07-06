#ifndef __TILESET_H
#define __TILESET_H

typedef struct s_tileset* TILESET;
typedef struct s_tileset{
	void **tile;
	int count;
}t_tileset;

void DestroyTileset(TILESET ts);
TILESET TilesFromBitmap(BITMAP *source, int tile_size);

#endif
