CC=g++
CFLAGS=-Wall -O2
DBFLAGS=-g -Wall

debug: main.cpp
	$(CC) $(DBFLAGS) $< -o build/cfront

clean:
	rm build/cfront

build: main.cpp
	$(CC) $(CFLAGS) $< -o build/cfront

install: main.cpp
	$(CC) $(CFLAGS) $< -o build/cfront
	cp build/cfront /usr/bin/