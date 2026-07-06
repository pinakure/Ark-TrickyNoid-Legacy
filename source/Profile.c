// M4GEEK - Runtime Profiler
// All rights Reserved, 2012
// Written by Mitikoro and Offsetzero
#include <Ncommon.h>

extern long GetCycles(void);

#ifndef NDEBUG
static char* ProfileNames[]={
	#define M(a)		#a,
	#include <ProfileNames.h>
	#undef M	
};

t_profilercontrol Profiler;

static void ProfileClear(void)
{
	int i;

	for(i=0; i<Profiler.count; i++)
	{
		Profiler.entry[i].peak=0;
	}
}			

void InitProfiler(void)
{
	int i;

	Profiler.count = PROFILE_MAX;
	Log(" >  PROF: Initializing %i entries...\n", PROFILE_MAX);
	
	Profiler.entry = malloc(sizeof(t_profileentry)*Profiler.count);
	if(!Profiler.entry)
	{
		__FAIL__
		exit(0);
	}
	Log("\t  Init complete.\n");

	Profiler.active = 0;
	

	//setup profile entries
	for(i=0; i<Profiler.count; i++)
	{
		Profiler.entry[i].avg=0;
		Profiler.entry[i].peak=0;
		Profiler.entry[i].value=0;
		Profiler.entry[i].clock=0;
		Profiler.entry[i].color=makecol(64*(rand()%4),255, 64*(rand()%4));

//		if(i==PROFILE_AVERAGETICK)
		Profiler.entry[i].scale=16;
		
//		else Profiler.entry[i].scale=9;
		Profiler.entry[i].name = ProfileNames[i];
	}	
}

void DeinitProfiler(void)
{
	free(Profiler.entry);
	Profiler.count = 0;
}

void StartProfiler(enum E_ProfileNames index)
{
	Profiler.entry[index].value = -GetCycles();
}

void StopProfiler(enum E_ProfileNames index)
{
	Profiler.entry[index].value += GetCycles();	
}

void TickProfiler(void)
{
	/*PROFILE p;
	int i, c;
	
	if(N.redraw)ProfileClear();
	for(i=0; i<Profiler.count; i++)
	{
		p = &Profiler.entry[i];
		

		p -> avg = (p->value+p ->avg)>>1;
		
		if( p->value > p->peak )
		{
			c = COLOR_RED;
			p -> peak = p->value;
		}
		
	}

	if(FOCUS == PROFILE_FRAME)
	{
		if(KeyDown(DOWN))
		{
			if(Profiler.active<PROFILE_MAX-1)
			Profiler.active++;
			else
			Profiler.active = 0;
		}
		if(KeyDown(UP))
		{
			if(Profiler.active>0)
			Profiler.active--;
			else Profiler.active=PROFILE_MAX-1;
		}	
		if(KeyDown(RIGHT))
		{
			if(Profiler.entry[Profiler.active].scale>0)
			Profiler.entry[Profiler.active].scale--;
		}
		if(KeyDown(LEFT))
		{
			if(Profiler.entry[Profiler.active].scale<24)		
			Profiler.entry[Profiler.active].scale++;
		}
		
		PROFILE_FRAME->redraw = 1;
	}*/
}
void DrawProfiler(void)
{
	/*PROFILE p;
	int i, c, z;

	if(N.redraw)ProfileClear();
	for(i=0; i<Profiler.count; i++)
	{
		p = &Profiler.entry[i];

		if( p->value > p->peak )
		{
			c = COLOR_RED;
		} else {
			c = COLOR_YELLOW;
		}

		Print(PROFILE_FRAME->surf, 8, 4 + i*10, 0 + ((i==Profiler.active)*Color(COLOR_HGREEN)), Color(COLOR_GRAY75), "%9s(%2i):%010u/%010u", p->name, p->scale, p->peak, p->avg);
					
		z = ((unsigned)(p->avg)>>p->scale);
		
		if(z<PROFILE_FRAME->w-207)
		{
			line(PROFILE_FRAME->surf, 185,	4 + (i*10)+p->clock, 
							185 + z, 4 + (i*10)+p->clock, 
							p->color); 
			line(PROFILE_FRAME->surf, 185 + z,	4 + (i*10)+p->clock, 
							PROFILE_WINDOW->w-22, 4 + (i*10)+p->clock, 
							Color(COLOR_GREEN)); 
		} else {
			line(PROFILE_FRAME->surf, 185 ,	4 + (i*10)+p->clock, 
							PROFILE_WINDOW->w-22, 4 + (i*10)+p->clock, 
							Color(COLOR_HRED)); 
		}
		
		
		p->clock++;	
		p->clock&=7;
	}*/
}
#endif

