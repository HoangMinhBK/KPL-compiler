CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: main

main: scanner.o reader.o charcode.o token.o error.o parser.o main.o
	${CC} scanner.o reader.o charcode.o token.o error.o parser.o  main.o -o main

reader.o: reader.c
	${CC} ${CFLAGS} reader.c

scanner.o: scanner.c
	${CC} ${CFLAGS} scanner.c

charcode.o: charcode.c
	${CC} ${CFLAGS} charcode.c

token.o: token.c
	${CC} ${CFLAGS} token.c

error.o: error.c
	${CC} ${CFLAGS} error.c

parser.o: parser.o
	${CC} ${CFLAGS} parser.c

main.o: main.o
	${CC} ${CFLAGS} main.c

clean:
	rm -f *.o *~
	


