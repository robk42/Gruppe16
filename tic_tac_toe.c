/*
 * tic_tac_toe.c
 *
 * Projekt: Tic-Tac-Toe mit Netzwerkunterstützung, ncurses-GUI, Spielstatistik,
 *          farbiger Darstellung und Replay-Funktion.
 *
 * Autoren:
 *   - Robin Kelm
 *   - Priyanshu Priyanshu
 *
 * Beschreibung:
 *   Dieses Programm implementiert ein klassisches Tic-Tac-Toe-Spiel, das im
 *   Einzelspieler- sowie im Multiplayer-Modus (über TCP/IP-Sockets) gespielt
 *   werden kann. Die Darstellung erfolgt über die ncurses-Bibliothek, die auch
 *   für farbige Ausgaben genutzt wird. Zusätzlich werden Spielstatistiken in
 *   einer Datei gespeichert, und nach jedem Spiel kann per Replay-Funktion ein
 *   weiteres Spiel gestartet werden, ohne dass das Programm neu gestartet werden muss.
 *
 * Kompilierung (unter Linux/Unix):
 *   gcc tic_tac_toe.c -o tic_tac_toe -lncurses -ltinfo -std=c99 -Wall -Wextra -pedantic
 *
 * Nutzung:
 *   Servermodus:         ./tic_tac_toe -s [port]  (12345)
 *   Clientmodus:         ./tic_tac_toe -c server_ip [port]
 *   Einzelspielermodus:  ./tic_tac_toe -1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <ncurses.h>

/* ==================== Konstante Definitionen ==================== */
#define BOARD_SIZE 9
#define PORT_DEFAULT 12345
#define STATS_FILE "stats.txt"

/* ==================== Globale Variablen ==================== */

/* Spielbrett als eindimensionales Array (3x3) */
char board[BOARD_SIZE];

/* Struktur zur Speicherung von Spielstatistiken */
typedef struct {
    int wins;
    int losses;
    int draws;
} Stats;

/* Globale Variable für Statistiken */
Stats stats = {0, 0, 0};

/* ==================== Funktionsprototypen ==================== */

/* Funktionen für Spielstatistiken */
void loadStats(void);
void saveStats(void);
void displayStatsNCurses(void);

/* Funktionen für die Spiellogik */
void initBoard(void);
void printBoardNCurses(void);
char checkWin(void);
int checkDraw(void);
int isValidMove(int move);
void makeMove(int move, char mark);
int getAIMove(void);
int getUserMoveNCurses(void);
int askReplayNCurses(void);

/* Funktionen für Netzwerkkommunikation */
int startServer(int port);
int connectToServer(const char *ip, int port);
int sendMove(int sock, int move);
int receiveMove(int sock, int *move);

/* Spielabläufe */
void gameLoopNetwork(int sock, int isServer);
void gameLoopSingle(void);

/* ==================== Funktionen für Spielstatistiken ==================== */

/**
 * loadStats - Lädt die Spielstatistiken aus STATS_FILE, sofern vorhanden.
 */
void loadStats(void) {
    FILE *file = fopen(STATS_FILE, "r");
    if (file) {
        fscanf(file, "%d %d %d", &stats.wins, &stats.losses, &stats.draws);
        fclose(file);
    }
}

/**
 * saveStats - Speichert die aktuellen Statistiken in STATS_FILE.
 */
void saveStats(void) {
    FILE *file = fopen(STATS_FILE, "w");
    if (file) {
        fprintf(file, "%d %d %d", stats.wins, stats.losses, stats.draws);
        fclose(file);
    }
}

/**
 * displayStatsNCurses - Zeigt die Spielstatistiken in der ncurses-Oberfläche an.
 */
void displayStatsNCurses(void) {
    int row = 14;
    mvprintw(row, 2, "Spielstatistiken:");
    mvprintw(row + 1, 2, "Siege:        %d", stats.wins);
    mvprintw(row + 2, 2, "Niederlagen:  %d", stats.losses);
    mvprintw(row + 3, 2, "Unentschieden:%d", stats.draws);
    mvprintw(row + 5, 2, "Beliebige Taste zum Beenden...");
    refresh();
    getch();
}

/* ==================== Funktionen für die Spiellogik ==================== */

/**
 * initBoard - Initialisiert das Spielbrett, indem alle Felder auf ' ' gesetzt werden.
 */
