#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

////////////////
// Arena Macros
#define push_array(arena, type, count)                                         \
  (type *)arena_push(arena, sizeof(type) * count)

////////////////////////
// Arena Implementation

typedef struct {
  uint64_t *head;
  uint64_t *content_tail;
  uint64_t size;
} Arena;

Arena *arena_alloc(uint64_t cap) {
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

void *arena_push_no_zero(Arena *arena, uint64_t size) {
  if (*arena->content_tail + size >= arena->size) {
    exit(1);
  }

  void *result_head = arena->content_tail;
  arena->content_tail += size;
  return result_head;
}

void *arena_push(Arena *arena, uint64_t size) {
  void *result_head = arena_push_no_zero(arena, size);
  memset(result_head, 0, size);
  return result_head;
}

void arena_pop(Arena *arena, uint64_t size) {
  if (arena->content_tail - size > arena->head) {
    arena->content_tail -= size;
  }
}

void arena_clear(Arena *arena) { arena->content_tail = arena->head; }

// Don't know how to implement these or what they do at all.
void arena_set_auto_align(Arena *arena, uint64_t align);
uint64_t arena_pos(Arena *arena);
void *arena_push_aligner(Arena *arena, uint64_t alignment);
void arena_pop_to(Arena *arena, uint64_t pos);

////////////////////////////////
// Arena Implementation Testing

#define KB 1000
#define ARENA_OCCUPATION_BAR_SIZE 50

// TODO: improve granularity for print_arena to print each byte in the arena provided. (potential steps below)
//  1. create arena to house printed arena str
//  2. push to arena the printed arena str
//  3. and thats it I think...
void print_arena(Arena *arena) {
  float percent_occupied_memory_size = (arena->content_tail - arena->head) / (float) arena->size;
  uint8_t occupied_bar_indx = 0;
  char arena_occupation_bar[ARENA_OCCUPATION_BAR_SIZE + 1] = {0};
  uint8_t occupied_bar_section =  (int) (ARENA_OCCUPATION_BAR_SIZE * percent_occupied_memory_size);

  for (; occupied_bar_indx < occupied_bar_section; occupied_bar_indx++) {
    arena_occupation_bar[occupied_bar_indx] = '#';
  }

  for (; occupied_bar_indx < ARENA_OCCUPATION_BAR_SIZE; occupied_bar_indx++) {
    arena_occupation_bar[occupied_bar_indx] = ' ';
  }

  printf("[%s]\n", arena_occupation_bar);
}

// TODO: Allocate 100 byes to the test arena and print it in a loop with a random number generated stop.
// Do a bound check before starting the loop to declare if the program should crash(due to no more space in the arena) or not
int main() {
  Arena *arena = arena_alloc(2 * KB);
  arena_push(arena, 1000);
  print_arena(arena);
}
