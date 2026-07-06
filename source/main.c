#include <Ncommon.h>

int main(void)
{
	Init();
	if(!N.init())return 1;

	Log(" > NGINE: Init complete\n\t  Jump into main loop...\n");
	while(N.status < ES_FINISH)
	{
		if(N.ticksb != N.ticks)
		{
			N.tick();	
		}
		
		Draw();		
	}
	
	Log("\n");
	Log(" > NGINE: Jumped outta the main loop...\n");

	N.deinit();
	return 0;
}
END_OF_MAIN()