void initBoard(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = ' ';
    }
}

/**
 * printBoardNCurses - Zeichnet das aktuelle Spielbrett mithilfe von ncurses und Farbpaare.
 */
void printBoardNCurses(void) {
    clear();
    int start_y = 1, start_x = 2;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = i * 3 + j;
            char cell = board[index];
            int color_pair = 1;  // Standardfarbe
            if (cell == 'X')
                color_pair = 2;  // Rot für X
            else if (cell == 'O')
                color_pair = 3;  // Blau für O

            attron(COLOR_PAIR(color_pair));
            mvprintw(start_y + i * 2, start_x + j * 4, " %c ", cell);
            attroff(COLOR_PAIR(color_pair));

            if (j < 2) {
                attron(COLOR_PAIR(4));  // Farbe für die Linien
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
 * checkWin - Prüft, ob ein Spieler gewonnen hat.
 * Returns: 'X' oder 'O' bei Gewinn, sonst ' '.
 */
char checkWin(void) {
    for (int i = 0; i < 3; i++) {
        if (board[3 * i] != ' ' &&
            board[3 * i] == board[3 * i + 1] &&
            board[3 * i + 1] == board[3 * i + 2])
            return board[3 * i];
    }
    for (int i = 0; i < 3; i++) {
        if (board[i] != ' ' &&
            board[i] == board[i + 3] &&
            board[i + 3] == board[i + 6])
            return board[i];
    }
    if (board[0] != ' ' &&
        board[0] == board[4] &&
        board[4] == board[8])
        return board[0];
    if (board[2] != ' ' &&
        board[2] == board[4] &&
        board[4] == board[6])
        return board[2];
    return ' ';
}

/**
 * checkDraw - Prüft, ob das Spiel unentschieden endet.
 * Returns: 1 bei Unentschieden, sonst 0.
 */
int checkDraw(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            return 0;
    }
    return 1;
}

/**
 * isValidMove - Prüft, ob der übergebene Zug (Index) gültig ist.
 */
int isValidMove(int move) {
    if (move < 0 || move >= BOARD_SIZE)
        return 0;
    if (board[move] != ' ')
        return 0;
    return 1;
}

/**
 * makeMove - Führt den Zug aus, indem das Zeichen (mark) an der Position (move) gesetzt wird.
 */
void makeMove(int move, char mark) {
    if (move >= 0 && move < BOARD_SIZE)
        board[move] = mark;
}

/**
 * getAIMove - Bestimmt einen zufälligen, gültigen Zug für die KI.
 * Returns: Index des gewählten Feldes oder -1, wenn kein Zug möglich ist.
 */
int getAIMove(void) {
    int available[BOARD_SIZE];
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == ' ')
            available[count++] = i;
    }
    if (count == 0)
        return -1;
    return available[rand() % count];
}

/**
 * getUserMoveNCurses - Liest die Eingabe des Spielers (Zug als Zahl) über ncurses ein.
 */
int getUserMoveNCurses(void) {
    char input[10];
    mvprintw(8, 2, "Dein Zug (0-8): ");
    clrtoeol();
    echo();
    getnstr(input, 9);
    noecho();
    return atoi(input);
}

/**
 * askReplayNCurses - Fragt den Spieler, ob er ein weiteres Spiel starten möchte.
 * Returns: 1 bei "j" (ja), sonst 0.
 */
int askReplayNCurses(void) {
    mvprintw(10, 2, "Noch ein Spiel? (j/n): ");
    clrtoeol();
    refresh();
    int ch = getch();
    if (ch == 'j' || ch == 'J')
        return 1;
    return 0;
}

/* ==================== Funktionen für Netzwerkkommunikation ==================== */

/**
 * startServer - Startet einen Server, der auf dem angegebenen Port auf eine Verbindung wartet.
 * Returns: Den Socket, der die Verbindung zum Client repräsentiert.
 */
int startServer(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        endwin();
        perror("Socket-Fehler");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        endwin();
        perror("Fehler bei setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        endwin();
        perror("Bind-Fehler");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        endwin();
        perror("Listen-Fehler");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Warte auf Verbindung am Port %d...", port);
    refresh();
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        endwin();
        perror("Accept-Fehler");
        exit(EXIT_FAILURE);
    }
    close(server_fd);
    mvprintw(0, 2, "Verbindung hergestellt!                   ");
    refresh();
    napms(1000);
    return new_socket;
}

