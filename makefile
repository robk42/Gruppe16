# Makefile für das Tic-Tac-Toe Netzwerkprojekt

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LIBS = -lncurses -ltinfo


TARGET = tic_tac_toe
SRC = tic_tac_toe.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
