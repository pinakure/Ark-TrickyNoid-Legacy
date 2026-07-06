#include <Ncommon.h>

static void MapMove(int x, int y)
{
	if(N.y+y>=0)
	{
		N.y+=y;
		N.mprio[0].offset_y += y * N.mprio[0].speed_y;
		N.mprio[1].offset_y += y * N.mprio[1].speed_y;
		N.mprio[2].offset_y += y * N.mprio[2].speed_y;
		N.mprio[3].offset_y += y * N.mprio[3].speed_y;
	}
	
	if(N.x+x>=0)
	{
		N.x+=x;
		N.mprio[0].offset_x += x * N.mprio[0].speed_x;
		N.mprio[1].offset_x += x * N.mprio[1].speed_x;
		N.mprio[2].offset_x += x * N.mprio[2].speed_x;
		N.mprio[3].offset_x += x * N.mprio[3].speed_x;
	}
}

void MapSet(int x,  int y, int c)
{
	N.mprio[N.renderer->activelayer].data[(y*N.mprio[N.renderer->activelayer].size_x)+x]=c;
}


void MapClear(void)
{
	int z, x, y;
	
	for(z = 0; z<4; z++)
	{
		N.renderer->select(z);
	
		for(y=0; y<N.mprio[z].size_y; y++)
		{
			for(x=0; x<N.mprio[z].size_x; x++)
			{				
				MapSet(x, y, N.mprio[z].tileset->count-1);				
			}	
		}
	}
}

void MapTick(void)
{
	if(FOCUS==MAP_FRAME)
	{
		if(KeyHold(RIGHT))
		{
		
			MapMove(1, 0);
		} else if(KeyHold(LEFT)) {
		
			MapMove(-1, 0);
		}
	
		if(KeyHold(DOWN))
		{
			
			MapMove(0, 1);
		}else if(KeyHold(UP)) {
		
			MapMove(0, -1);
		}
		
		//MAP_FRAME->redraw=1;
	}
}


void MapInit(void)
{
	
}