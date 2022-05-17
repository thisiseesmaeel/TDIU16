#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* header files you probably need, they are not used yet */
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/input.h"
#include "../lib/kernel/console.h"
#include "userprog/flist.h"
#include "userprog/plist.h"
#include "devices/timer.h"
#include "limits.h"


static void syscall_handler (struct intr_frame *);

struct file* retrieve_file(int32_t );

bool verify_fix_length(void* , unsigned );

bool verify_variable_length(char* );

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


/* This array defined the number of arguments each syscall expects.
   For example, if you want to find out the number of arguments for
   the read system call you shall write:
   
   int sys_read_arg_count = argc[ SYS_READ ];
   
   All system calls have a name such as SYS_READ defined as an enum
   type, see `lib/syscall-nr.h'. Use them instead of numbers.
 */
const int argc[] = {
  /* basic calls */
  0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1, 
  /* not implemented */
  2, 1,    1, 1, 2, 1, 1,
  /* extended, you may need to change the order of these two (plist, sleep) */
  0, 1
};

static void
syscall_handler (struct intr_frame *f) 
{
  int32_t* esp = (int32_t*)f->esp;
  // esp[0]  --> syscall number
  // esp[1]  --> param number 1
  switch ( esp[0] /* retrive syscall number */ )
  {
    case SYS_HALT:
    {
      power_off();
      break;
    }
    
    case SYS_EXIT:
    {
      if(!is_user_vaddr((char *) esp[1]))
        process_exit(-1);
      else
        process_exit((int) esp[1]);
      thread_exit();
      
      break;
    }
    
    case SYS_READ:
    {
      f->eax = -1;  // As default returns -1
      
      if (esp[1] == STDIN_FILENO) // Reading from standard input (keyboard)
      {
        int read_char_counter = 0;
        char input_char;
        for (int i = 0; i < esp[3]; i++)
        {
          input_char = input_getc();
          if (input_char != -1)
          {
            if (input_char == '\r')
              input_char = '\n';

            char *buffer = (char *)esp[2];
            buffer[i] = input_char;
            read_char_counter++;
            putchar((int)input_char);
          }
        }
        f->eax = read_char_counter;
      }
      else if(esp[1] > 1) // Reading from a file
      {
        if(!verify_fix_length((char *)esp[2], esp[3])){
          process_exit(-1);
          thread_exit();
        }

        if(!((int)esp[1] > FILE_TABLE_SIZE)){
          struct file *file_ptr= retrieve_file(esp[1]);

          if (file_ptr && (file_tell(file_ptr) + esp[3] <= file_length(file_ptr))){ 
            int result = file_read(file_ptr, (char *)esp[2], esp[3]);
            f->eax = result;
          }
        }
      }
      
      break;
    }
    
    case SYS_WRITE:
    {
      f->eax = -1; // As defult returns -1

      if(esp[1] == STDOUT_FILENO) // Writing to standard output (terminal)
      {
        putbuf((char *)esp[2], esp[3]);
        f->eax = esp[3];
      }
      else if(esp[1] > 1 && esp[1] < FILE_TABLE_SIZE) // Writing to a file
      {

        if(!verify_fix_length((char *)esp[2], esp[3])){
          process_exit(-1);
          thread_exit();
        }

        struct file *file_ptr= retrieve_file(esp[1]);
        if( file_ptr)
          f->eax = file_write(file_ptr, (char *)esp[2], esp[3]);
      }

      break;
    }

    case SYS_CREATE:
    {
      f->eax = false;
      if(!verify_fix_length((char *)esp[1], esp[2])){
        process_exit(-1);
        thread_exit();
      }

      if(!(char *) esp[1]){
        process_exit(-1);
        thread_exit();
        break;
      }
      
      if(filesys_create((char *)esp[1], esp[2]))
        f->eax = true;

      break;
    }

    case SYS_OPEN:
    {
      if(!(char *) esp[1]){
        f->eax = -1;
        break;
      }

      if(!verify_variable_length((char *) esp[1])){
        process_exit(-1);
        thread_exit();
      }

      struct file* file_ptr = filesys_open((char *)esp[1]);

      if(!file_ptr)
        f->eax = -1;
      else
      {
        struct thread* current_thread = thread_current();
        f->eax = file_table_insert(&(current_thread->file_table), file_ptr);

        if((int)f->eax == -1) // Close the file if it cannot add it to process table
          file_close(file_ptr);
      }

      break;
    }

    case SYS_CLOSE:
    {
      if(((int)esp[1] > FILE_TABLE_SIZE)){
         f->eax = -1;
         break;
      }
      struct thread* current_thread = thread_current();
      struct file* file_ptr = retrieve_file(esp[1]);

      if(file_ptr && esp[1] > 1)
      {
        file_table_remove(&(current_thread->file_table), esp[1]);
        file_close(file_ptr);
      }
      break;
    }

    case SYS_REMOVE:
    {
      f->eax = filesys_remove((char *)esp[1]);
      break;
    }

    case SYS_SEEK:
    {
      struct file* file_ptr = retrieve_file(esp[1]);
      
      if(file_ptr)
      {
        off_t length = file_length(file_ptr);
        if(esp[2] > length)
          file_seek(file_ptr, length);  
        else
          file_seek(file_ptr, esp[2]);
      }
      break;
    }

    case SYS_TELL:
    {
      f->eax = -1;
      struct thread* current_thread = thread_current();
      struct file* file_ptr = file_table_find(&(current_thread->file_table), esp[1]);
      
      if(file_ptr)
        f->eax = file_tell(file_ptr);
      
      break;
    }

    case SYS_FILESIZE:
    {
      f->eax = -1;
      struct file* file_ptr = retrieve_file(esp[1]);

      if(file_ptr)
        f->eax = file_length(file_ptr);

      break;
    }

    case SYS_EXEC:
    {
      if(!verify_variable_length((char *) esp[1])){
        process_exit(-1);
        thread_exit();
      }

      f->eax = process_execute( (char *) esp[1]);

      break;
    }

    case SYS_PLIST:
    {
      process_print_list();
      break;
    }

    case SYS_SLEEP:
    {
      timer_msleep(esp[1]);
      break;
    }

    case SYS_WAIT:
    {
      f -> eax = process_wait( (int) esp[1]);
      break;
    }
    
    default:
    {
      printf ("# Executed an unknown system call!\n");
      
      printf ("# Stack top + 0: %d\n", esp[0]);
      printf ("# Stack top + 1: %d\n", esp[1]);
      
      thread_exit ();
      
      break;
    }
  }
}

