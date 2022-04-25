#ifndef USERPROG_PLIST_H_
#define USERPROG_PLIST_H_

#include <stdbool.h>
#include "threads/synch.h"

/* Place functions to handle a running process here (process list).
   
   plist.h : Your function declarations and documentation.
   plist.c : Your implementation.

   The following is strongly recommended:

   - A function that given process inforamtion (up to you to create)
     inserts this in a list of running processes and return an integer
     that can be used to find the information later on.

   - A function that given an integer (obtained from above function)
     FIND the process information in the list. Should return some
     failure code if no process matching the integer is in the list.
     Or, optionally, several functions to access any information of a
     particular process that you currently need.

   - A function that given an integer REMOVE the process information
     from the list. Should only remove the information when no process
     or thread need it anymore, but must guarantee it is always
     removed EVENTUALLY.
     
   - A function that print the entire content of the list in a nice,
     clean, readable format.
     
 */


typedef int pid_t;

struct process
{
  pid_t parent_pid;
  pid_t my_pid;
  int status;
  bool status_needed;
  bool alive;
};

typedef struct process* pl_value_t;
typedef int key_t; 

#define PLIST_SIZE 128

struct plist {
    pl_value_t content[PLIST_SIZE];
    int size;

    struct lock lock;
};


void plist_init(struct plist* pl);

key_t plist_insert(struct plist* pl, pl_value_t v);

pl_value_t plist_find(struct plist* pl, key_t k);

pl_value_t plist_remove(struct plist* pl, key_t k);

pl_value_t plist_remove_helper(struct plist* pl, pid_t pid);

void plist_print(struct plist* pl);

pl_value_t plist_find_by_pid(struct plist* pl, pid_t pid);

#endif
