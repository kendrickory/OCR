CC=gcc 

CPPFLAGS= -MMD
CFLAGS = -Wmain -Wextra -Werror -O3 `pkg-config --cflags sdl2 SDL2_image` -g


LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image`



SRC = network_ocr.c

DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: ${PRG}

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}


