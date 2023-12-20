CC = gcc
CFLAGS = -Wall

all: newshell

newshell: newshell.o cd_builtin.o redirection.o alias.o
	$(CC) $(CFLAGS) -o newshell newshell.o cd_builtin.o redirection.o alias.o

newshell.o: newshell.c major2.h
	$(CC) $(CFLAGS) -c newshell.c

cd_builtin.o: cd_builtin.c major2.h
	$(CC) $(CFLAGS) -c cd_builtin.c

redirection.o: redirection.c major2.h
	$(CC) $(CFLAGS) -c redirection.c

alias.o: alias.c major2.h
	$(CC) $(CFLAGS) -c alias.c

clean:
	rm -f *.o newshell
