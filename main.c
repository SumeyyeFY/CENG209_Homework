#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Item.h"
#include "Creature.h"
#include "Player.h"
#include "Room.h"
#include "Dungeon.h"

void load_file(Dungeon* dungeon, char file_path[]);
void save_file(Dungeon* dungeon, char file_path[]);
void save_file_connection(Connection* con, FILE* file);
void load_file_connection(Connection* con, FILE* file);
void save_file_player(Player* player, FILE* file);
void load_file_player(Player* player, FILE* file);
void save_file_room(Room* room, FILE* file);
void load_file_room(Room* room, FILE* file);
Item* find_item(Player* player, char item_name[]);
void menubar();
Creature* create_creature(char name[], char file_path[]);
Item* create_item(char name[], char file_path[]);
Dungeon* create_dungeon();
void go_next_room(Dungeon *dungeon, Player *player);
void clean(Dungeon* dungeon, Player* player);

int main()
{
    Player *player;
    initialize_player(player);

    Dungeon *dungeon = create_dungeon();
    player->room = dungeon->rooms[dungeon->player_index];

    printf("Hello, warrior. Welcome to Hidden Dundeon Game. May I learn your name? ");
    scanf(" %s", player->player_name);
    printf("Greetings again %s. Here is introduction of game:\n", player->player_name);
    printf("You can move 4 directions: left, rigth, up and down \n");
    printf("Each room has a creature that will attack you immediately. You should kill them before they kill you \n");
    printf("Once you left the room, there is no coming back. Give your decisions wisely.\n");
    printf("\nLet's start the game! \n");

    char cmd[10];
    char dir[10];
    char item[20];
    char yes_no[5];
    char file_path[1000];
    while(dungeon->rooms[dungeon->player_index]->next_room_id >= 0) //In the last room, next_room_id becomes -1
    {
        menubar();
        scanf(" %s", cmd);
        getchar();

        if(strcmp(strlwr(cmd), "move"))
        {
            printf("Enter direction: ");
            scanf(" %s", dir);
            getchar();

            if(strcmp(strlwr(dir), "left")){
                move(player, LEFT, dungeon->rooms[dungeon->player_index]);
            }else if(strcmp(strlwr(dir), "right")){
                move(player, RIGTH, dungeon->rooms[dungeon->player_index]);
            }else if(strcmp(strlwr(dir), "up")){
                move(player, UP, dungeon->rooms[dungeon->player_index]);
            }else if(strcmp(strlwr(dir), "down")){
                move(player, DOWN, dungeon->rooms[dungeon->player_index]);
            }else{
                printf("Invalid direction!\n");
            }
        }
        else if(strcmp(strlwr(cmd), "look"))
        {
            look(dungeon->rooms[dungeon->player_index]);
        }
        else if(strcmp(strlwr(cmd), "print"))
        {
            print_room(dungeon->rooms[dungeon->player_index]);
        }
        else if(strcmp(strlwr(cmd), "inventory"))
        {
            inventory(player);
        }
        else if(strcmp(strlwr(cmd), "update_inventory"))
        {
            update_inventory_capacity(player);
        }
        else if(strcmp(strlwr(cmd), "pickup"))
        {
            pickup(player, player->room->treasure, dungeon->rooms[dungeon->player_index]);
        }
        else if(strcmp(strlwr(cmd), "attack"))
        {
            printf("Choose item from inventory: ");
            scanf(" %s", item);
            getchar();

            Item* item_ptr = find_item(player, item);
            if(item_ptr == NULL)
            {
                printf("There is no %s in the inventory\n", item);
            }else{
                attack(player, item_ptr, dungeon->rooms[dungeon->player_index]->creature);
            }
            creature_attack(dungeon->rooms[dungeon->player_index]->creature, player);
            free(item_ptr);
        }
        else if(strcmp(strlwr(cmd), "list"))
        {

        }
        else if(strcmp(strlwr(cmd), "save"))
        {
            printf("Enter file path: ");
            scanf(" %s", file_path);
            getchar();

            save_file(dungeon, file_path);
            printf("File saved, exiting...\n");
            break;
        }
        else if(strcmp(strlwr(cmd), "load"))
        {
            printf("Enter file path: ");
            scanf(" %s", file_path);
            getchar();

            load_file(dungeon, file_path);
            printf("File loadding, please wait...\n");
        }
        else if(strcmp(strlwr(cmd), "exit"))
        {
            printf("Do you want to save the game? ");
            scanf(" %s", yes_no);
            getchar();

            if(strcmp(strlwr(yes_no), "yes"))
            {
                printf("Enter file path: ");
                scanf(" %s", file_path);
                getchar();

                save_file(dungeon, file_path);
                printf("File saved, exiting...\n");
            }
            break;
        }
        else
        {
            printf("Invalid command! \n");
        }
    }

    printf("Goodbye!\n");
    free(cmd);
    free(dir);
    free(yes_no);
    free(item);
    free(file_path);
    clean(dungeon, player);
    return 0;
}

