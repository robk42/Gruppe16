#include "spiellogik.h"
#include <stdlib.h>

char board[BOARD_SIZE];  // Definition des globalen Spielfeld-Arrays

// Initialisiert das Spielfeld, indem alle Felder auf Leerzeichen gesetzt werden.
void initBoard(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = ' ';
    }
}

// Prüft, ob ein Spieler gewonnen hat (drei gleiche Zeichen in einer Reihe, Spalte oder Diagonale)
char checkWin(void) {
    // Zeilenprüfung
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] != ' ' &&
            board[i * 3] == board[i * 3 + 1] &&
            board[i * 3 + 1] == board[i * 3 + 2])
            return board[i * 3];
    }
    // Spaltenprüfung
    for (int i = 0; i < 3; i++) {
        if (board[i] != ' ' &&
            board[i] == board[i + 3] &&
            board[i + 3] == board[i + 6])
            return board[i];
    }
    // Diagonalenprüfung
    if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8])
        return board[0];
    if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6])
        return board[2];
    return ' ';  // Kein Gewinner
}

// Prüft, ob das Brett voll ist (unentschieden)
int checkDraw(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            return 0;
    }
    return 1;
}

// Überprüft, ob der Zug (Index) gültig ist (innerhalb des Bereichs und unbesetzt)
int isValidMove(int move) {
    if (move < 0 || move >= BOARD_SIZE)
        return 0;
    if (board[move] != ' ')
        return 0;
    return 1;
}

// Setzt an der Stelle 'move' das Zeichen 'mark' im Brett.
void makeMove(int move, char mark) {
    if (move >= 0 && move < BOARD_SIZE)
        board[move] = mark;
}

// Gibt einen zufälligen gültigen Zug für die KI zurück.
int getAIMove(void) {
    int available[BOARD_SIZE];
    int count = 0;
    // Sammle alle freien Felder
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            available[count++] = i;
    }
    if (count == 0)
        return -1;  // Keine Züge mehr möglich
    return available[rand() % count];  // Zufälliger freier Index
}
