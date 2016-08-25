PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo06.app

demo06.app: demo06.cpp
	$(CC) demo06.cpp $(CFLAGS) -o demo06.app -linkview -g -gdwarf-3

clean:
	rm -f demo06.app
