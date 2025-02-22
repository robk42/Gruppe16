#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>    // For close()
#include <ncurses.h>
#include "spiellogik.h"
#include "netzwerk.h"  // Network functions (reconnect removed)
#include "gui.h"
#include "stats.h"


#define PORT_DEFAULT 12345

// Prototypen
void displayErrorAndExit(const char *errorMessage);
int mainMenu(void);
void getServerIP(char *ipBuffer, size_t bufferSize);
void displayAndWaitStats(void);
void toggleTurn(int *turn);

void displayAndWaitStats(void) {
    clear();
    mvprintw(2, 2, "Game Over!");
    mvprintw(4, 2, "Stats: Wins: %d, Losses: %d, Draws: %d", 
             stats.wins, stats.losses, stats.draws);
    mvprintw(6, 2, "Press any key to continue...");
    refresh();
    getch();
}

void displayErrorAndExit(const char *errorMessage) {
    endGUI();
    fprintf(stderr, "%s\n", errorMessage);
    exit(EXIT_FAILURE);
}

void toggleTurn(int *turn) {
    *turn = !(*turn);
    napms(1000);
}

int mainMenu(void) {
    int choice = 0;
    int ch;
    do {
        clear();
        // Banner in Orange (Farb-Paar 5)
        attron(A_BOLD | COLOR_PAIR(5));
        mvprintw(2, 2, "*************************************");
        mvprintw(3, 2, "*       WELCOME TO TIC TAC TOE      *");
        mvprintw(4, 2, "*************************************");
        attroff(A_BOLD | COLOR_PAIR(5));
        mvprintw(6, 2, "Select Mode:");
        mvprintw(8, 4, "1. Singleplayer (Player vs. AI)");
        mvprintw(9, 4, "2. Multiplayer - Server");
        mvprintw(10, 4, "3. Multiplayer - Client");
        mvprintw(11, 4, "4. Quit");
        mvprintw(13, 2, "Enter your choice (1-4): ");
        refresh();
        ch = getch();
        if (ch >= '1' && ch <= '4') {
            choice = ch - '0';
        }
    } while (choice < 1 || choice > 4);
    return choice;
}

void getServerIP(char *ipBuffer, size_t bufferSize) {
    echo();
    clear();
    mvprintw(2, 2, "Enter server IP address: ");
    refresh();
    getnstr(ipBuffer, bufferSize - 1);
    noecho();
}

char playSingleGame(void) {
    int move;
    char winner;
    int playerTurn = 1;
    char result = 'D';

    initBoard();
    while (1) {
        printBoardNCurses();
        if (playerTurn) {
            move = getUserMoveNCurses();
            if (move == -2) {
                result = 'D';
                break;
            }
            if (!isValidMove(move)) {
                displayMessage("Invalid move. Choose an empty cell.");
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
                mvprintw(10, 2, "You lose!");
                refresh();
                winAnimation("You lose!");
                result = 'O';
            }
            break;
        } else if (checkDraw()) {
            printBoardNCurses();
            mvprintw(10, 2, "It's a draw!");
            refresh();
            winAnimation("It's a draw!");
            result = 'D';
            break;
        }
        toggleTurn(&playerTurn);
    }
    return result;
}

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
                    displayMessage("Invalid move. Choose an empty cell.");
                    continue;
                }
                makeMove(move, isServer ? 'X' : 'O');
                if (sendMove(sock, move) < 0) {
                    displayErrorAndExit("Error sending move.");
                }
            } else {
                mvprintw(10, 2, "Waiting for opponent's move...");
                clrtoeol();
                refresh();
                if (receiveMove(sock, &move) < 0) {
                    displayErrorAndExit("Error receiving opponent's move.");
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
                if (isServer) {
                    if (winner == 'X') {
                        mvprintw(10, 2, "You win!");
                        refresh();
                        winAnimation("You win!");
                        result = 'X';
                    } else {
                        mvprintw(10, 2, "You lose!");
                        refresh();
                        winAnimation("You lose!");
                        result = 'O';
                    }
                } else {
                    if (winner == 'O') {
                        mvprintw(10, 2, "You win!");
                        refresh();
                        winAnimation("You win!");
                        result = 'X';
                    } else {
                        mvprintw(10, 2, "You lose!");
                        refresh();
                        winAnimation("You lose!");
                        result = 'O';
                    }
                }
                gameOver = 1;
            } else if (checkDraw()) {
                printBoardNCurses();
                mvprintw(10, 2, "It's a draw!");
                refresh();
                winAnimation("It's a draw!");
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

int main(int argc, char *argv[]) {
    int mode = 0;
    char serverIP[50] = {0};
    int port = PORT_DEFAULT;

    if (argc < 2) {
        initGUI();
        mode = mainMenu();
    } else {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage:\n");
            printf("  %s -1             (Singleplayer mode: Player vs. AI)\n", argv[0]);
            printf("  %s -s [port]      (Server mode, default port %d)\n", argv[0], PORT_DEFAULT);
            printf("  %s -c <ip> [port] (Client mode, default port %d)\n", argv[0], PORT_DEFAULT);
            exit(EXIT_SUCCESS);
        }
        if (strcmp(argv[1], "-1") == 0)
            mode = 1;
        else if (strcmp(argv[1], "-s") == 0) {
            mode = 2;
            if (argc >= 3)
                port = atoi(argv[2]);
        } else if (strcmp(argv[1], "-c") == 0) {
            mode = 3;
            if (argc >= 3)
                strncpy(serverIP, argv[2], sizeof(serverIP) - 1);
            else {
                fprintf(stderr, "Client mode requires server IP address.\n");
                exit(EXIT_FAILURE);
            }
            if (argc >= 4)
                port = atoi(argv[3]);
        } else {
            fprintf(stderr, "Invalid mode.\n");
            exit(EXIT_FAILURE);
        }
        initGUI();
    }

    switch (mode) {
        case 1:
            singlePlayerMode();
            break;
        case 2:
            networkMode(1, NULL, port);
            break;
        case 3:
            if (serverIP[0] == '\0')
                getServerIP(serverIP, sizeof(serverIP));
            networkMode(0, serverIP, port);
            break;
        case 4:
            endGUI();
            exit(EXIT_SUCCESS);
        default:
            displayErrorAndExit("Invalid mode selection.");
    }

    saveStats();
    endGUI();
    return 0;
}
