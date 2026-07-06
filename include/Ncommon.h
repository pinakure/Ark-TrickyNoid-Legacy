#ifndef __NCOMMON_H
#define __NCOMMON_H

#include <stdio.h>
#include <allegro.h>

#define Color(a)			VPAL[a]
#define Log(...)		printf(__VA_ARGS__)
#define __OK__			Log("OK!\n"); 
#define __FAIL__		Log("FAIL!\n"); 

/* Dummy functions declarations */
void Init(void);

void __void0(void);
void __void1(int _1);
void __void2(int _1, int _2);
void __void3(int _1, int _2, int _3);
int __int0(void);
int __int1(int _1);
int __int2(int _1, int _2);
int __int3(int _1, int _2, int _3);

char *concat(char *str1, char *str2);
int GetRand3(void);
int GetRand(void);

void InitAsset(char *file);
void DeinitAsset(void);
void *GetAsset(int ident);
int GetAssetSize(int ident);

#include <Asset.h>
#include <Map.h>
#include <Profile.h>
#include <Tileset.h>
#include <Sprite.h>
#include <Input.h>
#include <Renderer.h>
#include <Environment.h>
#include <Engine.h>
#include <Gui.h>
#include <Widget.h>
#include <Mapic.h>

#endif
