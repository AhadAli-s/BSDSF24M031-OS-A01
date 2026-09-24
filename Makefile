CC = gcc
INCLUDE = -Iinclude

all: bin/client_static bin/client_dynamic

# Static build
bin/client_static: obj/main.o lib/libmyutils.a
	$(CC) obj/main.o -Llib -lmyutils -o bin/client_static

lib/libmyutils.a: obj/mystrfunctions.o obj/myfilefunctions.o
	ar rcs lib/libmyutils.a obj/mystrfunctions.o obj/myfilefunctions.o

# Dynamic build
bin/client_dynamic: obj/main.o lib/libmyutils.so
	$(CC) obj/main.o -Llib -lmyutils -o bin/client_dynamic

lib/libmyutils.so: obj/mystrfunctions_pic.o obj/myfilefunctions_pic.o
	$(CC) --shared obj/mystrfunctions_pic.o obj/myfilefunctions_pic.o -o lib/libmyutils.so

# Regular object files (used for static build)
obj/main.o: src/main.c
	$(CC) $(INCLUDE) -c src/main.c -o obj/main.o

obj/mystrfunctions.o: src/mystrfunctions.c
	$(CC) $(INCLUDE) -c src/mystrfunctions.c -o obj/mystrfunctions.o

obj/myfilefunctions.o: src/myfilefunctions.c
	$(CC) $(INCLUDE) -c src/myfilefunctions.c -o obj/myfilefunctions.o

# PIC object files (used for dynamic build)
obj/mystrfunctions_pic.o: src/mystrfunctions.c
	$(CC) $(INCLUDE) -fPIC -c src/mystrfunctions.c -o obj/mystrfunctions_pic.o

obj/myfilefunctions_pic.o: src/myfilefunctions.c
	$(CC) $(INCLUDE) -fPIC -c src/myfilefunctions.c -o obj/myfilefunctions_pic.o

clean:
	rm -f obj/*.o bin/client_static bin/client_dynamic lib/libmyutils.a lib/libmyutils.so
