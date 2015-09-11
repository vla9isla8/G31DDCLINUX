all: demo

demo: main.o  radio.o
	gcc main.o radio.o -lm -ldl -lcurses -lpthread -lstdc++  `pkg-config --libs gtk+-3.0` -o demo  -g
	
main.o: main.c
	gcc `pkg-config --cflags gtk+-3.0` -c main.c -g

radio.o:
	gcc -c radio.c -g

clean:
	rm -f demo *.o

