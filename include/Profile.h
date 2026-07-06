// M4GEEK - Runtime Profiler
// All rights Reserved, 2012
// Written by Mitikoro and Offsetzero
// Header file.
#ifndef __PROFILE_H
#define __PROFILE_H

typedef struct s_profileentry* PROFILE;
typedef struct s_profileentry
{
	int peak;
	int avg;
	int value;
	int scale;
	int color;
	int clock;
	char *name;
}t_profileentry;

typedef struct s_profilercontrol
{
	int count;
	int active;
	PROFILE entry;
}t_profilercontrol;

enum E_ProfileNames
{
	#define M(a)		PROFILE_##a,
	#include <ProfileNames.h>
	#undef M
	PROFILE_MAX
};

extern long GetCycles(void);

#ifndef NDEBUG
void StartProfiler(enum E_ProfileNames index);
void StopProfiler(enum E_ProfileNames index);
void DrawProfiler(void);
void TickProfiler(void);
void DeinitProfiler(void);
void InitProfiler(void);
#else 
#define StartProfiler(a)
#define StopProfiler(a)
#define TickProfiler()
#define DrawProfiler()
#define DeinitProfiler()
#define InitProfiler()
#endif

#endif