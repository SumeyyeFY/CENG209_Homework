#ifndef ITEM_H
#define ITEM_H

#include "Creature.h"

typedef struct
{
    char item_name[50];
    int item_strength;
    char* item_description;
}Item;

void load_item_attirbutes(char file_path[], Item* item);

#endif //!ITEM_H