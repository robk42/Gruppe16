#include "gui.h"
#include "spiellogik.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // Für isdigit()
#include <strings.h>    // Für strcasecmp

// Initialisiert ncurses und legt die verwendeten Farbpaare fest.
void initGUI(void) {
    initscr();               // Startet den ncurses-Modus
    cbreak();                // Sofortige Eingabe ohne Enter
    noecho();                // Verhindert die Anzeige der Tastatureingaben
    keypad(stdscr, TRUE);    // Aktiviert erweiterte Tasteneingaben (z. B. Pfeiltasten)
    if (!has_colors()) {
        endwin();
        printf("Terminal does not support colors.\n");
        exit(EXIT_FAILURE);
    }
    start_color();           // Aktiviert die Farbfunktion
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Für leere Felder (numerische Labels)
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Für Spieler (X) und Win-Animation
    init_pair(3, COLOR_BLUE, COLOR_BLACK);     // Für KI (O)
    init_pair(4, COLOR_GREEN, COLOR_BLACK);    // Für Spielfeldlinien
}

// Beendet den ncurses-Modus.
void endGUI(void) {
    endwin();
}

// Zeichnet das Spielfeld: Leere Felder werden mit numerischen Labels (1-9) angezeigt; besetzte Felder zeigen X oder O.
void printBoardNCurses(void) {
    clear();
    int start_y = 1, start_x = 2;
    char cellContent[4];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = i * 3 + j;
            if (board[index] == ' ') {
                // Zeige numerisches Label: index+1
                snprintf(cellContent, sizeof(cellContent), " %d ", index + 1);
            } else {
                // Zeige besetztes Feld (X oder O)
                snprintf(cellContent, sizeof(cellContent), " %c ", board[index]);
            }
            int color_pair = 1;
            if (board[index] == 'X')
                color_pair = 2;
            else if (board[index] == 'O')
                color_pair = 3;
            attron(COLOR_PAIR(color_pair));
            mvprintw(start_y + i * 2, start_x + j * 4, "%s", cellContent);
            attroff(COLOR_PAIR(color_pair));
            if (j < 2) {
                attron(COLOR_PAIR(4));
                mvprintw(start_y + i * 2, start_x + j * 4 + 3, "|");
                attroff(COLOR_PAIR(4));
            }
        }
        if (i < 2) {
            for (int j = 0; j < 3; j++) {
                attron(COLOR_PAIR(4));
                mvprintw(start_y + i * 2 + 1, start_x + j * 4, "---");
                attroff(COLOR_PAIR(4));
                if (j < 2) {
                    attron(COLOR_PAIR(4));
                    mvprintw(start_y + i * 2 + 1, start_x + j * 4 + 3, "+");
                    attroff(COLOR_PAIR(4));
                }
            }
        }
    }
    refresh();
}

/**
 * Liest die Benutzereingabe für einen Zug ein.
 * Erwartet eine Zahl von 1 bis 9 oder "quit" zum Beenden.
 * Zusätzlich wird geprüft, ob getnstr() erfolgreich war und ob die Eingabe nur Ziffern enthält.
 *
 * @return Den Feldindex (0-8), -2 falls "quit" eingegeben wird, oder -1 bei ungültiger Eingabe.
 */
int getUserMoveNCurses(void) {
    char input[10] = {0};
    mvprintw(8, 2, "Your move (1-9 or 'quit' to exit): ");
    clrtoeol();
    echo();
    if (getnstr(input, 9) == ERR) {
        noecho();
        return -1;
    }
    noecho();
    if (strcasecmp(input, "quit") == 0) {
        return -2;
    }
    // Prüfe, ob alle Zeichen Ziffern sind
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return -1;
        }
    }
    int num = atoi(input);
    if (num < 1 || num > 9) {
        return -1;
    }
    return num - 1;
}

// Fragt den Benutzer, ob er ein weiteres Spiel spielen möchte.
int askReplayNCurses(void) {
    mvprintw(10, 2, "Play again? (j/n): ");
    clrtoeol();
    refresh();
    int ch = getch();
    if (ch == 'j' || ch == 'J')
        return 1;
    return 0;
}

// Zeigt eine Win-Animation (blinkender Text) an.
void winAnimation(const char *message) {
    int row, col;
    getmaxyx(stdscr, row, col);
    for (int i = 0; i < 6; i++) {
        clear();
        if (i % 2 == 0) {
            attron(A_BOLD | COLOR_PAIR(2));
            mvprintw(row / 2, (col - (int)strlen(message)) / 2, "%s", message);
            attroff(A_BOLD | COLOR_PAIR(2));
        }
        refresh();
        napms(300);
    }
    clear();
    refresh();
}

/**
 * Zeigt eine Nachricht in der GUI an und wartet auf eine Tastatureingabe.
 *
 * @param msg Die Nachricht, die angezeigt werden soll.
 */
void displayMessage(const char *msg) {
    mvprintw(10, 2, "%s", msg);
    refresh();
    getch();
}
