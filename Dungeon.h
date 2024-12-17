#ifndef DUNGEON_H
#define DUNGEON_H

#include "Connection.h"

typedef struct 
{
    Room** rooms;
    int room_index;
    int player_index;
    int connection_index;
    Connection** connections;
}Dungeon;

#endif //!DUNGEON_H