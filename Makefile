# main make file

# library directories
LDIR=-Llib -Ilib

# libraries
ifdef SYSTEMROOT
	#windows libraries
	LIBS= -lglfw3_win -lgdi32 -lopengl32
else
	#linux libraries
	LIBS= -lglfw3_linux -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lXxf86vm -lXinerama -lXcursor -lrt
endif

IDIR=src
CC=gcc
#CFLAGS=-Wall -msse3 -g -I$(IDIR) $(LDIR) $(LIBS)
CFLAGS=-Wall -msse3 -O3 -I$(IDIR) $(LDIR) $(LIBS)

# hide .o files in obj directory
ODIR=obj

_DEPS = render/sprite.h render/shader.h render/texman.h phys.h helper/list.h const.h game.h helper/easing.h event.h player.h obstacle.h func.h proj.h target.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o render/shader.o render/sprite.o helper/glad.o render/texman.o phys.o helper/list.o game.o helper/easing.o event.o player.o obstacle.o func.o proj.o target.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# tells make to check include directory for dependencies
# without this, you get a 'cannot find name.o' error
VPATH = $(IDIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	find ./$(ODIR) -name "*.o" -delete

