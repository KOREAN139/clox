#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_chunk(chunk_t *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
}

void free_chunk(chunk_t *chunk)
{
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

void write_chunk(chunk_t *chunk, uint8_t byte, int line)
{
  if (chunk->capacity == chunk->count) {
    int old = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old);
    chunk->code = GROW_ARRAY(chunk->code, uint8_t,
                             old, chunk->capacity);
    chunk->lines = GROW_ARRAY(chunk->lines, int,
                              old, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

int add_constant(chunk_t *chunk, val_t value)
{
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}

void write_constant(chunk_t *chunk, val_t value, int line)
{
  int index = add_constant(chunk, value);

  if (index < 256) {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, index, line);
  } else {
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    /* Split index to 3-parts, and push to code chunks */
    write_chunk(chunk, index & 0xff, line);
    write_chunk(chunk, (index >> 8) & 0xff, line);
    write_chunk(chunk, (index >> 16) & 0xff, line);
  }
}
