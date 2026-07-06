#include <Ncommon.h>

char InputCheckKeyStatus(char dest, int source)
{
	switch(dest)
	{
		case KEYDOWN:
			if(source)
			{		
				return KEYHOLD;
			}
			return KEYUP;			
		
		case KEYHOLD:			
			if(source)return KEYHOLD;
			return KEYUP;
		
		case KEYUP:
			if(source)return KEYDOWN;			
			return KEYOFF;

		case KEYOFF:
			if(source)return KEYDOWN;
			return KEYOFF;

		default:
			return KEYOFF;
	}	
}

int CursorOver(int x, int y, int w, int h)
{
	//DrawRectangleB(screen, x, y, w, h, Palette[COLOR_GREEN]);
	if(*N.input->device->A1X>x){
		if(*N.input->device->A1X<x+w){
			if(*N.input->device->A1Y>y){
				if(*N.input->device->A1Y<y+h)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}