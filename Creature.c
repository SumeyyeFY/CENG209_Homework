#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Creature.h"

void creature_life_update(Creature* creature){
    if(creature->creature_health)
    {
        creature->isAlive = 1;
    }
    else
    {
        creature->creature_health = 0;
        creature->isAlive = 0;
        printf("%s is dead!\n", creature->creature_name);
    }
}

void load_creature_stats(char file_path[], Creature* creature)
{
    FILE *file;
    file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf("Error opening file for reading!\n");
    }

    char* text = (char*) malloc(100 * sizeof(char));
    char** stats = (char**) malloc(100 * sizeof(char*));

    for(int i = 0; i < 2; i++)
    {
        stats[i] = (char*) malloc(100 * sizeof(char));
    }

    if (ferror(file))
    {
        printf("Error reading file.\n");
    }

    int i = 0;
    while(fgets(text, 100, file))
    {
        strcpy(stats[i], text);
        i++;
    }

    creature->creature_total_health = atoi(stats[0]);
    creature->creature_health = creature->creature_total_health;
    creature->creature_strength = atoi(stats[1]);

    fclose(file);
}