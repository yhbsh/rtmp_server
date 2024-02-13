all: main

vim_logs: run
	vi -c '%!xxd -b' log

run:
	./main


main: main.c
	cc main.c -o main

clean:
	rm log main
