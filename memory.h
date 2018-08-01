#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"

#define GROW_CAPACITY(capacity)                          \
        ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(prev, type, old, upd)                 \
        (type *)__realloc(prev , sizeof(type) * (old),   \
                           sizeof(type) * (upd))

#define FREE_ARRAY(type, ptr, old)                       \
        __realloc(ptr, sizeof(type) * (old), 0)

#define MAX_HEAP_SIZE 4 * 1024 * 1024

#define BIN_LIST_SIZE 16

/*
 * Metadata of block
 * Size of block = sz bytes
 * next points next freed block if any
 * block is returned when {m,re}alloc is called
 */
struct block_meta{
  uint32_t sz;
  struct block_meta *next;
};
#define BLOCK_META_SIZE (sizeof(struct block_meta))

#define GET_BLOCK_META(block) \
        ((struct block_meta *)(block) - 1)

/* Put block in proper bin(in **freed_block_list) */
void __free(void *);

/*
 * If any freed block exists, return that block
 * else, make proper block and return that block
 */
void *__malloc(size_t);

/*
 * old       | update    | operation
 * 0         | Non-zero  | Allocate new block
 * Non-zero  | 0         | Free allocation
 * Non-zero  | > old     | Shrink existing allocation
 * Non-zero  | < old     | Grow existing allocation
 */
void *__realloc(void *, size_t, size_t);

#endif /* CLOX_MEMORY_H */
