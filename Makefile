all: shell

list.o : lista.c lista.h
	gcc -c lista.c
shell.o: p0.c lista.h
	gcc -c p0.c
shell: shell.o list.o
	gcc -o shell p0.o lista.o
clean:
	rm -f p0.o lista.o shell