/**
 * connectToServer - Baut als Client eine Verbindung zu einem Server mit der gegebenen IP und Port auf.
 * Returns: Den Socket, der die Verbindung repräsentiert.
 */
int connectToServer(const char *ip, int port) {
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        endwin();
        perror("Socket-Erstellung fehlgeschlagen");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        endwin();
        fprintf(stderr, "Ungültige Adresse/Adresse nicht unterstützt\n");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        endwin();
        fprintf(stderr, "Verbindungsfehler\n");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Mit Server verbunden!                   ");
    refresh();
    napms(1000);
    return sock;
}

/**
 * sendMove - Sendet den Zug (als Integer) über den Socket.
 */
int sendMove(int sock, int move) {
    int nmove = htonl(move);
    int bytes_sent = send(sock, &nmove, sizeof(nmove), 0);
    if (bytes_sent != sizeof(nmove))
        return -1;
    return 0;
}

/**
 * receiveMove - Empfängt einen Zug (als Integer) über den Socket.
 */
int receiveMove(int sock, int *move) {
    int nmove;
    int bytes_received = recv(sock, &nmove, sizeof(nmove), 0);
    if (bytes_received <= 0)
        return -1;
    *move = ntohl(nmove);
    return 0;
}

/* ==================== Spielabläufe ==================== */

/**
 * gameLoopNetwork - Führt den Spielablauf im Netzwerkmodus aus und
 *                   verhandelt nach Spielende, ob ein Replay erfolgt.
 */
void gameLoopNetwork(int sock, int isServer) {
    int replay = 1;
    while (replay) {
        initBoard();
        int gameOver = 0;
        int move;
        char winner;
        int localTurn = isServer ? 1 : 0;
        
        while (!gameOver) {
            printBoardNCurses();
            if (localTurn) {
                move = getUserMoveNCurses();
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Ungültiger Zug. Drücke eine Taste und versuche es erneut.");
                    refresh();
                    getch();
                    continue;
                }
                makeMove(move, isServer ? 'X' : 'O');
                if (sendMove(sock, move) < 0) {
                    mvprintw(10, 2, "Fehler beim Senden des Zuges.");
                    refresh();
                    napms(2000);
                    return;
                }
            } else {
                mvprintw(10, 2, "Warte auf Zug des Gegenspielers...");
                clrtoeol();
                refresh();
                if (receiveMove(sock, &move) < 0) {
                    mvprintw(10, 2, "Fehler beim Empfangen des Zuges.");
                    refresh();
                    napms(2000);
                    return;
                }
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Gegenspieler hat einen ungültigen Zug gemacht.");
                    refresh();
                    napms(2000);
                    continue;
                }
                makeMove(move, isServer ? 'O' : 'X');
            }
            
            winner = checkWin();
            if (winner != ' ') {
                printBoardNCurses();
                if ((isServer && winner == 'X') || (!isServer && winner == 'O')) {
                    mvprintw(10, 2, "Du hast gewonnen!");
                    stats.wins++;
                } else {
                    mvprintw(10, 2, "Du hast verloren!");
                    stats.losses++;
                }
                gameOver = 1;
            } else if (checkDraw()) {
                printBoardNCurses();
                mvprintw(10, 2, "Unentschieden!");
                stats.draws++;
                gameOver = 1;
            }
            refresh();
            napms(1000);
            localTurn = !localTurn;
        }
        /* Replay-Verhandlung: Beide Spieler geben ihre Entscheidung ab */
        mvprintw(12, 2, "Spiel beendet.");
        refresh();
        int localReplay = askReplayNCurses();
        if (sendMove(sock, localReplay) < 0) {
            mvprintw(13, 2, "Fehler beim Senden der Replay-Entscheidung.");
            refresh();
            napms(2000);
            break;
        }
        int remoteReplay;
        if (receiveMove(sock, &remoteReplay) < 0) {
            mvprintw(13, 2, "Fehler beim Empfangen der Replay-Entscheidung.");
            refresh();
            napms(2000);
            break;
        }
        if (localReplay && remoteReplay) {
            replay = 1;
        } else {
            replay = 0;
            mvprintw(14, 2, "Beide Spieler wollen nicht erneut spielen. Beende Spiel.");
            refresh();
            napms(2000);
        }
    }
    close(sock);
    displayStatsNCurses();
}

