#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Room.h"

void load_room_description(char file_path[], Room* room) 
{
    FILE *file;
    file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf("Error opening file for reading!\n");
    }

    char text[1000];

    while (fgets(text, 1000, file))
    {
        strcpy(room->room_description, text);
    }

    if (ferror(file))
    {
        printf("Error reading file.\n");
    }

    fclose(file); 
}

void build_room(Room* room)
{
    char map[ROOM_WIDTH][ROOM_LENGTH];
    for(int i = 0; i < ROOM_WIDTH; i++)
    {
        for(int j = 0; j < ROOM_LENGTH; j++)
        {
            if(i == room->treasure_coordinate->x && j == room->treasure_coordinate->y)
            {
                map[i][j] = 'T';
            }
            else if(i == room->exit_coordinate->x && j == room->exit_coordinate->y)
            {
                map[i][j] = 'G';
            }
            else if(i == room->enterance_coordinate->x && j == room->enterance_coordinate->y)
            {
                map[i][j] = 'g';
            }
            else
            {
                map[i][j] = '-';
            }
        }
    }
    memcpy(room->room_map, map, sizeof(map));
}

void initialize_room(Room* room, char file_path[], Creature* creature, Item* item, 
                     Coordinate* treasure_cord, Coordinate* exit_cord,
                     Coordinate* enterance_cord)
{
    room->room_description = (char*) malloc(256 * sizeof(char));
    load_room_description(file_path, room);
    room->creature = creature;
    room->treasure = item;
    room->treasure_coordinate = treasure_cord;
    room->exit_coordinate = exit_cord;
    room->enterance_coordinate = enterance_cord;
    build_room(room);
}

void look(Room* room)
{
    printf("%s\n", room->room_description);
}

void print_room(Room* room)
{
    for(int i = 0; i < ROOM_WIDTH; i++)
    {
        for(int j = 0; j < ROOM_LENGTH; j++)
        {
            printf("%c ", room->room_map[i][j]);
        }
        printf("\n");
    }
}