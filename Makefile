CC = gcc
INCLUDE = -Iinclude

all: bin/client_static

bin/client_static: obj/main.o lib/libmyutils.a
	$(CC) obj/main.o -Llib -lmyutils -o bin/client_static

lib/libmyutils.a: obj/mystrfunctions.o obj/myfilefunctions.o
	ar rcs lib/libmyutils.a obj/mystrfunctions.o obj/myfilefunctions.o

obj/main.o: src/main.c
	$(CC) $(INCLUDE) -c src/main.c -o obj/main.o

obj/mystrfunctions.o: src/mystrfunctions.c
	$(CC) $(INCLUDE) -c src/mystrfunctions.c -o obj/mystrfunctions.o

obj/myfilefunctions.o: src/myfilefunctions.c
	$(CC) $(INCLUDE) -c src/myfilefunctions.c -o obj/myfilefunctions.o

clean:
	rm -f obj/*.o bin/client_static lib/libmyutils.a
