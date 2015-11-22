all: shell.c shells.h
	gcc shell.c -o c-shell
run:
	./c-shell
clean:
	rm *~
	rm *.o

