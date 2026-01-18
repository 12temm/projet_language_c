#include "structure.h"
#include "config_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadConfig(char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Le fichier de config (%s) n'a pas pu etre lu .\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* chariot = strchr(line, '\n');
        if(chariot != NULL) {
            *chariot = '\0';
        }
        char *equalSign = strchr(line, '=');
        if (equalSign == NULL) {
            continue;
        }
        *equalSign = '\0';
        char *key = line;
        char *value = equalSign + 1;
        if (strcmp(key, "ATTACK_SPEED") == 0) {
            config->attckSpeed = atoi(value);
        }
        if (strcmp(key, "NUM_ENNEMI") == 0) {
            config->numEnnemi = atoi(value);
        }
    }
    fclose(file);
}