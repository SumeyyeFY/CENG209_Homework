#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Item.h"

void load_item_attirbutes(char file_path[], Item *item)
{
    FILE *file;
    file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading!\n");
    }

    char *text = (char *)malloc(256 * sizeof(char));
    char **stats = (char **)malloc(3 * sizeof(char *));

    for (int i = 0; i < 3; i++)
    {
        stats[i] = (char *)malloc(100 * sizeof(char));
    }

    if (ferror(file))
    {
        printf("Error reading file.\n");
    }

    int i = 0;
    while (fgets(text, 100, file))
    {
        strcpy(stats[i], text);
        i++;
    }
    strcpy(item->item_name, stats[2]);
    item->item_strength = atoi(stats[1]);
    strcpy(item->item_description, stats[0]);

    for (int i = 0; i < 3; i++)
    {
        free(stats[i]);
    }
    free(stats);
    free(text);
    fclose(file);
}
