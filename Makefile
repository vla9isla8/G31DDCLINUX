all: demo

demo: main.c
	gcc  main.c -lm -ldl -lcurses -lpthread -lstdc++  -o demo

clean:
	rm -f demo