void load_file_room(Room* room, FILE* file)
{
        int result = 0;
        //Creature
        result += fscanf("%d %d %d %d %d\n" ,&room->creature->creature_health, &room->creature->creature_name, &room->creature->creature_strength, &room->creature->creature_total_health, &room->creature->isAlive);

        //Item
        result += fscanf("%s %s %d\n", room->treasure->item_description, room->treasure->item_name, &room->treasure->item_strength);
    
        //treasure_coordinate
        result += fscanf("%d %d\n", &room->treasure_coordinate->x, &room->treasure_coordinate->y);

        //enterance_coordinate
        result += fscanf("%d %d\n", &room->enterance_coordinate->x, &room->enterance_coordinate->y);

        //exit_coordinate
        result += fscanf("%d %d\n", &room->exit_coordinate->x, &room->exit_coordinate->y);

        //Map
        for(int w = 0; w < ROOM_WIDTH; w++)
        {
            result += fscanf("%c %c %c %c %c\n", &room->room_map[w][0], &room->room_map[w][1], &room->room_map[w][2], &room->room_map[w][3], &room->room_map[w][4]);
        }

        if(result != 39)
        {
            printf("Error occured during file reading! \n");
        }
}

void save_file_room(Room* room, FILE* file)
{
        //Creature
        fprintf("%d %d %d %d %d\n" ,room->creature->creature_health, room->creature->creature_name, room->creature->creature_strength, room->creature->creature_total_health, room->creature->isAlive);

        //Item
        fprintf("%s %s %d\n", room->treasure->item_description, room->treasure->item_name, room->treasure->item_strength);
    
        //treasure_coordinate
        fprintf("%d %d\n", room->treasure_coordinate->x, room->treasure_coordinate->y);

        //enterance_coordinate
        fprintf("%d %d\n", room->enterance_coordinate->x, room->enterance_coordinate->y);

        //exit_coordinate
        fprintf("%d %d\n", room->exit_coordinate->x, room->exit_coordinate->y);

        //Map
        for(int w = 0; w < ROOM_WIDTH; w++)
        {
            fprintf("%c %c %c %c %c\n", room->room_map[w][0], room->room_map[w][1], room->room_map[w][2], room->room_map[w][3], room->room_map[w][4]);
        }
}

void load_file_player(Player* player, FILE* file)
{
    fscanf("%c %d %d %d %d\n", player->player_name, player->isAlive, player->inArmour, player->health, player->total_health);

    //coordinate
    fscanf("%d %d\n", player->coordinate->x, player->coordinate->y);

    fscanf("%d %d %d\n", player->inventory_capacity, player->used_inventory_space, player->coins);

    //Inventory
    for(int i = 0; i < player->inventory_capacity; i++)
    {
        fscanf("%s %s %d\n", player->inventory[i]->item_description , player->inventory[i]->item_name, player->inventory[i]->item_strength);
    }

    //Room
    load_file_room(player->room, file);
}

