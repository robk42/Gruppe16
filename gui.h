#ifndef GUI_H
#define GUI_H

/**
 * @brief Initialisiert die ncurses-GUI und legt die Farbpaare fest.
 */
void initGUI(void);

/**
 * @brief Beendet den ncurses-Modus.
 */
void endGUI(void);

/**
 * @brief Zeichnet das Spielfeld in der ncurses-Oberfläche.
 *
 * Leere Felder werden numerisch von 1 bis 9 angezeigt,
 * besetzte Felder zeigen den Spielerzug (X oder O) zentriert.
 */
void printBoardNCurses(void);

/**
 * @brief Liest die Benutzereingabe für einen Zug ein.
 *
 * Erwartet eine Zahl zwischen 1 und 9 oder "quit" zum Beenden.
 * @return Den Feldindex (0–8), -2 falls "quit" eingegeben wird, oder -1 bei ungültiger Eingabe.
 */
int getUserMoveNCurses(void);

/**
 * @brief Fragt den Benutzer, ob er ein weiteres Spiel spielen möchte.
 * @return 1, wenn 'j' eingegeben wird, sonst 0.
 */
int askReplayNCurses(void);

/**
 * @brief Zeigt eine Win-Animation (blinkender Text) in der ncurses-Oberfläche an.
 * Abhängig vom Text wird eine entsprechende Farbe genutzt:
 * - "You win!" → orange,
 * - "It's a draw!" → gelb,
 * - "You lose!" → rot.
 * @param message Der anzuzeigende Text.
 */
void winAnimation(const char *message);

/**
 * @brief Zeigt eine Nachricht in der GUI an und wartet auf eine Tastatureingabe.
 * @param msg Die Nachricht, die angezeigt werden soll.
 */
void displayMessage(const char *msg);

#endif  /* GUI_H */
