PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

all: demo04.app

demo04.app: demo04.cpp
	$(CC) demo04.cpp -o demo04.app -linkview -g -gdwarf-3

clean:
	rm -f demo04.app
