EXECUTABLE=shell
CFLAGS= -Wall -g

shell.o: main.c
	gcc $(CFLAGS) -o $(EXECUTABLE) headers.h lista.h lista.c p0.c p0.h p1.c p1.h main.c
