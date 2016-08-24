PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

all: demo05.app

demo05.app: demo05.cpp
	$(CC) demo05.cpp -o demo05.app -linkview -g -gdwarf-3

clean:
	rm -f demo05.app
