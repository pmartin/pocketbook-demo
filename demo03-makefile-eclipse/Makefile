PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

all: demo03.app

demo03.app: demo03.cpp
	$(CC) demo03.cpp -o demo03.app -linkview -g -gdwarf-3

clean:
	rm -f demo03.app
