#include "spiellogik.h"
#include <stdlib.h>

char board[BOARD_SIZE];

void initBoard(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = ' ';
    }
}

char checkWin(void) {
    // Zeilen prüfen
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] != ' ' &&
            board[i * 3] == board[i * 3 + 1] &&
            board[i * 3 + 1] == board[i * 3 + 2])
            return board[i * 3];
    }
    // Spalten prüfen
    for (int i = 0; i < 3; i++) {
        if (board[i] != ' ' &&
            board[i] == board[i + 3] &&
            board[i + 3] == board[i + 6])
            return board[i];
    }
    // Diagonalen prüfen
    if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8])
        return board[0];
    if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6])
        return board[2];
    return ' ';
}

int checkDraw(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            return 0;
    }
    return 1;
}

int isValidMove(int move) {
    if (move < 0 || move >= BOARD_SIZE)
        return 0;
    if (board[move] != ' ')
        return 0;
    return 1;
}

void makeMove(int move, char mark) {
    if (move >= 0 && move < BOARD_SIZE)
        board[move] = mark;
}

int getAIMove(void) {
    int available[BOARD_SIZE];
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            available[count++] = i;
    }
    if (count == 0)
        return -1;
    return available[rand() % count];
}
