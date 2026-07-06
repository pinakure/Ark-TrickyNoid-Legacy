#include <Ncommon.h>

static void IdleQueueAddSprite(SPRITE spr, int queue)
{
	SPRITE q;
	
	q = &(*N.sprio[queue]);
	
	Log(" >SPRITE: Adding sprite (%p) into queue %i [%p]", spr, queue, N.sprio[queue]);		
	
	if(!q)
	{
		//first item in the list!!
		Log(" as first item!\n");
		N.sprio[queue] = spr;
		return;
	}
		
	while(q->next)
	{
		q=q->next;
	}	
	
	Log(", next to %p!\n", q);
	spr->prev = q;
	q->next = spr;
	
}

static SPRITE GetLastSprite(SPRITE spr)
{
	if(!spr)return NULL;
	while(spr->next)
	{
		spr=spr->next;
	}
	return spr;
}

void SpriteSetPriority(SPRITE spr, int prio)
{
	SPRITE tq;//target queue
	
	tq = GetLastSprite(N.sprio[prio]);
	
	if(spr->prev)
	{
		//source sprite was not first in his queue
		spr->prev->next = spr->next;//erase gap
	} else {
		//item is first! check if it's last too!
		if(!spr->next)
		{
			//it is, then set queue to null;
			N.sprio[spr->priority]=NULL;
		} else {
			N.sprio[spr->priority]=spr->next;
		}
	} 
	
	//source sprite was first in his queue
	if(spr->next)
	{
		//but not last item
		spr->next->prev = spr->prev;
	}
	spr->next = NULL;
	spr->prev = NULL;
	
	if(!tq)
	{
		N.sprio[prio] = spr;				
		//moving to empty queue!		
	} else {
		tq->next = spr;
		spr->prev = tq->next;
	}
	
	spr->priority = prio;
}

static void DestroySprite(SPRITE spr)
{
	int f;
	
	ASSERT(spr);
	
	for(f=0; f<spr->frames ; f++)
	{
		destroy_bitmap(((BITMAP**)spr->data)[f]);
	}	
}

void DeinitSprites(void)
{
	SPRITE q;
	SPRITE n;
	int z;
	
	Log(" >SPRITE: Deinitialize system...\n");
	
	for(z=0;z<5;z++)
	{
		 Log("\t  Destroying priority queue #%i...\n", z);
		 
		 q = N.sprio[z];
		 while(q)
		 {
			n = q->next;
			Log("{%p}...", q);
			DestroySprite(q);
			free(q);
			q = n;
		 }		 
	}
	Log("\t  Deinit complete.\n");
}

static SPRITE CreateSprite(int hsize, int vsize, FPSTYPE index, int frames, int prio)
{
	SPRITE spr;
	
	//Log("Allocating sprite memory...");
	spr = malloc(sizeof(t_sprite));
	if(!spr)
	{
		Log(" >SPRITE: FATAL: Not enough memory!\n");
		exit(666);
	}
	//setup sprite
	spr->next = NULL;
	spr->prev = NULL;
	
	spr->priority = prio;
	
	spr->x=VWIDTH>>1;
	spr->y=VHEIGHT>>1;
	spr->width=hsize;
	spr->height=vsize;
	
	spr->frame = 0;
	spr->frames = frames;
	
	spr->animation = 0;
	spr->reverseanim = 0;
	spr->fps = &N.clock[index];
	
	spr->hflip = 0;
	spr->vflip = 0;
	
	spr->status = STATUS_IDLE;
	
	//Put sprite on the idle sprite list	
	return spr;
}

SPRITE SpriteFromBitmap(BITMAP *source, int hsize, int vsize, FPSTYPE index)
{
	SPRITE spr;
	int count;
	int x;
	int y;
	int h;
	int w;
	
	h = source->h;
	w = source->w;
	count = (h/vsize)*(w/hsize);
	
	spr = CreateSprite(hsize, vsize, index, count, 4);
	if(!spr)return NULL;
	
	
	spr->data = malloc(count*sizeof(BITMAP *));
	if(!spr->data)
	{
		Log(" >SPRITE: FATAL: Not enough memory for %i frames!\n", count);
		exit(666);
	}
	//__OK__
	
	IdleQueueAddSprite(spr, 4);
	
	//Log("Trimming %ix%i bitmap in %ix%i slices (%i frames)...", source->w, source->h, hsize,vsize, count);
	count = 0;
	for(y = 0;y < h;y += vsize)
	{
		for(x = 0;x < w;x += hsize)
		{
			((BITMAP **)(spr->data))[count] = create_bitmap(w, h);
			blit(source, ((BITMAP **)(spr->data))[count], x, y, 0, 0, hsize, vsize);
			count++;
		}
	}
	//__OK__
	
	return spr;
}

void SpriteQueueDraw(int z)
{
	SPRITE s;
	
	s = N.sprio[z];
	
	while(s)
	{
		N.renderer->drawsprite(s);
		s->frame=*s->fps;
		s->frame%=s->frames;
		s=s->next;
	}
}

void SpriteTick(void)
{
		
}

