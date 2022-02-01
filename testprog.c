#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"


int main(void)
{
    double *num0 = malloc(sizeof(double));
    fprintf(stderr, "address of num0 is: %p\n\n", num0);

    num0 = realloc(num0, 0);

    num0 = realloc(num0, 2 * sizeof(double));
    fprintf(stderr, "address of num0 is: %p\n\n", num0);

    free(num0);

    int *num1 = malloc(sizeof(int));
    fprintf(stderr, "address of num1 is: %p\n\n", num1);

    free(num1);
}
