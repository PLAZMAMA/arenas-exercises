Arena *arena_alloc(U64 cap);
void arena_release(Arena *arena);
void arena_set_auto_align(arena *arena, U64 align);
U64 arena_pos(Arena *arena);
void *arena_push_no_zero(Arena *arena, U64 size);
void *arena_push_aligner(Arena *arena, U64 alignment);
void *arena_push(Arena *arena, U64 size);
void arena_pop_to(Arena *arena, U64 pos);
void arena_pop(Arena *arena, U64 size);
void arena_clear(Arena *arena);

int main() {
}
