# Makefile für das Tic Tac Toe Projekt

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LIBS = -lncurses -ltinfo
TARGET = tic_tac_toe

SOURCES = main.c spiellogik.c netzwerk.c gui.c stats.c
HEADERS = spiellogik.h netzwerk.h gui.h stats.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)
