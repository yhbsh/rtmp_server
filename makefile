all: clean main run

run:
	./main
	vi -c '%!xxd -b' log

main: main.c
	cc main.c -o main

clean:
	rm log main
