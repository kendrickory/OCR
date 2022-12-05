CC=gcc

CPPFLAGS= -MMD
CFLAGS = -Wmain -Wextra -Werror

LDFLAGS=
LDLIBS= -lm

SRC = xor_neural_net.c

DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: ${PRG}

-include ${DEP}

clean:
	rm -f *.o
	rm -f ${DEP}
	rm -f ${PRG}