void save_file_player(Player* player, FILE* file)
{
    fprintf("%c %d %d %d %d\n", player->player_name, player->isAlive, player->inArmour, player->health, player->total_health);

    //coordinate
    fprintf("%d %d\n", player->coordinate->x, player->coordinate->y);

    fprintf("%d %d %d\n", player->inventory_capacity, player->used_inventory_space, player->coins);

    //Inventory
    for(int i = 0; i < player->inventory_capacity; i++)
    {
        fprintf("%s %s %d\n", player->inventory[i]->item_description , player->inventory[i]->item_name, player->inventory[i]->item_strength);
    }

    //Room
    save_file_room(player->room, file);
}

void load_file_connection(Connection* con, FILE* file)
{
    load_file_room(con->after, file);
    load_file_room(con->before, file);
    
    fscanf("%d\n", con->direction);
}

void save_file_connection(Connection* con, FILE* file)
{
    save_file_room(con->after, file);
    save_file_room(con->before, file);
    
    fprintf("%d\n", con->direction);
}

void save_file(Dungeon* dungeon, char file_path[])
{

    FILE *file;
    file = fopen(file_path, "w");
    if(file == NULL)
    {
        printf("Error opening file for writing!\n");
    }

    //Rooms
    for(int r = 0; r < dungeon->room_index; r++)
    {
        save_file_room(dungeon->rooms[r], file);
    }

    //integer values
    fprintf("%d %d %d\n", dungeon->connection_index, dungeon->player_index, dungeon->room_index);

    for(int c = 0; c < dungeon->connection_index; c++)
    {
    save_file_connection(dungeon->connections[c], file);
    }

    fclose(file);

}

void load_file(Dungeon* dungeon, char file_path[])
{
    FILE* file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf("Error opening file for reading!\n");
    }

    //Rooms
    for(int r = 0; r < dungeon->room_index; r++)
    {
        load_file_room(dungeon->rooms[r], file);
    }

    //integer values
    fscanf("%d %d %d\n", dungeon->connection_index, dungeon->player_index, dungeon->room_index);

    for(int c = 0; c < dungeon->connection_index; c++)
    {
    load_file_connection(dungeon->connections[c], file);
    }

    fclose(file);

}

Item* find_item(Player* player, char item_name[])
{
    for(int i = 0; i < player->inventory_capacity; i++)
    {
        if(strcmp(player->inventory[i]->item_name, item_name))
        {
            return player->inventory[i];
        }
    }
    return NULL;
}

void menubar()
{
    printf("Game Commands: move<direction> | look | print | inventory | update_inventory | pickup<item> | attack\n");
    printf("Menu Commands: list | save <filepath> | load <filepath> | exit \n");
}

Creature* create_creature(char name[], char file_path[])
{
    Creature *creature;
    strcpy(creature->creature_name, name);
    load_creature_stats(file_path, creature);
    return creature;
}

Item* create_item(char name[], char file_path[])
{
    Item *item;
    strcpy(item->item_name, name);
    load_item_attirbutes(file_path, item);
    return item;
}

