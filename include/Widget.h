#ifndef __WIDGET_H
#define __WIDGET_H

#define GetWidgets(a, b)	((a)(b->data))

#define AllocWidgets(type, dst, parent,tag)									\
			Log(">"#tag": "#parent" %p allocating "#type"...\n", parent);	\
			parent->data = malloc(sizeof(type));							\
			dst = GetWidgets(type*, parent); 								\
			Log("\t  "#tag"->"#type" allocated in %p...\n", dst);


typedef struct s_widget* WIDGET;
typedef struct s_widget
{
	NGWTYPE type;
	int redraw;			// Specifies if widget needs to be reblitted
	int retick;			// Specifies if widget tick function needs to be called
	int x; 				// horizontal position, always relative with parent
	int y; 				// horizontal position, always relative with parent
	int w; 				// widget width
	int h; 				// widget height
	int value;			// widget value
	char *caption;		// widget caption	
	//void (*init)(WIDGET); //init process hook
	//void (*deinit)(WIDGET); //deinit process hook
	//void (*render)(WIDGET); //PRErender process hook
	void (*tick)(WIDGET);	//Tick function hook
	void *surf; 		//video surface used to draw pixels to and read colors from
	void *data;			//auxiliar pointer to bind data to
	NGWSTATUS status;
	WIDGET parent;		//
	WIDGET child;		//direct child
	WIDGET next;		//sibiling	
	int childcount;	
}t_widget;

typedef struct s_guiwidgets* GUIWIDGETS;
typedef struct s_guiwidgets
{
	WIDGET  toolbar;
	WIDGET  profiler;
	WIDGET	tileset;
	WIDGET  sprites;
	WIDGET	map;
	WIDGET  editor;
}t_guiwidgets;

typedef struct s_buttonwidgets* BUTTONWIDGETS;
typedef struct s_buttonwidgets
{
	void *image[3];  //The 3 different states the button can have	
	void(*hook)(int param); //function hook
	int *var;		   //variable to make switch
	int value;
}t_buttonwidgets;


typedef struct s_winwidgets* WINWIDGETS;
typedef struct s_winwidgets
{
	WIDGET	title;
	WIDGET	menu;
	WIDGET  frame;
	WIDGET  statusbar;
}t_winwidgets;

typedef struct s_framewidgets* FRAMEWIDGETS;
typedef struct s_framewidgets
{
	int	bevel;
	int backcolor;
	int fgcolor;	
}t_framewidgets;

typedef struct s_toolbarwidgets* TOOLBARWIDGETS;
typedef struct s_toolbarwidgets
{
	WIDGET *button;
	TILESET icons;
	int count;	
}t_toolbarwidgets;

WIDGET CreateWidget(NGWTYPE type, int x, int y, int w, int h, char *caption, WIDGET parent, void *data);
void DestroyWidget(WIDGET w);
void DisableWidget(WIDGET w, int recurse);
void EnableWidget(WIDGET w, int recurse);
void SetFocus(WIDGET focus);

void WIDGET_FrameInit(WIDGET me);
void WIDGET_WindowInit(WIDGET me);
void WIDGET_ButtonInit(WIDGET me);
void WIDGET_ToolbarInit(WIDGET me);
void WIDGET_DesktopInit(WIDGET me);
void WIDGET_FrameDeinit(WIDGET me);
void WIDGET_WindowDeinit(WIDGET me);
void WIDGET_ButtonDeinit(WIDGET me);
void WIDGET_ToolbarDeinit(WIDGET me);
void WIDGET_DesktopDeinit(WIDGET me);
void WIDGET_FrameRender(WIDGET me);
void WIDGET_WindowRender(WIDGET me);
void WIDGET_ButtonRender(WIDGET me);
void WIDGET_ToolbarRender(WIDGET me);
void WIDGET_DesktopRender(WIDGET me);
void WIDGET_FrameTick(WIDGET me);
void WIDGET_DesktopTick(WIDGET me);
void WIDGET_WindowTick(WIDGET me);
void WIDGET_ToolbarTick(WIDGET me);
void WIDGET_ButtonTick(WIDGET me);


extern WIDGET FOCUS;
extern WIDGET GUI;
#endif
