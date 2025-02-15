#ifndef GUI_H
#define GUI_H

/* Initialisiert ncurses und setzt die Farbpaare */
void initGUI(void);

/* Beendet ncurses */
void endGUI(void);

/* Zeichnet das Spielfeld mithilfe von ncurses */
void printBoardNCurses(void);

/* Liest den vom Benutzer eingegebenen Zug ein.
   Erwartet eine Zahl von 1 bis 9 oder "quit" zum Beenden.
   Gibt den entsprechenden Index (0–8) zurück oder -2 für "quit".
   Bei ungültiger Eingabe wird -1 zurückgegeben. */
int getUserMoveNCurses(void);

/* Fragt den Benutzer, ob er ein Replay möchte (j/n) */
int askReplayNCurses(void);

/* Zeigt eine Win-Animation (blinkender Text) an */
void winAnimation(const char *message);

#endif  /* GUI_H */
