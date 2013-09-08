#==stuff linked to
DYNAMIC = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL -lm
#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
CFLAGS = -O3 -fsingle-precision-constant -fPIC
# Testtweaks: -fgcse-lm -fgcse-sm -fsched-spec-load -fmodulo-sched -funsafe-loop-optimizations -Wunsafe-loop-optimizations -fgcse-las -fgcse-after-reload -fvariable-expansion-in-unroller -ftracer -fbranch-target-load-optimize
GENERAL_TWEAKS = -ffast-math
#==PC==
CPP = gcc -g -march=native -DX86CPU $(GENERAL_TWEAKS)
SDL = `sdl-config --cflags`

SPARROW_FOLDER = ../sparrow3d

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

all: Sparrow-C4A-Manager
	@echo "=== Built for Target "$(TARGET)" ==="

targets:
	@echo "The targets are the same like for sparrow3d. :P"

Sparrow-C4A-Manager: Sparrow-C4A-Manager.c account.o menu.o highscore.o defines.h makeBuildDir
	cp $(SPARROW_LIB)/libsparrow3d.so $(BUILD)
	cp $(SPARROW_LIB)/libsparrowNet.so $(BUILD)
	$(CPP) $(CFLAGS) Sparrow-C4A-Manager.c account.o menu.o highscore.o $(SDL) $(INCLUDE) $(LIB) $(STATIC) $(DYNAMIC) -o $(BUILD)/Sparrow-C4A-Manager

makeBuildDir:
	 @if [ ! -d $(BUILD:/Sparrow-C4A-Manager=/) ]; then mkdir $(BUILD:/Sparrow-C4A-Manager=/);fi
	 @if [ ! -d $(BUILD) ]; then mkdir $(BUILD);fi

account.o: account.c account.h defines.h
	$(CPP) $(CFLAGS) -c account.c $(SDL) $(INCLUDE)

highscore.o: highscore.c highscore.h defines.h
	$(CPP) $(CFLAGS) -c highscore.c $(SDL) $(INCLUDE)

menu.o: menu.c menu.h defines.h
	$(CPP) $(CFLAGS) -c menu.c $(SDL) $(INCLUDE)

clean:
	rm -f *.o
	rm -f Sparrow-C4A-Manager
