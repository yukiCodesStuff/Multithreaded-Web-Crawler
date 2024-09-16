clean:
	rm -rf main

all: main objects

main:
	g++ main.cpp -o main

objects:
	g++ /objects/*
