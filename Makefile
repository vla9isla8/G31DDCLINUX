all: demo

demo: main.o window.o
	gcc main.o -lm -ldl -lcurses -lpthread -lstdc++  window.o `pkg-config --libs gtk+-3.0` -o demo  
	
main.o: main.c
	gcc -c main.c

window.o: window.c
	gcc `pkg-config --cflags gtk+-3.0` -c window.c
	
clean:
	rm -f demo *.o

