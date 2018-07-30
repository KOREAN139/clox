#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

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

  /* (16 * sizeof(struct block_meta *))bytes for freed_block_list */
  freed_block_list = (struct block_meta **)memory_pool;
  brk_ptr = (char *)memory_pool + 16 * sizeof(struct block_meta *);
  end_pool = (char *)memory_pool + MAX_HEAP_SIZE;
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

void __free(void *block)
{
  int index = -1;
  struct block_meta *bm = GET_BLOCK_META(block);

  bm->next = NULL;

  /* Index will be floor(log2(sz)) */
  for (int sz = bm->sz; sz; index++) {
    sz >>= 1;
  }

  /* If bin is not empty, link this block to head of this bin*/
  if (freed_block_list[index]) {
    bm->next = freed_block_list[index];
  }

  /* This block is the head of bin now */
  freed_block_list[index] = bm;
}

void *__malloc(size_t size)
{
  int block_size;
  int index = -1;
  struct block_meta *bm = NULL;
  struct block_meta *prev = NULL;

  /* Index will be floor(log2(size)) */
  for (block_size = size; block_size; index++) {
    block_size >>= 1;
  }

  /* Find proper block in bin */
  for (bm = freed_block_list[index]; bm && bm->sz < size; bm = bm->next) {
    prev = bm;
  }

  if (bm) {
    /* Pop out proper block */
    prev->next = bm->next;
    if (bm == freed_block_list[index]) {
      freed_block_list[index] = NULL;
    }
    bm->next = NULL;
  } else {
    /* Check whether we can make new block from pool */
    if ((char *)end_pool < brk_ptr + BLOCK_META_SIZE + size) {
      puts("External fragmentation occurs");
      /* Terminate process for now, need to implement coalescing */
      exit(EXIT_FAILURE);
    }
    /* Make new block from pool */
    bm = (struct block_meta *)brk_ptr;
    bm->sz = size;
    bm->next = NULL;

    brk_ptr = brk_ptr + BLOCK_META_SIZE + size;
  }

  return bm + 1;
}

/*
 * If old is 0, return NULL
 * Otherwise,   return proper block
 */
void *__realloc(void *block, size_t old, size_t update)
{
  if (!old) {
    return __malloc(update);
  }

  if (!update) {
    __free(block);
  }

  struct block_meta *bm = GET_BLOCK_META(block);
  void *new_block = (void *)0;

  /*
   * Get new block if update size is bigger than current block's size
   * If current block's size can cover up update size, NOP
   */
  if (bm->sz < update) {
    new_block = __malloc(update);
    memcpy(new_block, block, old);
    __free(block);
  } else {
    new_block = block;
  }

  return new_block;
}
