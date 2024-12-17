#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Player.h"

void initialize_player(Player *player)
{
    player->isAlive = 1;
    player->total_health = 100;
    player->health = 100;
    player->inventory_capacity = 3;
    player->inventory = (Item **)malloc(player->inventory_capacity * sizeof(Item *));
    player->used_inventory_space = 0;
    player->coins = 0;
}

void player_life_update(Player *player)
{
    if (player->health)
    {
        if(player->health > player->total_health)
        {
            player->health = player->total_health;
        }
        player->isAlive = 1;
    }
    else
    {
        player->health = 0;
        player->isAlive = 0;
        printf("Player is dead!\n");
        printf("DEFEAT Game Over\n");
    }
}

int is_in_inventory(Player *player, Item *item)
{
    for (int i = 0; i < player->inventory_capacity; i++)
    {
        if (player->inventory[i] == item)
        {
            return 1;
        }
    }
    return 0;
}

void put_old_sign(Room *room, Player *player)
{
    // printf("Inside put old sign\n");
    if (room->treasure_coordinate->x == player->coordinate->x && room->treasure_coordinate->y == player->coordinate->y)
    {
        // printf("Inside T\n");
        room->room_map[player->coordinate->x][player->coordinate->y] = 'T';
    }
    else if (room->exit_coordinate->x == player->coordinate->x && room->exit_coordinate->y == player->coordinate->y)
    {
        // printf("Inside G\n");
        room->room_map[player->coordinate->x][player->coordinate->y] = 'G';
    }
    else if (room->enterance_coordinate->x == player->coordinate->x && room->enterance_coordinate->y == player->coordinate->y)
    {
        // printf("Inside g\n");
        room->room_map[player->coordinate->x][player->coordinate->y] = 'g';
    }
    else
    {
        room->room_map[player->coordinate->x][player->coordinate->y] = '-';
    }
}

void attack(Player *player, Item *item, Creature *creature)
{
    if (is_in_inventory(player, item))
    {
        if (creature->isAlive | player->isAlive)
        {
            creature->creature_health += item->item_strength;
            printf("Player is attacked! %d health to the %s (Remaining health: %d)\n", item->item_strength, creature->creature_name, creature->creature_health);
        }
        creature_life_update(creature);
    }
    else
    {
        printf("Item %s is not found! Unsuccessful attack! \n", item->item_name);
    }
}

void creature_attack(Creature *creature, Player *player)
{
    if(!player->inArmour)
    {
        player->health += creature->creature_strength;
        //player->health = player->health += creature->creature_strength < 0 ? player->health += creature->creature_strength : 0;
    }
    else
    {
        player->health += creature->creature_strength / 2;
        //player->health = (player->health += creature->creature_strength / 2) < 0 ? (player->health += creature->creature_strength / 2) : 0;
    }
    printf("%s is attacked! %d health (Reamining health: %d)\n", creature->creature_name, creature->creature_strength, player->health);
    player_life_update(player);
}

void move(Player *player, Direction direction, Room *room)
{
    // printf("Inside move\n");
    switch (direction)
    {
        // printf("Inside switch\n");
    case LEFT:
        if (player->coordinate->y - 1 >= 0)
        {
            put_old_sign(room, player);
            player->coordinate->y -= 1;
            printf("Moved to %d %d\n", player->coordinate->x, player->coordinate->y);
        }
        else
        {
            printf("You hit the wall! \n");
        }
        break;
    case RIGTH:
        // printf("Inside rigth\n");
        if (player->coordinate->y + 1 < ROOM_LENGTH)
        {
            // printf("Inside if\n");
            put_old_sign(room, player);
            player->coordinate->y += 1;
            // printf("Inside updated coord\n");
            printf("Moved to %d %d\n", player->coordinate->x, player->coordinate->y);
        }
        else
        {
            printf("You hit the wall! \n");
        }
        break;
    case UP:
        if (player->coordinate->x - 1 < ROOM_WIDTH)
        {
            put_old_sign(room, player);
            player->coordinate->x -= 1;
            printf("Moved to %d %d\n", player->coordinate->x, player->coordinate->y);
        }
        else
        {
            printf("You hit the wall! \n");
        }
        break;
    case DOWN:
        if (player->coordinate->x + 1 >= 0)
        {
            put_old_sign(room, player);
            player->coordinate->x += 1;
            printf("Moved to %d %d\n", player->coordinate->x, player->coordinate->y);
        }
        else
        {
            printf("You hit the wall! \n");
        }
        break;
    default:
        printf("Invalid direction!\n");
        break;
    }

    // printf("Switches ended\n");

    if (room->room_map[player->coordinate->x][player->coordinate->y] == 'T')
    {
        printf("You found a treasure! It is %s! Pick it up or not. \n", room->treasure->item_name);
    }

    if (room->room_map[player->coordinate->x][player->coordinate->y] == 'G')
    {
        printf("You are in the gate! \n");
    }

    // printf("Where you are said\n");

    room->room_map[player->coordinate->x][player->coordinate->y] = 'P';
    // printf("Player\n");
    // free(room);
}

void update_inventory_capacity(Player *player)
{
    if (player->coins >= 15)
    {
        player->inventory_capacity += 3;
        player->coins -= 15;
        printf("Your inventory capacity incremented by 3! (-15 coins)\n");
    }
    else
    {
        printf("You cannot afford new inventory memory! (15+ coins needed)\n");
    }
}

void pickup(Player *player, Item *item, Room *room)
{
    if (!room->creature->isAlive)
    {
        if (player->used_inventory_space >= player->inventory_capacity)
        {
            printf("Inventory is full! You need to upgrade! \n");
        }
        else
        {
            player->inventory[player->used_inventory_space++] = item;
            room->room_map[room->treasure_coordinate->x][room->treasure_coordinate->y] = '1';
            room->treasure = NULL;
            printf("You picked up %s!\n", item->item_name);

            if(item->item_strength) //For potions
            {
                printf("You gained %d health from %s! \n", item->item_strength, item->item_name);
                player->inventory[player->used_inventory_space--] = NULL; 
            }
        }
    }
    else
    {
        printf("You cannot take treasure unless you killed the %s!\n", room->creature->creature_name);
    }
}

void inventory(Player *player)
{
    for (int i = 0; i < player->used_inventory_space; i++)
    {
        printf("%d: %s\n", i+1, player->inventory[i]->item_description);
    }
}