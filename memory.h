#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#define GROW_CAPACITY(capacity)                          \
        ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(prev, type, old, upd)                 \
        (type *)reallocate(prev , sizeof(type) * (old),  \
                           sizeof(type) * (upd))

#define FREE_ARRAY(type, ptr, old)                       \
        reallocate(ptr, sizeof(type) * (old), 0)

/*
 * old       | update    | operation
 * 0         | Non-zero  | Allocate new block
 * Non-zero  | 0         | Free allocation
 * Non-zero  | > old     | Shrink existing allocation
 * Non-zero  | < old     | Grow existing allocation
 */
void *reallocate(void *previous, size_t old, size_t update);

#endif /* CLOX_MEMORY_H */
