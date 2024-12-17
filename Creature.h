#ifndef CREATURE_H
#define CREATURE_H

typedef struct 
{
    char creature_name[50];
    int isAlive;
    int creature_health;
    int creature_total_health;
    int creature_strength;
}Creature;

void creature_life_update(Creature* creature);
void load_creature_stats(char file_path[], Creature* creature);

#endif //!CREATURE_H