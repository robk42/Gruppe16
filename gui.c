#include "gui.h"
#include "spiellogik.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // Für strcasecmp
#include <ctype.h>    // Für isdigit()

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

    // Standardfarbpaare:
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Für leere Felder (numerische Labels)
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Für "You lose!" (rot)
    init_pair(3, COLOR_BLUE, COLOR_BLACK);     // Für besetzte Felder (z. B. O)
    init_pair(4, COLOR_GREEN, COLOR_BLACK);    // Für Spielfeldlinien

    // Farb-Paar 5: Orange für Banner und "You win!"
    if (can_change_color()) {
        // Definiere Orange an Index 10 (Rot=1000, Grün=500, Blau=0)
        init_color(10, 1000, 500, 0);
        init_pair(5, 10, COLOR_BLACK);
    } else {
        // Fallback: Verwende Gelb, falls keine benutzerdefinierte Farbe möglich ist.
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    }
    // Farb-Paar 6 für "It's a draw!": Gelb
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
}

void endGUI(void) {
    endwin();
}

void printBoardNCurses(void) {
    clear();
    int start_y = 1, start_x = 2;
    char cellContent[4];

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int index = i * 3 + j;
            if (board[index] == ' ')
                snprintf(cellContent, sizeof(cellContent), " %d ", index + 1);
            else
                snprintf(cellContent, sizeof(cellContent), " %c ", board[index]);
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
            for (int j = 0; j < 3; j++){
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
    char input[10] = {0};
    // Angepasster Prompt, der deutlich macht, dass nur leere Zellen wählbar sind.
    mvprintw(8, 2, "Your move (choose an empty cell 1-9 or 'quit' to exit): ");
    clrtoeol();
    echo();
    if (getnstr(input, 9) == ERR) {
        noecho();
        return -1;
    }
    noecho();
    if (strcasecmp(input, "quit") == 0)
        return -2;
    for (size_t i = 0; i < strlen(input); i++){
        if (!isdigit(input[i]))
            return -1;
    }
    int num = atoi(input);
    if (num < 1 || num > 9)
        return -1;
    return num - 1;
}

/**
 * @brief Fragt den Benutzer, ob er ein weiteres Spiel spielen möchte.
 * Diese Funktion fordert so lange eine Eingabe an, bis 'j' (yes) oder 'n' (no) eingegeben wird.
 *
 * @return 1, wenn 'j' eingegeben wird; 0, wenn 'n' eingegeben wird.
 */
int askReplayNCurses(void) {
    int ch;
    while (1) {
        mvprintw(10, 2, "Play again? (j/n): ");
        clrtoeol();
        refresh();
        ch = getch();
        if (ch == 'j' || ch == 'J') {
            return 1;
        } else if (ch == 'n' || ch == 'N') {
            return 0;
        } else {
            mvprintw(12, 2, "Invalid input. Please press 'j' for yes or 'n' for no.");
            refresh();
            napms(1500);
            // Lösche die Fehlermeldung, bevor erneut gefragt wird.
            mvprintw(12, 2, "                                               ");
        }
    }
}

void winAnimation(const char *message) {
    int row, col;
    getmaxyx(stdscr, row, col);
    
    // Bestimme die Farbe anhand des Nachrichten-Strings:
    // "You win!" → grün (Farb-Paar 4)
    // "It's a draw!" → gelb (Farb-Paar 6)
    // "You lose!" → rot (Farb-Paar 2)
    int colorPair;
    if (strcasecmp(message, "You win!") == 0)
        colorPair = 4;
    else if (strcasecmp(message, "It's a draw!") == 0)
        colorPair = 6;
    else if (strcasecmp(message, "You lose!") == 0)
        colorPair = 2;
    else
        colorPair = 1;
    
    for (int i = 0; i < 6; i++){
        clear();
        if (i % 2 == 0){
            attron(A_BOLD | COLOR_PAIR(colorPair));
            mvprintw(row/2, (col - (int)strlen(message)) / 2, "%s", message);
            attroff(A_BOLD | COLOR_PAIR(colorPair));
        }
        refresh();
        napms(300);
    }
    clear();
    refresh();
}

void displayMessage(const char *msg) {
    mvprintw(10, 2, "%s", msg);
    refresh();
    getch();
}
