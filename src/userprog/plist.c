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
    lock_init (&pl->lock);
}

key_t plist_insert(struct plist *pl, pl_value_t v)
{
    lock_acquire(&pl->lock);

    key_t index = -1;

    if ((pl->size) >= PLIST_SIZE)
    {
        lock_release(&pl->lock);
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

    lock_release(&pl->lock);
    return index;
}

pl_value_t plist_find(struct plist *pl, key_t k)
{
    lock_acquire(&pl->lock);

    if ((pl->content[k]) == NULL)
    {
        lock_release(&pl->lock);
        return NULL;
    }

    lock_release(&pl->lock);
    return pl->content[k];
}

pl_value_t plist_remove(struct plist *pl, key_t k)
{   
    pl_value_t p = plist_find(pl, k);
    
    lock_acquire(&pl->lock);
    if (p != NULL)
    {
        pl->content[k] = NULL;
        pl->size--;
        lock_release(&pl->lock);
        return p;
       
    }
    lock_release(&pl->lock);
    return NULL;
}

pl_value_t plist_remove_helper(struct plist* pl, pid_t pid){
    lock_acquire(&pl->lock);

    for (int i = 0; i < PLIST_SIZE; i++)
    {
        if (((pl->content[i]) != NULL) && 
            ((pl->content[i]->my_pid) == pid))
        {
            struct process* removed_process = pl->content[i];
            pl->content[i] = NULL;
            pl->size--;
            lock_release(&pl->lock);
            return removed_process;
        }
        
    }
    lock_release(&pl->lock);
    return NULL;
}

void plist_print(struct plist *pl)
{
    lock_acquire(&pl->lock);
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
    lock_release(&pl->lock);
    printf("\n\n");
}


pl_value_t plist_find_by_pid(struct plist* pl, pid_t pid){
    lock_acquire(&pl->lock);
    for(int i = 0; i < PLIST_SIZE; i++){
        if((pl->content[i] != NULL) && (pl->content[i]-> my_pid == pid))
        {
            lock_release(&pl->lock);
            return pl->content[i];
        }
    }
    lock_release(&pl->lock);
    return NULL;
}