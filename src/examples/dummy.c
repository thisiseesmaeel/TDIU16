/* klaar@ida

   A small dummy process that just uses up a process slot in the long
   runtime test. Just return it's first argument.

   Normally used by some paren process.
 */

#include <stdlib.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 0;
  
  printf("\n\nDUMMY IS RUNNING\n\n");
  return atoi(argv[1]);
}
