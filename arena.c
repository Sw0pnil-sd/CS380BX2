#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARENA_SIZE 1024

typedef struct
{
    uint8_t memory[ARENA_SIZE];
    size_t offset;
} Arena;

// Initialize the arena
void arena_init(Arena *a)
{
    a->offset = 0;
}

// Your function (this is the important part)
void *arena_alloc(Arena *a, size_t size)
{
    if (a->offset + size > ARENA_SIZE)
    {
        return NULL;
    }

    void *ptr = &a->memory[a->offset];
    a->offset += size;

    return ptr;
}

int main()
{
    Arena my_arena;
    arena_init(&my_arena);

    int *num = (int *)arena_alloc(&my_arena, sizeof(int));
    if (num)
    {
        *num = 42;
        printf("Allocated number: %d\n", *num);
    }
    else
    {
        printf("Allocation failed!\n");
    }

    char *big_string = (char *)arena_alloc(&my_arena, 2048);
    if (!big_string)
    {
        printf("Successfully caught out-of-memory error!\n");
    }

    return 0;
}
