#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

Stats stats = {0, 0, 0};  // Initialisiert die Statistiken mit Null

// Lädt die Statistiken aus "stats.txt", falls vorhanden.
void loadStats(void) {
    FILE *file = fopen("stats.txt", "r");
    if (file) {
        fscanf(file, "%d %d %d", &stats.wins, &stats.losses, &stats.draws);
        fclose(file);
    }
}

// Speichert die aktuellen Statistiken in "stats.txt".
void saveStats(void) {
    FILE *file = fopen("stats.txt", "w");
    if (file) {
        fprintf(file, "%d %d %d", stats.wins, stats.losses, stats.draws);
        fclose(file);
    }
}

// Aktualisiert die Statistiken anhand des Spielergebnisses.
void updateStats(char result) {
    if (result == 'X') {         // Spieler gewinnt
        stats.wins++;
    } else if (result == 'O') {  // KI gewinnt
        stats.losses++;
    } else if (result == 'D') {  // Unentschieden
        stats.draws++;
    }
}
