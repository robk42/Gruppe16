#ifndef SPIELLOGIK_H
#define SPIELLOGIK_H

#define BOARD_SIZE 9

/* Globales Spielfeld (3x3 als eindimensionales Array) */
extern char board[BOARD_SIZE];

/* Initialisiert das Spielfeld (setzt alle Felder auf ' ') */
void initBoard(void);

/* Prüft, ob ein Spieler gewonnen hat. Gibt 'X' oder 'O' zurück, wenn gewonnen, sonst ' ' */
char checkWin(void);

/* Prüft, ob alle Felder belegt sind (Unentschieden) */
int checkDraw(void);

/* Prüft, ob der eingegebene Zug (Index) gültig ist */
int isValidMove(int move);

/* Setzt an der Stelle 'move' das Zeichen 'mark' */
void makeMove(int move, char mark);

/* Gibt einen zufälligen, gültigen Zug für die KI zurück */
int getAIMove(void);

#endif  /* SPIELLOGIK_H */
