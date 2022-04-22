#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "plist.h"

void plist_init(struct plist *pl)
{
    for (int i = 0; i < PLIST_SIZE; i++)
    {
        pl->content[i] = NULL;
    }
    pl->size = 0;
}

key_t plist_insert(struct plist *pl, pl_value_t v)
{
    key_t index = -1;

    if ((pl->size) >= PLIST_SIZE)
    {
        return -1;
    }

    for (int i = 0; i < PLIST_SIZE; i++)
    {
        if ((pl->content[i]) == NULL)
        {
            pl->content[i] = v;
            pl->size++;
            index = i;
            break;
        }
    }

    return index;
}

pl_value_t plist_find(struct plist *pl, key_t k)
{

    if ((pl->content[k]) == NULL)
    {
        return NULL;
    }
    return pl->content[k];
}

pl_value_t plist_remove(struct plist *pl, key_t k)
{
    pl_value_t p = plist_find(pl, k);
    if (p != NULL)
    {
        pl->content[k] = NULL;
        pl->size--;
        return p;
    }
    return NULL;
}

void plist_print(struct plist *pl)
{
    const int number_of_chars = 73;

    putchar('\n');
    printf("%-15s%-15s%-15s%-15s%-15s\n", "PID", "Alive",
           "Status", "Parent id", "Status needed");
    for (int k = 0; k < number_of_chars; k++)
        putchar('=');

    for (int i = 0; i < pl->size; i++)
    {
        if (pl->content[i] != NULL)
        {
            putchar('\n');
            printf("%d) %-12d | %-12d | %-12d | %-12d | %-12d\n", i+1,
                                            pl->content[i]->my_pid,
                                            pl->content[i]->alive,
                                            pl->content[i]->status,
                                            pl->content[i]->parent_pid,
                                            pl->content[i]->status_needed);
        }
    }

    printf("\n\n");
}