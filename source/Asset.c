#include <Ncommon.h>

static int *AssetID = NULL;		// id array
static int *AssetPTR = NULL;	// pointer array
static int *AssetSIZE = NULL;	// size array
static int *AssetCOMP = NULL;	// size array

static void *AssetDATA = NULL;

static int AssetCOUNT = 0;
static int AssetTSIZE = 0;

int Compare(const void *p1,  const void *p2)
{
	if( *(int*)p2 > *(int*)p1)return -1;	
	if( *(int*)p1 > *(int*)p2)return 1;
	return 0;
}

void InitAsset(char *file)
{
	FILE *fp;
	
	int i;
	int magic;
	int version;
	
	Log(" > ASSET: Loading '%s'...\n", file);
	
	//open asset file
	fp = fopen(file, "rb");
	if(!fp)
	{
		Log(" > ASSET: FATAL: Couln't load \"%s\"!\n", file);
		exit(666);
	}
	
	//read file header	
	fread(&magic, 4, 1, fp);/*check magic number*/
	fread(&version, 4, 1, fp);
	fread(&AssetCOUNT, 4, 1, fp);
	fread(&AssetTSIZE, 4, 1, fp);
	
	if(magic == 0x1A1A1A1A)Log("\t  Signature Passed, filetype seems correct...\n");
	else {
		Log(" > ASSET: FATAL: Signature is incorrect!\n");
		exit(666);
	}
	
	Log("\t  %i items found!...\n", AssetCOUNT);
	
	//create memory for storing all the existing pointers
	AssetID  = malloc(AssetCOUNT*4);
	AssetCOMP= malloc(AssetCOUNT*4);
	AssetSIZE= malloc(AssetCOUNT*4);
	AssetPTR = malloc(AssetCOUNT*4);
	
	for(i=0; i<AssetCOUNT; i++)
	{
		fread(&AssetID[i], 4, 1, fp);
		fread(&AssetCOMP[i], 4, 1, fp);
		fread(&AssetSIZE[i], 4, 1, fp);
		fread(&AssetPTR[i], 4, 1, fp);
	}
	
	//load raw data
	Log("\t  Loading raw data (%i bytes)...\n", AssetTSIZE-(16+(16*AssetCOUNT)));
	AssetDATA = malloc(AssetTSIZE-(16+(16*AssetCOUNT)));
	if(!AssetDATA)
	{
		Log(" > ASSET: PANIC: Not enough memory!!\n");
		return;
	}
	fread(AssetDATA, AssetTSIZE-(16+(16*AssetCOUNT)), 1, fp);
	
	//unload file
	fclose(fp);
	
	Log("\t  Sucessfully loaded!\n");
}

void DeinitAsset(void)
{
	Log(" > ASSET: Release raw memory...\n");
	//destroy asset memory
	if(AssetID)free(AssetID);
	if(AssetDATA)free(AssetDATA);
	if(AssetSIZE)free(AssetSIZE);
	if(AssetPTR)free(AssetPTR);
	
	AssetID = NULL;
	AssetPTR = NULL;
	AssetDATA = NULL;
	AssetSIZE = NULL;
	AssetTSIZE = 0;
	AssetCOUNT = 0;
	Log("\t  Deinit complete.\n");
}

int GetAssetSize(int ident)
{
	void *ptr;
	int i;
	
	//returns given identifier address (if exist)
	ptr = bsearch(&ident, AssetID, AssetCOUNT, 4, Compare);
	if(!ptr)
	{
		Log(" > ASSET: FATAL: Couldn't load asset item #%i\n", ident);
		exit(555);
	}
	
	
	i = ( (int)( (int*)ptr-AssetID)/*/sizeof(int*)*/);
	
	return AssetSIZE[i];
}

void *GetAsset(int ident)
{
	void *ptr;
	int i;
	
	//returns given identifier address (if exist)
	ptr = bsearch(&ident, AssetID, AssetCOUNT, 4, Compare);
	if(!ptr)
	{
		Log(" > ASSET: FATAL: Couldn't load asset item #%i\n", ident);
		exit(1);
	}
	
	
	i = ( (int)( (int*)ptr-AssetID)/*/sizeof(int*)*/);
	
	return (char*)AssetDATA + AssetPTR[i] -(16+(16*AssetCOUNT));
}

