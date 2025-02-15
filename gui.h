#ifndef GUI_H
#define GUI_H

/**
 * @brief Initialisiert ncurses und setzt die Farbpaare.
 */
void initGUI(void);

/**
 * @brief Beendet ncurses.
 */
void endGUI(void);

/**
 * @brief Zeichnet das Spielfeld in der ncurses-GUI.
 */
void printBoardNCurses(void);

/**
 * @brief Liest den vom Benutzer eingegebenen Zug ein.
 * Der Benutzer soll eine Zahl zwischen 1 und 9 oder "quit" eingeben.
 * @return Index des Zugs (0-8), -2 falls "quit" eingegeben wurde, -1 bei ungültiger Eingabe.
 */
int getUserMoveNCurses(void);

/**
 * @brief Fragt den Benutzer, ob er ein weiteres Spiel spielen möchte.
 * @return 1, wenn 'j' eingegeben wurde, sonst 0.
 */
int askReplayNCurses(void);

/**
 * @brief Zeigt eine Win-Animation (blinkender Text) an.
 * @param message Der anzuzeigende Text.
 */
void winAnimation(const char *message);

#endif  /* GUI_H */
