all: file.c data.c main.c
	gcc -c file.c
	gcc -c data.c
	gcc -c main.c
	ar cr libtest.a *.o
	gcc -o main main.o libtest.a
clean:
	rm *.o $(project)
