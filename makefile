PROJECT=mapic
OBJECT=$(OBJECTDIR)Ncommon.o \
	$(OBJECTDIR)Main.o \
	$(OBJECTDIR)Asset.o \
	$(OBJECTDIR)AllRenderer.o \
	$(OBJECTDIR)AllInput.o \
	$(OBJECTDIR)Input.o \
	$(OBJECTDIR)Map.o \
	$(OBJECTDIR)Renderer.o \
	$(OBJECTDIR)Sprite.o \
	$(OBJECTDIR)Tileset.o \
	$(OBJECTDIR)Mapic.o \
	$(OBJECTDIR)TurboFX.o \
	$(OBJECTDIR)Gui.o
HLINE=---------------------------------------------------
PACK=./pack

ifdef NDEBUG
PROGRAM=$(PROJECT)_release
OBJECTDIR=release/
CFLAGS=`allegro-config --cflags` -DLINUX=1 -DNDEBUG=1 -O3
LDFLAGS=`allegro-config --libs` -O3

$(OBJECTDIR)%.o : %.c
	@echo Compiling $<...
	@colorgcc $(CFLAGS) -c $< -o $@

$(OBJECTDIR)%.o : %.asm
	@echo Assembling $<...
	@nasm -felf $< -o $@

else
# DEBUG COMPILE
PROGRAM=$(PROJECT)_debug
OBJECTDIR=debug/
CFLAGS=`allegro-config --cflags` -DLINUX=1 -D_DEBUG=1 -g -Wall -Wextra
LDFLAGS=`allegro-config --libs` -g

OBJECT:=$(OBJECT) \
	$(OBJECTDIR)Profile.o

$(OBJECTDIR)%.o : %.c
	@echo Compiling $<...
	@colorgcc $(CFLAGS) -c $< -o $@

$(OBJECTDIR)%.o : %.asm
	@echo Assembling $<...
	@nasm -felf $< -o $@

endif

all : $(PROGRAM)

$(PROGRAM) : $(OBJECT)
	@echo $(HLINE)
	@echo Linking $(PROGRAM)...
	@colorgcc $(LDFLAGS) -o $(PROGRAM) $(OBJECT)
	@ls -l $(PROGRAM)

clean :
	@echo Cleaning project...
	@rm -f $(OBJECT) $(PROGRAM)

run :
	@./$(PROGRAM)

