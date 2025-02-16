#include "netzwerk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ncurses.h>

// Neue Konstanten für Wiederverbindungsversuche
#define RECONNECT_ATTEMPTS 6
#define RECONNECT_INTERVAL_MS 5000

int startServer(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        endwin();
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        endwin();
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        endwin();
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        endwin();
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Waiting for connection on port %d...", port);
    refresh();
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        endwin();
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    close(server_fd);
    mvprintw(0, 2, "Connection established!");
    refresh();
    napms(1000);
    return new_socket;
}

int connectToServer(const char *ip, int port) {
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        endwin();
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        endwin();
        fprintf(stderr, "Invalid address/Address not supported\n");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        endwin();
        fprintf(stderr, "Connection Failed\n");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Connected to server!");
    refresh();
    napms(1000);
    return sock;
}

int sendMove(int sock, int move) {
    int nmove = htonl(move);
    int bytes_sent = send(sock, &nmove, sizeof(nmove), 0);
    if (bytes_sent != sizeof(nmove))
        return -1;
    return 0;
}

int receiveMove(int sock, int *move) {
    int nmove;
    int bytes_received = recv(sock, &nmove, sizeof(nmove), 0);
    if (bytes_received <= 0)
        return -1;
    *move = ntohl(nmove);
    return 0;
}

/**
 * @brief Versucht, eine verlorene Verbindung wiederherzustellen.
 *
 * Nutzt RECONNECT_ATTEMPTS und RECONNECT_INTERVAL_MS zur Steuerung der Versuche.
 *
 * @param server_ip Die IP-Adresse des Servers.
 * @param port Der zu verwendende Port.
 * @return Neuer Socket bei Erfolg, -1 bei Misserfolg.
 */
int attemptReconnect(const char *server_ip, int port) {
    int newSock;
    for (int attempt = 0; attempt < RECONNECT_ATTEMPTS; attempt++) {
        mvprintw(11, 2, "Connection lost. Reconnecting... (%d sec left)", 
                 (RECONNECT_ATTEMPTS - attempt) * (RECONNECT_INTERVAL_MS / 1000));
        refresh();
        napms(RECONNECT_INTERVAL_MS);
        newSock = connectToServer(server_ip, port);
        if (newSock >= 0) {
            return newSock;
        }
    }
    return -1;
}
