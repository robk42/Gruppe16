#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>    // Für close()
#include "spiellogik.h"
#include "netzwerk.h"  // Netzwerkfunktionen (wird im Singleplayer nicht genutzt)
#include "gui.h"
#include "stats.h"
#include <ncurses.h>

#define PORT_DEFAULT 12345

// Prototyp für displayErrorAndExit() (wird später definiert).
void displayErrorAndExit(const char *errorMessage);

/**
 * Zeigt die aktuellen Statistiken an und pausiert 2 Sekunden.
 */
void displayAndWaitStats(void) {
    mvprintw(12, 2, "Stats: Wins: %d, Losses: %d, Draws: %d", 
             stats.wins, stats.losses, stats.draws);
    refresh();
    napms(2000);
}

/**
 * Hilfsfunktion zur Fehlerbehandlung: Zeigt eine Fehlermeldung, beendet ncurses und terminiert das Programm.
 */
void displayErrorAndExit(const char *errorMessage) {
    endGUI();
    fprintf(stderr, "%s\n", errorMessage);
    exit(EXIT_FAILURE);
}

/**
 * Hilfsfunktion, die den Spielzug-Turn umschaltet und eine Pause einfügt.
 */
void toggleTurn(int *turn) {
    *turn = !(*turn);
    napms(1000);
}

/**
 * Führt ein einzelnes Spiel im Singleplayer-Modus aus.
 *
 * @return Das Ergebnis des Spiels: 'X' (Spieler gewinnt), 'O' (KI gewinnt) oder 'D' (Draw).
 */
char playSingleGame(void) {
    int move;
    char winner;
    int playerTurn = 1;  // Spieler (X) beginnt
    char result = 'D';

    initBoard();
    while (1) {
        printBoardNCurses();
        if (playerTurn) {
            move = getUserMoveNCurses();
            if (move == -2) {  // "quit" eingegeben
                result = 'D';
                break;
            }
            if (!isValidMove(move)) {
                displayMessage("Invalid move. Press any key to try again.");
                continue;
            }
            makeMove(move, 'X');
        } else {
            napms(1000);
            move = getAIMove();
            makeMove(move, 'O');
        }
        winner = checkWin();
        if (winner != ' ') {
            printBoardNCurses();
            if (winner == 'X') {
                mvprintw(10, 2, "You win!");
                refresh();
                winAnimation("You win!");
                result = 'X';
            } else {
                mvprintw(10, 2, "AI wins!");
                refresh();
                winAnimation("AI wins!");
                result = 'O';
            }
            break;
        } else if (checkDraw()) {
            printBoardNCurses();
            mvprintw(10, 2, "It's a draw!");
            refresh();
            result = 'D';
            break;
        }
        toggleTurn(&playerTurn);
    }
    return result;
}

/**
 * Singleplayer-Modus: Führt wiederholt einzelne Spiele aus.
 */
void singlePlayerMode(void) {
    int replay = 1;
    char result;

    while (replay) {
        result = playSingleGame();
        updateStats(result);
        displayAndWaitStats();
        replay = askReplayNCurses();
    }
}

/**
 * Netzwerkmodus: Ermöglicht Spielen über TCP/IP.
 *
 * Falls der Client die Verbindung verliert, erhält er 30 Sekunden Zeit (6 Versuche à 5 Sekunden),
 * um sich neu zu verbinden. Scheitert dies, verliert der Client und der Host gewinnt.
 *
 * @param isServer 1, wenn der lokale Spieler als Server agiert; 0, wenn als Client.
 * @param server_ip IP-Adresse des Servers (nur im Client-Modus benötigt).
 * @param port Der zu verwendende Port.
 */
