#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pagedir.h"
#include "thread.h"
#define UNUSED(x) (void)(x)

/* verfy_*_lenght are intended to be used in a system call that accept
 * parameters containing suspisious (user mode) adresses. The
 * operating system (executng the system call in kernel mode) must not
 * be fooled into using (reading or writing) addresses not available
 * to the user mode process performing the system call.
 *
 * In pagedir.h you can find some supporting functions that will help
 * you dermining if a logic address can be translated into a physical
 * addrerss using the process pagetable. A single translation is
 * costly. Work out a way to perform as few translations as
 * possible.
 *
 * Recommended compilation command:
 *
 *  gcc -Wall -Wextra -std=gnu99 -pedantic -m32 -g pagedir.o verify_adr.c
 */
struct thread* thread;
/* Verify all addresses from and including 'start' up to but excluding
 * (start+length). */
bool verify_fix_length(void* start, unsigned length)
{
  //ADD YOUR CODE HERE
  bool result = false;
  void* begin = pg_round_down(start);
  void* end = (void*) ((unsigned) start + length );


    if(pagedir_get_page(thread->pagedir, begin) == NULL)
    {
      printf("False in start\n");
      return result;
    }
    while((begin + PGSIZE ) < end)
    {
      if(pagedir_get_page(thread->pagedir, begin + PGSIZE) == NULL)
      {
        printf("False in end\n");
        return result;
      }
      begin = begin + PGSIZE;
    }

  result = true;
  printf("True\n");
  return result;
}

/* Verify all addresses from and including 'start' up to and including
 * the address first containg a null-character ('\0'). (The way
 * C-strings are stored.)
 */

bool verify_variable_length(char* start)
{
   // ADD YOUR CODE HERE
  char* test = "";
  int counter = 0;
  //unsigned int length = strlen(start);

  while (is_end_of_string(start))
  {
    printf("Something....\n");
  }
  
  return false; 
}

/* Definition of test cases. */
struct test_case_t
{
  void* start;
  unsigned length;
};

#define TEST_CASE_COUNT 6

const struct test_case_t test_case[TEST_CASE_COUNT] =
{
  {(void*)100, 100}, /* one full page */
  {(void*)199, 102},
  {(void*)101, 98},
  {(void*)250, 190},
  {(void*)250, 200},
  {(void*)250, 210}
};

/* This main program will evalutate your solution. */
int main(int argc, char* argv[])
{
  int i;
  bool result;

  if ( argc == 2 )
  {
    simulator_set_pagefault_time( /* atoi(argv[1]) */ 0 );
  }
  thread_init();

  thread = thread_current();

  /* Test the algorithm with a given intervall (a buffer). */
  for (i = 0; i < TEST_CASE_COUNT; ++i)
  {
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_fix_length(test_case[i].start, test_case[i].length);
    evaluate(result);
    end_evaluate_algorithm();
  } 

  /* Test the algorithm with a C-string (start address with
   * terminating null-character).
   */
 /*  for (i = 0; i < TEST_CASE_COUNT; ++i)
  {
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_variable_length(test_case[i].start);
    evaluate(result);
    end_evaluate_algorithm();
  }  */
  return 0;
}
