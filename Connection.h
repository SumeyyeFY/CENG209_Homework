#ifndef CONNECTION_H
#define CONNECTION_H

#include "Room.h"
#include "Player.h"

typedef struct 
{
    Room* before;
    Room* after;
    Direction direction;
}Connection;

#endif //!CONNECTION_H