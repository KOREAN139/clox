#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

/* Operation codes */
typedef enum {
  OP_RETURN,
} OpCode;

/*
 * Dynamic array implementation
 * Capacity grows exponential when it's full
 */
typedef struct {
  int count;
  int capacity;
  uint8_t *code;
} Chunk;

void init_chunk(Chunk *);
void free_chunk(Chunk *);
void write_chunk(Chunk *, uint8_t);

#endif /* CLOX_CHUNK_H */
