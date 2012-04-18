all: 
	cc main.c Executor.c Process.c -o executor -Wall
	rm -rf bin$
	mkdir bin
	mv executor bin
	
clean:
	rm -rf *o bin/executor
	rm -rf bin/*
