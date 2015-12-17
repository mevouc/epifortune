CC=clang

CPPFLAGS=
CFLAGS= -Wall -Wextra -pedantic -std=c99 -D_XOPEN_SOURCE=600 -O0 -g
LDFLAGS=
LDLIBS=

SRC= html_operations.c epifortune.c 
OBJ=${SRC:.c=.o}
OUT= epifortune

all: ${OBJ}
	${CC} ${OBJ} -o ${OUT}

clean:
	rm -f *~ *.o
	rm -f ${OUT}

# END