/**
 * gameLoopSingle - Führt den Spielablauf im Einzelspielermodus aus,
 *                   inklusive Replay-Funktion.
 */
void gameLoopSingle(void) {
    int replay = 1;
    while (replay) {
        initBoard();
        int gameOver = 0;
        int move;
        char winner;
        int playerTurn = 1;  // Spieler (X) beginnt
        while (!gameOver) {
            printBoardNCurses();
            if (playerTurn) {
                move = getUserMoveNCurses();
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Ungültiger Zug. Drücke eine Taste und versuche es erneut.");
                    refresh();
                    getch();
                    continue;
                }
                makeMove(move, 'X');
            } else {
                mvprintw(10, 2, "KI ist am Zug...");
                clrtoeol();
                refresh();
                napms(1000);
                move = getAIMove();
                if (move < 0) {
                    mvprintw(10, 2, "Fehler: Kein gültiger Zug für die KI.");
                    refresh();
                    napms(2000);
                    break;
                }
                makeMove(move, 'O');
            }
            winner = checkWin();
            if (winner != ' ') {
                printBoardNCurses();
                if (winner == 'X') {
                    mvprintw(10, 2, "Du hast gewonnen!");
                    stats.wins++;
                } else {
                    mvprintw(10, 2, "Die KI hat gewonnen!");
                    stats.losses++;
                }
                gameOver = 1;
            } else if (checkDraw()) {
                printBoardNCurses();
                mvprintw(10, 2, "Unentschieden!");
                stats.draws++;
                gameOver = 1;
            }
            refresh();
            napms(1000);
            playerTurn = !playerTurn;
        }
        int replayChoice = askReplayNCurses();
        if (!replayChoice)
            replay = 0;
    }
    displayStatsNCurses();
}

/* ==================== Main-Funktion ==================== */

/**
 * main - Einstiegspunkt des Programms.
 */
int main(int argc, char *argv[]) {
    int sock;
    srand((unsigned int) time(NULL));
    loadStats();
    
    /* ncurses initialisieren */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    /* Prüfe, ob Farben unterstützt werden, und initialisiere Farbpaare */
    if (has_colors() == FALSE) {
        endwin();
        printf("Terminal unterstützt keine Farben.\n");
        exit(EXIT_FAILURE);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Standard
    init_pair(2, COLOR_RED, COLOR_BLACK);    // Für X
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   // Für O
    init_pair(4, COLOR_GREEN, COLOR_BLACK);  // Für Spielfeldlinien
    
    if (argc < 2) {
        mvprintw(0, 2, "Nutzung:");
        mvprintw(1, 2, "  %s -s [port]           // Servermodus (optional: Port, Standard: %d)", argv[0], PORT_DEFAULT);
        mvprintw(2, 2, "  %s -c server_ip [port]  // Clientmodus (Server-IP und optional Port, Standard: %d)", argv[0], PORT_DEFAULT);
        mvprintw(3, 2, "  %s -1                  // Einzelspielermodus (gegen einfache KI)", argv[0]);
        refresh();
        getch();
        endwin();
        exit(EXIT_FAILURE);
    }
    
    if (strcmp(argv[1], "-s") == 0) {
        int port = PORT_DEFAULT;
        if (argc >= 3)
            port = atoi(argv[2]);
        sock = startServer(port);
        gameLoopNetwork(sock, 1);
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 3) {
            mvprintw(0, 2, "Bitte Server-IP angeben.");
            refresh();
            getch();
            endwin();
            exit(EXIT_FAILURE);
        }
        {
            const char *server_ip = argv[2];
            int port = PORT_DEFAULT;
            if (argc >= 4)
                port = atoi(argv[3]);
            sock = connectToServer(server_ip, port);
        }
        gameLoopNetwork(sock, 0);
    } else if (strcmp(argv[1], "-1") == 0) {
        gameLoopSingle();
    } else {
        mvprintw(0, 2, "Unbekannter Modus.");
        refresh();
        getch();
        endwin();
        exit(EXIT_FAILURE);
    }
    
    saveStats();
    endwin();
    return EXIT_SUCCESS;
}
