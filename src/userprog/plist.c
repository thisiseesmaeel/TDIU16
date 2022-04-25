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

pl_value_t plist_remove_helper(struct plist* pl, pid_t pid){
    // struct process* child = plist_find_by_pid(pl, pid);
    // struct process* parent = plist_find_by_pid(pl, child->parent_pid);

    
    printf("\n\nEntering remove and tries to remove %d\n\n", pid);
    plist_print(pl);


    for (int i = 0; i < PLIST_SIZE; i++)
    {
        //printf("PID: %d\tSTATUS_NEEDED: %d\n", pl->content[i]->my_pid, pl->content[i]->status_needed);
        if (((pl->content[i]) != NULL) && 
            ((pl->content[i]->my_pid) == pid) && 
            (pl->content[i]->status_needed == false))
        {
            struct process* removed_process = pl->content[i];
            pl->content[i] = NULL;
            pl->size--;
            printf("\nFound the process %d and removed it!\n\n", pid);
            printf("plist after removing:\n");
            plist_print(pl);
            return removed_process;
        }
        
    }
    //plist_print(pl);
    //printf("\n");
    
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

    for (int i = 0; i < PLIST_SIZE; i++)
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


pl_value_t plist_find_by_pid(struct plist* pl, pid_t pid){
    for(int i = 0; i < PLIST_SIZE; i++){
        if((pl->content[i] != NULL) && (pl->content[i]-> my_pid == pid))
        {
            return pl->content[i];
        }
    }

    return NULL;
}