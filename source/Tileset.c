#include <Ncommon.h>

void DestroyTileset(TILESET ts)
{
	int c, z;
	
	
	Log(" > TILES: Releasing Tileset %p...\n", ts);
	if(ts)
	{
		for(z=0; z<4; z++)
		{
			if(N.mprio[z].tileset==ts)
			{
				N.mprio[z].tileset = NULL;		
			}
		}
	
		if(ts->tile)
		{
			Log("\t  Bitmap data...\n");
			for(c=0; c < ts->count; c++)
			{
				destroy_bitmap((BITMAP*)ts->tile[c]);
			}
		
			Log("\t  Bitmap pointers...\n");			
				free(ts->tile);			
			ts->tile=NULL;
		} 
		
		Log("\t  Tileset memory...\n");
		ts->count = 0;
		free(ts);		
		ts = NULL;
		
		Log("\t  Tileset destroyed.\n");
		return;
		
	}
	Log("\t  Nothing to do.\n");
}

TILESET TilesFromBitmap(BITMAP *source, int tile_size)
{
	TILESET ts;
	int count;
	int x;
	int y;
	int h;
	int w;
	
	h = source->h;
	w = source->w;
	count = (h/tile_size)*(w/tile_size);
	
	//allocate memory for the tileset	
	Log("\t  Allocating memory...\n");
	ts = malloc(sizeof(t_tileset));
	if(!ts)
	{
		Log(" > TILES: FATAL: Not enough memory!\n");
		exit(999);
	}	
	ts->count = count;
	
	//allocate memory for each tile
	ts->tile = malloc(count*sizeof(BITMAP));
	if(!ts->tile)
	{
		Log(" > TILES: FATAL: Not enough memory for %i tile(s)!\n", count);
		exit(777);
	}
	Log("\t  Allocated %u bytes of structures.\n", sizeof(ts->tile));
		
	//parse source bitmap
	Log("\t  Trim %ix%i image -> %i tiles of %ix%i...\n", source->w, source->h, count, tile_size,tile_size);
	count = 0;
	for(y = 0;y < h;y += tile_size)
	{
		for(x = 0;x < w;x += tile_size)
		{
			ts->tile[count] = create_bitmap(w, h);
			blit(source, ts->tile[count], x, y, 0, 0, tile_size, tile_size);						
			count++;
		}
	}
	Log("\t  Loading complete.\n");
	
	return ts;
}
