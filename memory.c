#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "common.h"
#include "memory.h"

/* Request 4MB memory pool from OS, and let memory_pool point the pool */
static void __attribute__((constructor)) init_heap()
{
  memory_pool = mmap(NULL, MAX_HEAP_SIZE, (PROT_READ | PROT_WRITE),
                     (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);

  if (memory_pool == (void *) -1) {
    puts("FAILURE ON MMAP()");
    exit(EXIT_FAILURE);
  }

  /* (16 * sizeof(struct block_meta *))bytes is for freed_block_list */
  freed_block_list = (struct block_meta **)memory_pool;
  brk_ptr = (char *)memory_pool + 16 * sizeof(struct block_meta *);
}

/* Give back borrowed 4MB memory pool to OS */
static void __attribute__((destructor)) destroy_heap()
{
  /*
   * Once mmap() successfully give us memory pool,
   * the pool size and address of pool are never modified through program
   * Thus munmap() never fails if this program ends normally
   */
  munmap(memory_pool, MAX_HEAP_SIZE);
}

/* Do implement */
void __free(void *block);

void *__malloc(size_t size);

void *__realloc(void *block, size_t old, size_t update);

void *reallocate(void *previous, size_t old, size_t update)
{
  if (!update) {
    free(previous);
    return NULL;
  }

  return realloc(previous, update);
}
