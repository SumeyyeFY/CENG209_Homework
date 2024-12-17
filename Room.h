#ifndef ROOM_H
#define ROOM_H

#include "Creature.h"
#include "Item.h"
#include "Coordinate.h"

#define ROOM_WIDTH 5
#define ROOM_LENGTH 5

typedef struct 
{
    int room_id;
    int next_room_id;
    char name[50];
    char* room_description;
    Creature* creature;
    Item* treasure;
    Coordinate* treasure_coordinate;
    Coordinate* exit_coordinate;
    Coordinate* enterance_coordinate;
    char room_map[ROOM_WIDTH][ROOM_LENGTH];
}Room;

void load_room_description(char file_path[], Room* room);
void build_room(Room* room);
void initialize_room(Room* room, char file_path[], Creature* creature, Item* item, 
                     Coordinate* treasure_cord, Coordinate* exit_cord,
                     Coordinate* enterance_cord);
void look(Room* room);
void print_room(Room* room);

#endif //!ROOM_H