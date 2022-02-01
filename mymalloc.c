#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "mymalloc.h"
#include <limits.h>
#include <string.h>
#include <dlfcn.h>

#define TOTAL 1000000000

/*
Have a linked list of structs, that point to start have a size and have an 
occupied flag
 */

struct node
{
    bool occupied;
    int size;
    struct node *next;
    struct node* prev;
    int malloced_space;
};

static char stuff[TOTAL];
static struct node *head;
static struct node *end;


// char *start = stuff + sizeof(struct node);
static struct node *current;
static void *end_array = stuff + TOTAL;
static bool initialized = false;

// initializes the linked list of structs, adding one header struct
// at the beginning of the char array, and another just after.
// The second points to null, is not occupied and is 
// ready to be filled up (daddy?)
static void initialize(void)
{
    head = (void*)stuff;
    struct node* temp = ((void *)head) + sizeof(struct node);

    head->size = 0;
    head->occupied = true;
    head->prev = NULL;
    head->next = temp;
    current = head;

    temp->size = 0;
    temp->occupied = false;
    temp->prev = head;
    temp->next = NULL;

    end = temp;
    initialized = true;
}

// allocates memory to the middle 
static void * add_to_middle(size_t size)
{
    struct node * temp, *store;

    temp = head;
    store = NULL;

    while (temp->next != NULL)
    {
        temp = temp->next;
        int min_size = INT_MAX;
        if (size < temp->size && temp->occupied == false && temp->size < min_size)
        {
            temp->size = min_size;
            store = temp;
            return store;
        }
    }

    if (store != NULL)
    {
        store->occupied = true;
        store->malloced_space = size;
        fprintf(stderr, "Added to middle at: %p\n\n", (void *)store);
    }


    return store;
}

// allocates memory to the end 
static void add_to_end(size_t size)
{
    struct node *temp, *temp2;
    temp = end;

    temp->next = ((void *)temp) + sizeof(struct node) + size;
    temp2 = temp->next;

    temp->size = size;
    temp->malloced_space = size;
    temp->occupied = true;

    temp2->size = 0;
    temp2->malloced_space = 0;
    temp2->occupied = false;
    temp2->prev = temp;
    temp2->next = NULL;

    end = temp2;

    fprintf(stderr, "Address of added struct is: %p\n", temp2);
}

void * malloc(size_t size)
{
    fprintf(stderr, "Size: %i\n", size);
    
    if (initialized == false)
    {
        fprintf(stderr, "initializing\n");
        initialize();
        fprintf(stderr, "Head address is: %p\n\n", head);
    }

    struct node* store = add_to_middle(size);

    if (store != NULL)
    {
        return (void*) store + sizeof(struct node);
    }

    fprintf(stderr, "Setting current pointer\n");
    current = end;
    fprintf(stderr, "Current pointer set at %p. \n\n", current);


    if (size < (end_array - (void*)current))
    {
        fprintf(stderr, "Adding new struct\n");
        add_to_end(size);
        fprintf(stderr, "\nSetting current pointer again\n");
        current = end;
        fprintf(stderr, "Structure that is full is at: %p\n", (void *)current - size - sizeof(struct node));
        fprintf(stderr, "Current pointer set at %p. \n\n", current);
        return (((void *)current) - size); // - sizeof(struct node));
    }

    else
    {
        return NULL;
    }

}

// sets the pointer that you are freeing to unnocupied, so that it
// can be used again when adding back to the middle
void free(void * ptr)
{
    
    if (ptr == NULL)
    {
        return;
    }

    struct node* ptr2 = ptr - sizeof(struct node);
    fprintf(stderr, "\n\nFreeing: %p\n\n", ptr2);

    ptr2->occupied = false;

    if ((ptr2->prev)->occupied == false)
    {
        /* link them */
    }

    else if ((ptr2->next)->occupied == false)
    {
        /* same again */
    }
}

void * calloc(size_t size, size_t nmemb)
{
    if (!size || !nmemb)
    {
        return NULL;
    }
    
    void *ptr = malloc(size * nmemb);
    fprintf(stderr, "setting memory to 0");
    ptr = memset(ptr, 0, size*nmemb);
    fprintf(stderr, "memory set to zero");
    return ptr;
}

void * realloc(void *ptr, size_t size)
{
    void *ptr1;
    if (ptr == NULL)
    {
        ptr1 = malloc(size);
        return ptr1;
    }
    else if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    else
    {
        ptr1 = malloc(size);
        struct node *ptr2 = ptr - sizeof(struct node);
        memcpy(ptr1, ptr, ptr2->malloced_space);
        free(ptr);
        fprintf(stderr, "\nfreed using realloc\n");
        fprintf(stderr, "\nptr1 malloced succesfully\n");
        return ptr1;    
    }

}

