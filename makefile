all: main

main: main.c
	cc main.c -o main

clean:
	rm log main
