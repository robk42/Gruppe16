#ifndef NETZWERK_H
#define NETZWERK_H

/**
 * @brief Startet einen Server, der auf dem angegebenen Port auf eine Verbindung wartet.
 * @param port Der Port, auf dem der Server lauscht.
 * @return Der Socket, der die Verbindung zum Client repräsentiert.
 */
int startServer(int port);

/**
 * @brief Baut als Client eine Verbindung zu einem Server auf.
 * @param ip Die IP-Adresse des Servers.
 * @param port Der Port, auf dem der Server lauscht.
 * @return Der Socket, der die Verbindung repräsentiert.
 */
int connectToServer(const char *ip, int port);

/**
 * @brief Sendet einen Zug (als Integer) über den Socket.
 * @param sock Der Socket, über den gesendet wird.
 * @param move Der zu sendende Zug.
 * @return 0 bei Erfolg, -1 bei Fehler.
 */
int sendMove(int sock, int move);

/**
 * @brief Empfängt einen Zug (als Integer) über den Socket und wandelt ihn um.
 * @param sock Der Socket, von dem empfangen wird.
 * @param move Pointer, unter dem der empfangene Zug gespeichert wird.
 * @return 0 bei Erfolg, -1 bei Fehler.
 */
int receiveMove(int sock, int *move);

/**
 * @brief Versucht, eine verlorene Verbindung wiederherzustellen.
 * @param server_ip Die IP-Adresse des Servers.
 * @param port Der zu verwendende Port.
 * @return Neuer Socket bei Erfolg, -1 bei Misserfolg.
 */
int attemptReconnect(const char *server_ip, int port);

#endif  /* NETZWERK_H */
