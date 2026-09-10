all:
	gcc -ggdb -o storm storm_main.c storm.c -lX11 
	./storm


