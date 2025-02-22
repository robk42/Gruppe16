#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

Stats stats = {0, 0, 0};

void loadStats(void) {
    FILE *file = fopen("stats.txt", "r");
    if (file) {
        fscanf(file, "%d %d %d", &stats.wins, &stats.losses, &stats.draws);
        fclose(file);
    }
}

void saveStats(void) {
    FILE *file = fopen("stats.txt", "w");
    if (file) {
        fprintf(file, "%d %d %d", stats.wins, stats.losses, stats.draws);
        fclose(file);
    }
}

void updateStats(char result) {
    if (result == 'X') {
        stats.wins++;
    } else if (result == 'O') {
        stats.losses++;
    } else if (result == 'D') {
        stats.draws++;
    }
}
