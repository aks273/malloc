#include <stddef.h>
#ifndef MALLOC_H
#define MALLOC_H

void * malloc(size_t size);
void * realloc(void *ptr, size_t size);
void free(void * ptr);
struct node ;
void add_to_end(size_t size);


#endif
