#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // Für close()
#include "spiellogik.h"
#include "netzwerk.h"   // Für den Netzwerkmodus
#include "gui.h"
#include "stats.h"
#include <ncurses.h>

#define PORT_DEFAULT 12345

/* Singleplayer-Modus (Spieler vs. KI) */
void singlePlayerMode(void) {
    int replay = 1;
    int move;
    char winner;
    int playerTurn;
    char result; // 'X' = Spieler gewinnt, 'O' = KI gewinnt, 'D' = Draw

    while (replay) {
        initBoard();
        int gameOver = 0;
        playerTurn = 1;  // Spieler (X) beginnt

        while (!gameOver) {
            printBoardNCurses();
            if (playerTurn) {
                move = getUserMoveNCurses();
                if (move == -2) {  // Quit-Code erkannt
                    gameOver = 1;
                    replay = 0;
                    break;
                }
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Invalid move. Press any key to try again.");
                    refresh();
                    getch();
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
                gameOver = 1;
            } else if (checkDraw()) {
                printBoardNCurses();
                mvprintw(10, 2, "It's a draw!");
                refresh();
                gameOver = 1;
                result = 'D';
            }
            napms(1000);
            playerTurn = !playerTurn;
        }
        if (replay) {
            updateStats(result);
            mvprintw(12, 2, "Stats: Wins: %d, Losses: %d, Draws: %d", stats.wins, stats.losses, stats.draws);
            refresh();
            napms(2000);
            replay = askReplayNCurses();
        }
    }
}

/* Netzwerkmodus (Server/Client) */
void networkMode(int isServer, const char *server_ip, int port) {
    int sock;
    if (isServer) {
        sock = startServer(port);
    } else {
        sock = connectToServer(server_ip, port);
    }
    
    int replay = 1;
    int move;
    char winner;
    int playerTurn = isServer ? 1 : 0;  // Server beginnt
    char result; // 'X', 'O' oder 'D'
    
    while (replay) {
        initBoard();
        int gameOver = 0;
        while (!gameOver) {
            printBoardNCurses();
            if (playerTurn) {
                move = getUserMoveNCurses();
                if (move == -2) {  // Quit-Code erkannt
                    gameOver = 1;
                    replay = 0;
                    break;
                }
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Invalid move. Press any key to try again.");
                    refresh();
                    getch();
                    continue;
                }
                makeMove(move, isServer ? 'X' : 'O');
                if (sendMove(sock, move) < 0) {
                    mvprintw(10, 2, "Error sending move.");
                    refresh();
                    getch();
                    endGUI();
                    exit(EXIT_FAILURE);
                }
            } else {
                mvprintw(10, 2, "Waiting for opponent's move...");
                clrtoeol();
                refresh();
                if (receiveMove(sock, &move) < 0) {
                    mvprintw(10, 2, "Error receiving move.");
                    refresh();
                    getch();
                    endGUI();
                    exit(EXIT_FAILURE);
                }
                if (!isValidMove(move)) {
                    mvprintw(10, 2, "Opponent made an invalid move.");
                    refresh();
                    getch();
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
            playerTurn = !playerTurn;
        }
        if (replay) {
            updateStats(result);
            mvprintw(12, 2, "Stats: Wins: %d, Losses: %d, Draws: %d", stats.wins, stats.losses, stats.draws);
            refresh();
            napms(2000);
            replay = askReplayNCurses();
        }
    }
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s -1             (Singleplayer mode)\n", argv[0]);
        fprintf(stderr, "  %s -s [port]      (Server mode, default port %d)\n", argv[0], PORT_DEFAULT);
        fprintf(stderr, "  %s -c <ip> [port] (Client mode, default port %d)\n", argv[0], PORT_DEFAULT);
        exit(EXIT_FAILURE);
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
