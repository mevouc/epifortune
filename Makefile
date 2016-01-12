# Makefile for epifortune

CC=gcc

CPPFLAGS=
CFLAGS= -Wall -Wextra -pedantic -std=c99 -D_XOPEN_SOURCE=600 -O3
LDFLAGS=
LDLIBS=

SRC= src/html_operations.c src/quotation.c src/epifortune.c
OBJ=${SRC:.c=.o}
OUT= epifortune

all: ${OBJ}
	${CC} ${OBJ} -o ${OUT}

clean:
	rm -f ${OBJ}
	rm -f ${OUT}

# END
