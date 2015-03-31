#==stuff linked to
DYNAMIC = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL -lm
#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
CFLAGS = -O3 -fsingle-precision-constant -fPIC
# Testtweaks: -fgcse-lm -fgcse-sm -fsched-spec-load -fmodulo-sched -funsafe-loop-optimizations -Wunsafe-loop-optimizations -fgcse-las -fgcse-after-reload -fvariable-expansion-in-unroller -ftracer -fbranch-target-load-optimize
GENERAL_TWEAKS = -ffast-math
#==PC==
FLAGS = -g -DDESKTOP $(GENERAL_TWEAKS)
SDL = `sdl-config --cflags`

SPARROW_FOLDER = ../sparrow3d

SPARROW3D_LIB = libsparrow3d.so
SPARROWNET_LIB = libsparrowNet.so
SPARROWSOUND_LIB = libsparrowSound.so

ifdef TARGET
include $(SPARROW_FOLDER)/target-files/$(TARGET).mk
BUILD = ./build/$(TARGET)/Sparrow-C4A-Manager
SPARROW_LIB = $(SPARROW_FOLDER)/build/$(TARGET)/sparrow3d
else
TARGET = "Default (change with make TARGET=otherTarget. See All targets with make targets)"
BUILD = .
SPARROW_LIB = $(SPARROW_FOLDER)
endif
LIB += -L$(SPARROW_LIB)
INCLUDE += -I$(SPARROW_FOLDER)
DYNAMIC += -lsparrow3d -lsparrowNet

CFLAGS += $(PARAMETER) $(FLAGS)

all: Sparrow-C4A-Manager
	@echo "=== Built for Target "$(TARGET)" ==="

targets:
	@echo "The targets are the same like for sparrow3d. :P"

Sparrow-C4A-Manager: Sparrow-C4A-Manager.c account.o menu.o highscore.o defines.h makeBuildDir
	cp $(SPARROW_LIB)/$(SPARROW3D_LIB) $(BUILD)
	cp $(SPARROW_LIB)/$(SPARROWNET_LIB) $(BUILD)
	$(CC) $(CFLAGS) $(LINK_FLAGS) $< account.o menu.o highscore.o $(SDL) $(INCLUDE) $(LIB) $(STATIC) $(DYNAMIC) -o $(BUILD)/$@$(SUFFIX)

makeBuildDir:
	 @if [ ! -d $(BUILD:/Sparrow-C4A-Manager=/) ]; then mkdir $(BUILD:/Sparrow-C4A-Manager=/);fi
	 @if [ ! -d $(BUILD) ]; then mkdir $(BUILD);fi

account.o: account.c account.h defines.h
	$(CC) $(CFLAGS) -c account.c $(SDL) $(INCLUDE)

highscore.o: highscore.c highscore.h defines.h
	$(CC) $(CFLAGS) -c highscore.c $(SDL) $(INCLUDE)

menu.o: menu.c menu.h defines.h
	$(CC) $(CFLAGS) -c menu.c $(SDL) $(INCLUDE)

clean:
	rm -f *.o
	rm -f Sparrow-C4A-Manager

oclean:
	rm -f *.o
