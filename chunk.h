#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

/* Operation codes */
typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} opcode_t;

/*
 * Dynamic array implementation
 * Capacity grows exponential when it's full
 */
typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  int *lines;
  val_array_t constants;
} chunk_t;

void init_chunk(chunk_t *);
void free_chunk(chunk_t *);
void write_chunk(chunk_t *, uint8_t, int);
int add_constant(chunk_t *, val_t);

#endif /* CLOX_CHUNK_H */
