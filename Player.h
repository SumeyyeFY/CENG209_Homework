#ifndef PLAYER_H
#define PLAYER_H

#include "Item.h"
#include "Room.h"

typedef enum
{
    LEFT, RIGTH, UP, DOWN
}Direction;

typedef struct 
{
    char player_name[50];
    int isAlive;
    int inArmour;
    int health;
    int total_health;
    Coordinate* coordinate;
    int inventory_capacity;
    int used_inventory_space;
    int coins;
    Item** inventory;
    Room* room;
}Player;

void initialize_player(Player* player);
void player_life_update(Player* player);
int is_in_inventory(Player* player, Item* item);
void put_old_sign(Room *room, Player *player);

void attack(Player* player, Item* item, Creature* creature);
void creature_attack(Creature* creature, Player* player);
void move(Player* player, Direction direction, Room* room);
void update_inventory_capacity(Player* player);
void pickup(Player* player, Item* item, Room* room);
void inventory(Player* player);

#endif //!PLAYER_H