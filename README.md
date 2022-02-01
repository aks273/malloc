# Custom Memory Allocator
Implementation of a memory allocator library, as described in K&R, page 185.

We create a large byte array of size `TOTAL` on the stack, which is where memory will be allocated. Within this array we have a linked list of once alloced "chunks", and other chunks of memory that have not been allocated to yet. When calling malloc, we scan the linked list for a free chunk which is large enough to be reused. If no such chunk exists, we create a new one of the size to be allocated at the end of the list.

This library has to be preloaded into a command, using `LD_PRELOAD`.
We print/log to stderr, to avoid recursive calls to the system malloc.

The following functions
- `calloc` (`malloc` but filling the alloc'ed memory with 0's)
- `realloc` (changing the size of an already alloce'ed chunk of memory) 
- `free` 

have also been implemented.
