#include <stdint.h>
#include <assert.h>

typedef struct {
    void * buff;
    size_t buff_size;
    size_t curr_offset;
    size_t prev_offset;
} Arena;

Arena new_arena(void *backing_buff, size_t backing_buff_size) {
    Arena a = {0};
    a.buff = backing_buff;
    a.buff_size = backing_buff_size;
    return a;
}

bool is_power_of_two(size_t val) {
    return (val & (val-1)) == 0;
}

size_t align_forward(size_t addr, size_t align) {
    assert(is_power_of_two(align));
    size_t mod = addr % align;
    if (mod != 0) {
        addr += (align - mod);
    }
    return addr;
}

void *arena_alloc_align(Arena *a, size_t size, size_t align) {
    size_t aligned_offset = align_forward((size_t)a->buff + a->curr_offset, align);
    aligned_offset -= (size_t)a->buff;
    
    if (aligned_offset + size <= a->buff_size) {
        void *ptr = (uint8_t *)a->buff + aligned_offset;
        a->prev_offset = aligned_offset;
        a->curr_offset = aligned_offset + size;
        return ptr;
    }

    return NULL;
}