#ifndef SPIELLOGIK_H
#define SPIELLOGIK_H

#define BOARD_SIZE 9

/* Globales Spielfeld, dargestellt als eindimensionales Array mit 9 Elementen (3x3) */
extern char board[BOARD_SIZE];

/**
 * @brief Initialisiert das Spielfeld.
 * Setzt alle Felder des Bretts auf ' ' (Leerzeichen).
 */
void initBoard(void);

/**
 * @brief Prüft, ob ein Spieler gewonnen hat.
 * @return Gibt 'X' oder 'O' zurück, falls ein Spieler gewonnen hat, sonst ' '.
 */
char checkWin(void);

/**
 * @brief Prüft, ob das Spiel unentschieden ist.
 * @return 1 wenn alle Felder belegt sind (unentschieden), sonst 0.
 */
int checkDraw(void);

/**
 * @brief Prüft, ob der eingegebene Zug (Index) gültig ist.
 * @param move Der zu prüfende Index (0-8).
 * @return 1, wenn der Zug gültig ist, sonst 0.
 */
int isValidMove(int move);

/**
 * @brief Führt einen Zug aus.
 * Setzt das Zeichen (mark) an der Stelle (move) im Spielfeld.
 * @param move Index des Felds (0-8).
 * @param mark Das Zeichen ('X' oder 'O').
 */
void makeMove(int move, char mark);

/**
 * @brief Bestimmt einen zufälligen, gültigen Zug für die KI.
 * @return Den Index (0-8) eines freien Felds oder -1, falls keine Züge möglich sind.
 */
int getAIMove(void);

#endif  /* SPIELLOGIK_H */