struct file* retrieve_file(int32_t fd)
{
  struct thread *current_thread = thread_current();
  return file_table_find(&(current_thread->file_table), fd);
}


bool verify_fix_length(void* start, unsigned length)
{
  void* page_start = pg_round_down(start);
  void* page_end = (void *) ((int)page_start + PGSIZE);
  void* end = (void*) ((unsigned) start + length );

  struct thread *cur = thread_current();
  uint32_t *pd = cur->pagedir;

  if(!is_user_vaddr(start))
    return false;

  if(!pagedir_get_page(pd, page_start))
    return false;

  while(page_end < end)
  {
    if(!pagedir_get_page(pd, page_end))
      return false;

    page_end = (void *) ((int)page_end + PGSIZE);
  }

  return true;
}

bool verify_variable_length(char* start)
{
  void* page_start = pg_round_down(start);
  unsigned current_pg_no = pg_no(start);
  struct thread *cur = thread_current();
  uint32_t *pd = cur->pagedir;
  
  if(!pagedir_get_page(pd, page_start))
    return false;

  if(!is_user_vaddr(start))
    return false;

  size_t i = 0;
  while(start[i] != '\0')
  {
    i++;
    if(pg_no(&start[i]) != current_pg_no){
      current_pg_no = pg_no(&start[i]);
      page_start = pg_round_down(&start[i]);

      if(!pagedir_get_page(pd, page_start))
        return false;
    }
  }  
  return true; 
}