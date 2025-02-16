#ifndef SPIELLOGIK_H
#define SPIELLOGIK_H

#define BOARD_SIZE 9

/* Globales Spielfeld (3x3 als eindimensionales Array) */
extern char board[BOARD_SIZE];

/**
 * @brief Initialisiert das Spielfeld, indem alle Felder auf ' ' gesetzt werden.
 */
void initBoard(void);

/**
 * @brief Prüft, ob ein Spieler gewonnen hat.
 * @return 'X' oder 'O' falls ein Spieler gewonnen hat, sonst ' '.
 */
char checkWin(void);

/**
 * @brief Prüft, ob das Spiel unentschieden ist (alle Felder belegt).
 * @return 1 bei Unentschieden, sonst 0.
 */
int checkDraw(void);

/**
 * @brief Prüft, ob der eingegebene Zug (Index) gültig ist.
 * @param move Index des Zugs (0–8).
 * @return 1, wenn der Zug gültig ist, sonst 0.
 */
int isValidMove(int move);

/**
 * @brief Führt einen Zug aus, indem an der Stelle 'move' das Zeichen 'mark' gesetzt wird.
 * @param move Index des Feldes (0–8).
 * @param mark 'X' oder 'O'.
 */
void makeMove(int move, char mark);

/**
 * @brief Gibt einen zufälligen, gültigen Zug für die KI zurück.
 * @return Index des freien Feldes (0–8) oder -1, wenn keine Züge mehr möglich sind.
 */
int getAIMove(void);

#endif  /* SPIELLOGIK_H */
