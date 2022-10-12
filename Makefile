all: shell

list.o : lista.c lista.h
	gcc -c lista.c
shell.o: p1.c lista.h
	gcc -c p1.c
shell: shell.o list.o
	gcc -o shell p1.o lista.o
clean:
	rm -f p1.o lista.o shell