Dungeon* create_dungeon()
{
    Dungeon dungeon = {(Room **)malloc(8 * sizeof(Room *)), 0, 0, 0, (Connection **)malloc(7 * sizeof(Connection *))};

    // Room Snake
    Creature *snake = create_creature("Snake", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\Snake.txt");
    Item *dagger = create_item("Dragger", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\Dagger.txt");

    Room *snake_room;
    strcpy(snake_room->name, "Snake Room");
    snake_room->room_id = 1;
    snake_room->next_room_id = 2;

    Coordinate snake_treasure = {2, 2};
    Coordinate snake_exit = {0, 4};
    Coordinate snake_entrance = {0, 0};

    initialize_room(snake_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\SnakesRoom.txt",
                    snake, dagger, &snake_treasure, &snake_exit, &snake_entrance);
    dungeon.rooms[dungeon.room_index++] = snake_room;

    // Room Goblin
    Creature *goblin = create_creature("Goblin", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\Goblin.txt");
    Item *sword = create_item("Sword", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\Sword.txt");

    Room *goblin_room;
    strcpy(goblin_room->name, "Goblin Room");
    goblin_room->room_id = 2;
    goblin_room->next_room_id = 1;

    Coordinate goblin_treasure = {3, 0};
    Coordinate goblin_exit = {4, 4};
    Coordinate goblin_entrance = {0, 0};

    initialize_room(goblin_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\GoblinRoom.txt",
                    goblin, sword, &goblin_treasure, &goblin_exit, &goblin_entrance);
    dungeon.rooms[dungeon.room_index++] = goblin_room;

    // Room Tiger
    Creature *tiger = create_creature("Tiger", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\Tiger.txt");
    Item *tnt = create_item("Sword", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\Tnt.txt");

    Room *tiger_room;
    strcpy(tiger_room->name, "Tiger Room");
    tiger_room->room_id = 3;
    tiger_room->next_room_id = 4;

    Coordinate tiger_treasure = {2, 2};
    Coordinate tiger_exit = {0, 4};
    Coordinate tiger_entrance = {0, 0};

    initialize_room(tiger_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\TigersRoom.txt",
                    tiger, tnt, &tiger_treasure, &tiger_exit, &tiger_entrance);
    dungeon.rooms[dungeon.room_index++] = tiger_room;

    // Room Ice Gnome
    Creature *gnome = create_creature("Ice Gnome", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\IceGnome.txt");
    Item *arrow = create_item("Arrow", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\ArrowAndBow.txt");

    Room *gnome_room;
    strcpy(gnome_room->name, "Gnome Room");
    gnome_room->room_id = 4;
    gnome_room->next_room_id = 5;

    Coordinate gnome_treasure = {0, 0};
    Coordinate gnome_exit = {4, 0};
    Coordinate gnome_entrance = {2, 4};

    initialize_room(gnome_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\GnomesRoom.txt",
                    gnome, arrow, &gnome_treasure, &gnome_exit, &gnome_entrance);
    dungeon.rooms[dungeon.room_index++] = gnome_room;

    // Room Genie
    Creature *genie = create_creature("Genie", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\Genie.txt");
    Item *red_sword = create_item("Red Sword", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\RedSword.txt");

    Room *genie_room;
    strcpy(genie_room->name, "Genie Room");
    genie_room->room_id = 5;
    genie_room->next_room_id = 6;

    Coordinate genie_treasure = {4, 0};
    Coordinate genie_exit = {0, 4};
    Coordinate genie_entrance = {0, 0};

    initialize_room(genie_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\GeniesRoom.txt",
                    genie, red_sword, &genie_treasure, &genie_exit, &genie_entrance);
    dungeon.rooms[dungeon.room_index++] = genie_room;

    // Room Rock Monster
    Creature *rock = create_creature("Rock Monster", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\RockMonster.txt");
    Item *ultimate_potion = create_item("Ultimate Health Potion", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\Ultimate.txt");

    Room *rock_room;
    strcpy(rock_room->name, "Rock Monster Room");
    rock_room->room_id = 6;
    rock_room->next_room_id = 7;

    Coordinate rock_treasure = {3, 2};
    Coordinate rock_exit = {4, 4};
    Coordinate rock_entrance = {3, 0};

    initialize_room(rock_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\RockMonstersRoom.txt",
                    rock, ultimate_potion, &rock_treasure, &rock_exit, &rock_entrance);
    dungeon.rooms[dungeon.room_index++] = rock_room;

    // Room Great Wizard
    Creature *wizard = create_creature("Great Wizard", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\GreatWizard.txt");
    Item *ulti_sword = create_item("Ultimate Sword", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\UltimateSword.txt");

    Room *wizard_room;
    strcpy(wizard_room->name, "Great Wizard Room");
    wizard_room->room_id = 7;
    wizard_room->next_room_id = 8;

    Coordinate wizard_treasure = {1, 0};
    Coordinate wizard_exit = {4, 0};
    Coordinate wizard_entrance = {0, 4};

    initialize_room(wizard_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\GreatWizardsRoom.txt",
                    wizard, ulti_sword, &wizard_treasure, &wizard_exit, &wizard_entrance);
    dungeon.rooms[dungeon.room_index++] = wizard_room;

    // Room Dragon
    Creature *dragon = create_creature("Dragon", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Creatures\\Dragon.txt");
    Item *dragon_book = create_item("Dragon Book", "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Items\\DragonBook.txt");

    Room *dragon_room;
    strcpy(dragon_room->name, "Dragon Room");
    dragon_room->room_id = 8;
    dragon_room->next_room_id = -1;

    Coordinate dragon_treasure = {4, 0};
    Coordinate dragon_exit = {1, 0};
    Coordinate dragon_entrance = {4, 4};

    initialize_room(dragon_room, "C:\\Users\\HP\\Desktop\\CENG209Homework\\Descriptions\\Rooms\\DragonsRoom.txt",
                    dragon, dragon_book, &dragon_treasure, &dragon_exit, &dragon_entrance);
    dungeon.rooms[dungeon.room_index] = dragon_room;

    Connection snake_goblin = {snake_room, goblin_room, RIGTH};
    dungeon.connections[dungeon.connection_index++] = &snake_goblin;
    Connection goblin_tiger = {goblin_room, tiger_room, DOWN};
    dungeon.connections[dungeon.connection_index++] = &goblin_tiger;
    Connection tiger_gnome = {tiger_room, gnome_room, LEFT};
    dungeon.connections[dungeon.connection_index++] = &tiger_gnome;
    Connection gnome_genie = {gnome_room, genie_room, DOWN};
    dungeon.connections[dungeon.connection_index++] = &gnome_genie;
    Connection genie_rock = {genie_room, rock_room, RIGTH};
    dungeon.connections[dungeon.connection_index++] = &genie_rock;
    Connection rock_wizard = {rock_room, wizard_room, DOWN};
    dungeon.connections[dungeon.connection_index++] = &rock_wizard;
    Connection wizard_dragon = {wizard_room, dragon_room, LEFT};
    dungeon.connections[dungeon.connection_index] = &wizard_dragon;

    return &dungeon;
}

void go_next_room(Dungeon *dungeon, Player *player)
{
    player->room = dungeon->connections[dungeon->player_index]->after;
    printf("Changing room... \n");
    player->coordinate->x = player->room->enterance_coordinate->x;
    player->coordinate->y = player->room->enterance_coordinate->y;
    player->coins += 5;
    printf("+5 coins for changing room! \n");
    look(player->room);
}

void clean(Dungeon* dungeon, Player* player)
{
    for(int i = 0; i < dungeon->connection_index; i++)
    {
        free(dungeon->connections[i]);
    }
    free(dungeon->connections);

    for(int i = 0; i < dungeon->room_index; i++)
    {
        for(int j = 0; j < ROOM_WIDTH; j++)
        {
            for(int k = 0; k < ROOM_WIDTH; k++)
            {
                free(dungeon->rooms[i]->room_map[j][k]);
            }
            free(dungeon->rooms[i]->room_map[j]);
        }
        free(dungeon->rooms[i]->creature);
        free(dungeon->rooms[i]->treasure);
        free(dungeon->rooms[i]->exit_coordinate);
        free(dungeon->rooms[i]->enterance_coordinate);
        free(dungeon->rooms[i]->treasure_coordinate);
        free(dungeon->rooms[i]);
    }

    free(player);
}