void networkMode(int isServer, const char *server_ip, int port) {
    int sock;
    if (isServer)
        sock = startServer(port);
    else
        sock = connectToServer(server_ip, port);
    
    int replay = 1;
    int move, gameOver, playerTurn = isServer ? 1 : 0;
    char winner, result;
    
    while (replay) {
        initBoard();
        gameOver = 0;
        while (!gameOver) {
            printBoardNCurses();
            if (playerTurn) {
                move = getUserMoveNCurses();
                if (move == -2) {
                    gameOver = 1;
                    replay = 0;
                    break;
                }
                if (!isValidMove(move)) {
                    displayMessage("Invalid move. Press any key to try again.");
                    continue;
                }
                makeMove(move, isServer ? 'X' : 'O');
                if (sendMove(sock, move) < 0) {
                    if (!isServer) {
                        int newSock = attemptReconnect(server_ip, port);
                        if (newSock < 0) {
                            mvprintw(12, 2, "Failed to reconnect. You lose!");
                            refresh();
                            napms(2000);
                            gameOver = 1;
                            result = 'X';
                            break;
                        } else {
                            sock = newSock;
                        }
                    } else {
                        displayErrorAndExit("Error sending move.");
                    }
                }
            } else {
                mvprintw(10, 2, "Waiting for opponent's move...");
                clrtoeol();
                refresh();
                if (receiveMove(sock, &move) < 0) {
                    if (!isServer) {
                        int newSock = attemptReconnect(server_ip, port);
                        if (newSock < 0) {
                            mvprintw(12, 2, "Failed to reconnect. You lose!");
                            refresh();
                            napms(2000);
                            gameOver = 1;
                            result = 'X';
                            break;
                        } else {
                            sock = newSock;
                        }
                    } else {
                        displayErrorAndExit("Error receiving opponent's move.");
                    }
                }
                if (!isValidMove(move)) {
                    displayMessage("Opponent made an invalid move.");
                    continue;
                }
                makeMove(move, isServer ? 'O' : 'X');
            }
            winner = checkWin();
            if (winner != ' ') {
                printBoardNCurses();
                if ((isServer && winner == 'X') || (!isServer && winner == 'O')) {
                    mvprintw(10, 2, "You win!");
                    refresh();
                    winAnimation("You win!");
                    result = isServer ? 'X' : 'O';
                } else {
                    mvprintw(10, 2, "You lose!");
                    refresh();
                    winAnimation("You lose!");
                    result = isServer ? 'O' : 'X';
                }
                gameOver = 1;
            } else if (checkDraw()) {
                printBoardNCurses();
                mvprintw(10, 2, "It's a draw!");
                refresh();
                gameOver = 1;
                result = 'D';
            }
            napms(1000);
            toggleTurn(&playerTurn);
        }
        updateStats(result);
        displayAndWaitStats();
        if (replay)
            replay = askReplayNCurses();
    }
    close(sock);
}

/**
 * Hauptfunktion: Wählt den Spielmodus anhand der Kommandozeilenargumente.
 *
 * Verfügbare Modi:
 * - Singleplayer:  ./tic_tac_toe -1
 * - Netzwerkmodus – Server:  ./tic_tac_toe -s [port]
 * - Netzwerkmodus – Client:  ./tic_tac_toe -c <ip> [port]
 * - Hilfe:  ./tic_tac_toe --help oder -h
 */
int main(int argc, char *argv[]) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage:\n");
        printf("  %s -1             (Singleplayer mode: Player vs. AI)\n", argv[0]);
        printf("  %s -s [port]      (Server mode, default port %d)\n", argv[0], PORT_DEFAULT);
        printf("  %s -c <ip> [port] (Client mode, default port %d)\n", argv[0], PORT_DEFAULT);
        printf("  %s --help or -h   (Display this help message)\n", argv[0]);
        exit(EXIT_SUCCESS);
    }
    
    loadStats();
    srand((unsigned int)time(NULL));
    
    if (strcmp(argv[1], "-1") == 0) {
        initGUI();
        singlePlayerMode();
        saveStats();
        endGUI();
    } else if (strcmp(argv[1], "-s") == 0) {
        int port = PORT_DEFAULT;
        if (argc >= 3)
            port = atoi(argv[2]);
        initGUI();
        networkMode(1, NULL, port);
        saveStats();
        endGUI();
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Client mode requires server IP address.\n");
            exit(EXIT_FAILURE);
        }
        int port = PORT_DEFAULT;
        if (argc >= 4)
            port = atoi(argv[3]);
        initGUI();
        networkMode(0, argv[2], port);
        saveStats();
        endGUI();
    } else {
        fprintf(stderr, "Invalid mode.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
