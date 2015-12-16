CC=clang

CPPFLAGS=
CFLAGS= -Wall -Wextra -pedantic -std=c99 -O3
LDFLAGS=
LDLIBS=

SRC= epifortune.c 
OBJ=${SRC:.c=.o}
OUT= epifortune

all: ${OBJ}
	${CC} ${OBJ} -o ${OUT}

clean:
	rm -f *~ *.o
	rm -f ${OUT}

# END
