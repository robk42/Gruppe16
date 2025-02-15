#include "gui.h"
#include "spiellogik.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // Für strcasecmp

void initGUI(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    if (!has_colors()) {
        endwin();
        printf("Terminal does not support colors.\n");
        exit(EXIT_FAILURE);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Standardfarbe (für leere Felder)
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Für Spieler (X) / Win-Animation
    init_pair(3, COLOR_BLUE, COLOR_BLACK);     // Für KI (O)
    init_pair(4, COLOR_GREEN, COLOR_BLACK);    // Für Spielfeldlinien
}

void endGUI(void) {
    endwin();
}

void printBoardNCurses(void) {
    clear();
    int start_y = 1, start_x = 2;
    char cellContent[4];  // Platz für 3 Zeichen + Nullterminator

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = i * 3 + j;
            if (board[index] == ' ') {
                /* Leeres Feld: keine Beschriftung, nur Leerzeichen */
                snprintf(cellContent, sizeof(cellContent), "   ");
            } else {
                /* Besetztes Feld: zentrierte Anzeige von X oder O */
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

int getUserMoveNCurses(void) {
    char input[10];
    mvprintw(8, 2, "Your move (1-9 or 'quit' to exit): ");
    clrtoeol();
    echo();
    getnstr(input, 9);
    noecho();
    if (strcasecmp(input, "quit") == 0) {
        return -2;  // spezieller Code für "quit"
    }
    int num = atoi(input);
    if (num < 1 || num > 9) {
        return -1;  // ungültige Eingabe
    }
    return num - 1;  // Umwandlung: Eingabe 1 entspricht Index 0, 9 entspricht Index 8
}

int askReplayNCurses(void) {
    mvprintw(10, 2, "Play again? (j/n): ");
    clrtoeol();
    refresh();
    int ch = getch();
    if (ch == 'j' || ch == 'J')
        return 1;
    return 0;
}

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
