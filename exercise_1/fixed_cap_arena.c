#include <stdlib.h>
#include <stdint.h>

////////////////////////
// Arena Implementation

typedef struct {
    uint64_t *head;
    uint64_t *content_tail;
    uint64_t size;
} Arena;

Arena *arena_alloc(uint64_t cap){
    Arena *arena = malloc(sizeof(Arena));
    arena->head = malloc(cap);
    arena->content_tail = arena->head;
    arena->size = cap;
    return arena;
}

void arena_release(Arena *arena) {
    free(arena->head);
    free(arena);
}

void *arena_push(Arena *arena, uint64_t size) {
    if (*arena->content_tail + size >= arena->size) {
        exit(1);
    }

    void *result_head = arena->content_tail;
    arena->content_tail += size;
    return result_head;
}

void arena_pop(Arena *arena, uint64_t size) {
    if (arena->content_tail - size > 0) {
        arena->content_tail -= size;
    }
}

void arena_clear(Arena *arena) {
    arena->content_tail = arena->head;
}

// Don't know how to implement these or what they do at all.
void arena_set_auto_align(Arena *arena, uint64_t align);
unsigned int arena_pos(Arena *arena);
void *arena_push_no_zero(Arena *arena, uint64_t size);
void *arena_push_aligner(Arena *arena, uint64_t alignment);
void arena_pop_to(Arena *arena, uint64_t pos);

////////////////////////////////
// Arena Implementation Testing

int main() {
}
