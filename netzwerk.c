#include "netzwerk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ncurses.h>

// Startet einen Server, der auf dem angegebenen Port auf eine Verbindung wartet.
int startServer(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Erstelle einen Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        endwin();
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    // Setze Socket-Optionen
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        endwin();
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    // Konfiguriere die Adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    // Binde den Socket an die Adresse
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        endwin();
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    // Lausche auf dem Socket
    if (listen(server_fd, 3) < 0) {
        endwin();
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Waiting for connection on port %d...", port);
    refresh();
    // Akzeptiere eine eingehende Verbindung
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        endwin();
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    // Schließe den Server-Socket, da er nicht mehr benötigt wird
    close(server_fd);
    mvprintw(0, 2, "Connection established!                ");
    refresh();
    napms(1000);
    return new_socket;
}

// Baut als Client eine Verbindung zu einem Server auf.
int connectToServer(const char *ip, int port) {
    int sock;
    struct sockaddr_in serv_addr;
    // Erstelle einen Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        endwin();
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // Konvertiere die IP-Adresse und speichere sie in serv_addr
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        endwin();
        fprintf(stderr, "Invalid address/Address not supported\n");
        exit(EXIT_FAILURE);
    }
    // Versuche, eine Verbindung aufzubauen
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        endwin();
        fprintf(stderr, "Connection Failed\n");
        exit(EXIT_FAILURE);
    }
    mvprintw(0, 2, "Connected to server!                ");
    refresh();
    napms(1000);
    return sock;
}

// Sendet einen Zug (als Integer) über den Socket.
int sendMove(int sock, int move) {
    int nmove = htonl(move);  // Konvertiere in Netzwerk-Byte-Reihenfolge
    int bytes_sent = send(sock, &nmove, sizeof(nmove), 0);
    if (bytes_sent != sizeof(nmove))
        return -1;
    return 0;
}

// Empfängt einen Zug (als Integer) über den Socket.
int receiveMove(int sock, int *move) {
    int nmove;
    int bytes_received = recv(sock, &nmove, sizeof(nmove), 0);
    if (bytes_received <= 0)
        return -1;
    *move = ntohl(nmove);  // Konvertiere zurück in Host-Byte-Reihenfolge
    return 0;
}
