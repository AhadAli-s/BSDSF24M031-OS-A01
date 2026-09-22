CC = gcc
INCLUDE = -Iinclude

all: bin/client

bin/client: obj/main.o obj/mystrfunctions.o obj/myfilefunctions.o
	$(CC) obj/main.o obj/mystrfunctions.o obj/myfilefunctions.o -o bin/client

obj/main.o: src/main.c
	$(CC) $(INCLUDE) -c src/main.c -o obj/main.o

obj/mystrfunctions.o: src/mystrfunctions.c
	$(CC) $(INCLUDE) -c src/mystrfunctions.c -o obj/mystrfunctions.o

obj/myfilefunctions.o: src/myfilefunctions.c
	$(CC) $(INCLUDE) -c src/myfilefunctions.c -o obj/myfilefunctions.o

clean:
	rm -f obj/*.o bin/client
