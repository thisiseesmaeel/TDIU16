#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int total_chars = 0;
    float total_args = 0;
    
    int i;
    for (i = 0; i < argc; i++) {
        int length = strlen(argv[i]);
        total_chars += length;
        total_args += 1;
        printf("%-20s %4d\n", argv[i], length);
    }

    printf("%-20s %4d\n", "Total length", total_chars);
    printf("%-20s %4.2f\n", "Average length", total_chars / total_args);

    return total_chars;
}
