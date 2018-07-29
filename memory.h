#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#define GROW_CAPACITY(capacity)                          \
        ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(prev, type, old, upd)                 \
        (type *)reallocate(prev , sizeof(type) * (old),  \
                           sizeof(type) * (upd))

#define FREE_ARRAY(type, ptr, old)                       \
        reallocate(ptr, sizeof(type) * (old), 0)

#define MAX_HEAP_SIZE 4 * 1024 * 1024

/* Points memory pool which we use as Heap */
static void *memory_pool;

/* Points very first address of memory which isn't used yet in pool */
static char *brk_ptr;

/*
 * Metadata of block
 * Size of block = (2^sz) bytes
 * next points next freed block if any
 * block is returned when {m,re}alloc is called
 */
struct block_meta{
  size_t sz;
  struct block_meta *next;
  void *block;
};
#define BLOCK_META_SIZE (sizeof(struct block_meta))

/*
 * List of freed blocks
 * Block within n-th list has (2^n)byte size
 */
static struct block_meta **freed_block_list;

/*
 * old       | update    | operation
 * 0         | Non-zero  | Allocate new block
 * Non-zero  | 0         | Free allocation
 * Non-zero  | > old     | Shrink existing allocation
 * Non-zero  | < old     | Grow existing allocation
 */
void *reallocate(void *previous, size_t old, size_t update);

#endif /* CLOX_MEMORY_H */
