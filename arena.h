#ifndef ARENA
#define ARENA

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef KILOBYTE
#define KILOBYTE 1024

#ifndef DEFAULT_ARENA_SIZE
#define DEFAULT_ARENA_SIZE KILOBYTE
#endif

#else // KILOBYTE
#ifndef DEFAULT_ARENA_SIZE
#define DEFAULT_ARENA_SIZE 1024
#endif
#endif// KILOBYTE

#define A_BYTE
#ifdef A_BYTE
#define byte uint8_t
#endif

struct Arena {

    /*Data is originally allocated and used as u_int8_t*/
    void *data;
    size_t capacity;
    size_t head;
    struct Arena *next;
};

typedef struct Arena Arena;

Arena *arena_new(size_t size);
void *arena_alloc(Arena *arena, size_t size);
void arena_free(Arena *arena);

#endif // ARENA

#ifdef ARENA_IMPLEMENTATION

#define ASSERT_NO_MEM(ptr) assert(ptr != NULL && "OUT OF MEMORRY")

struct Arena *arena_new(size_t size) {
    if (size <= 0)
        size = DEFAULT_ARENA_SIZE;

    struct Arena *n_arena = malloc(sizeof(struct Arena));
    ASSERT_NO_MEM(n_arena);

    byte *data = malloc(sizeof(byte) * size);
    ASSERT_NO_MEM(data);

    *n_arena = (struct Arena){
        .data = (void *)data,
        .capacity = size,
        .head = 0,
        .next = NULL,
    };

    return n_arena;
}

void *arena_realloc(struct Arena *arena, void *old_p, size_t old_size,
                    size_t new_size) {

    if (new_size <= old_size)
        return old_p;

    void *new_p = arena_alloc(arena, new_size);
    memcpy(new_p, old_p, old_size);

    return new_p;
}

void *arena_alloc(struct Arena *arena, size_t size) {
    assert(size > 0);
    struct Arena *current = arena;
    while (current->head + size > current->capacity) {
        if (current->next == NULL) {
            current->next = arena_new(current->head + size);
        }
        current = current->next;
    }

    void *alloc_p = (void *)((byte *)current->data + current->head);

    current->head += size;
    return alloc_p;
}

void arena_free(struct Arena *arena) {

    struct Arena *cur = arena;

    while (cur != NULL) {
        Arena *tmp = cur->next;
        free(cur->data);
        free(cur);
        cur = tmp;
    }
}

#endif // ARENA_IMPLEMENTATION

#undef A_BYTE
