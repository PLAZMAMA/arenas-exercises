#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint64_t *arena_head;
    uint64_t *content_tail;
    uint64_t size;
} Arena;

Arena *arena_alloc(uint64_t cap){
    Arena *arena = malloc(sizeof(Arena));
    arena->arena_head = malloc(cap);
    arena->content_tail = arena->arena_head;
    arena->size = cap;
    return arena;
}

void arena_release(Arena *arena) {
    free(arena->arena_head);
    free(arena);
}

void *arena_push(Arena *arena, uint64_t size) {
    void *result_head = arena->content_tail;
    arena->content_tail += size;
    return result_head;
}

void arena_pop(Arena *arena, uint64_t size);
void arena_clear(Arena *arena);

void arena_set_auto_align(Arena *arena, uint64_t align);
unsigned int arena_pos(Arena *arena);
void *arena_push_no_zero(Arena *arena, uint64_t size);
void *arena_push_aligner(Arena *arena, uint64_t alignment);
void arena_pop_to(Arena *arena, uint64_t pos);

int main() {
}
