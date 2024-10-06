#ifdef _win32
#include <windows.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// Allocates/creates a new arena struct
Arena *arena_alloc(uint64_t cap) {
  Arena *arena = malloc(sizeof(Arena));
  arena->head = malloc(cap);
  arena->content_tail = arena->head;
  arena->size = cap;
  return arena;
}

// Releases/frees given areana struct
void arena_release(Arena *arena) {
  free(arena->head);
  free(arena);
}

// Appends a chunk of memory of the given size to the arena's tail.
void *arena_push_no_zero(Arena *arena, uint64_t size) {
  if ((arena->content_tail - arena->head) >= arena->size) {
    exit(1);
  }

  void *result_head = arena->content_tail;
  arena->content_tail += size;
  return result_head;
}

// Appends a chunk of memory of the given size to the arena's tail,
// and zeros out the memory is appended chunk.
void *arena_push(Arena *arena, uint64_t size) {
  void *result_head = arena_push_no_zero(arena, size);
  memset(result_head, 0, size);
  return result_head;
}

// Deletes a memory chunk of given size from the arena's tail.
void arena_pop(Arena *arena, uint64_t size) {
  if ((arena->content_tail - arena->head) >= size) {
    arena->content_tail -= size;
  }
}

// Clears the whole arena
void arena_clear(Arena *arena) { arena->content_tail = arena->head; }

// Don't know how to implement these or what they do at all.
void arena_set_auto_align(Arena *arena, uint64_t align);
uint64_t arena_pos(Arena *arena);
void *arena_push_aligner(Arena *arena, uint64_t alignment);
void arena_pop_to(Arena *arena, uint64_t pos);

////////////////////////////////
// Arena Implementation Testing

#define KB 1000
#define ARENA_SIZE KB
#define ALLOCATION_BYTE_SIZE 100
#define SLEEP_TIME_MS 500
#define MILISEC_TO_NANOSEC_MULT 1000000
#define MILISEC_TO_SECS_DIV 1000

void sleep_ms(uint8_t ms) {
#ifdef _win32
  Sleep(ms);
#else
  struct timespec ts;
  ts.tv_sec = ms / MILISEC_TO_SECS_DIV;
  ts.tv_nsec = ms * MILISEC_TO_NANOSEC_MULT;
  nanosleep(&ts, NULL);
#endif
}

void print_arena(Arena *arena) {
  uint64_t ammount_allocated = arena->content_tail - arena->head;

  printf("[");
  for (uint64_t arena_indx = 0; arena_indx < arena->size; arena_indx++) {
    if (arena_indx <= ammount_allocated) {
      printf("#");
    } else {
      printf(" ");
    }
  }
  printf("]\n ammount_allocated:%d\n", (int)ammount_allocated);
}

// TODO: Allocate 100 byes to the test arena and print it in a loop with a
// random number generated stop. Do a bound check before starting the loop to
// declare if the program should crash(due to no more space in the arena) or not
int main() {
  Arena *arena = arena_alloc(ARENA_SIZE);
  srand(time(NULL));
  uint8_t number_of_allocations =
      rand() % (int8_t)(2 * (ARENA_SIZE / ALLOCATION_BYTE_SIZE));
  bool will_out_of_range =
      number_of_allocations * ALLOCATION_BYTE_SIZE > ARENA_SIZE;

  printf("Number of allocations: %d\n", number_of_allocations);
  printf("Will the arena allocation fail due to out of range? %s\n",
         will_out_of_range ? "True" : "False");
  for (uint8_t indx = 0; indx < number_of_allocations; indx++) {
    arena_push(arena, ALLOCATION_BYTE_SIZE);
    print_arena(arena);
    sleep_ms((uint8_t)SLEEP_TIME_MS);
  }
  printf("SUCCESS!!! Got throught all the allocations made!\n");
}
