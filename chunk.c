#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_line_array(line_array_t *array)
{
  array->count = 0;
  array->capacity = 0;
  array->data = NULL;
}

void init_chunk(chunk_t *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  init_line_array(&chunk->lines);
  init_value_array(&chunk->constants);
}

void free_line_array(line_array_t *array)
{
  FREE_ARRAY(line_t, array->data, array->capacity);
  init_line_array(array);
}

void free_chunk(chunk_t *chunk)
{
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  free_line_array(&chunk->lines);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

void write_chunk(chunk_t *chunk, uint8_t byte, int line)
{
  line_array_t *lines = &chunk->lines;

  if (chunk->capacity == chunk->count) {
    int old = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old);
    chunk->code = GROW_ARRAY(chunk->code, uint8_t,
                             old, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;

  /* If work on same line, no need to store line info */
  if (lines->count && lines->data[lines->count - 1].line == line) {
    return;
  }

  if (!lines->count || lines->count == lines->capacity) {
    int old = lines->capacity;
    lines->capacity = GROW_CAPACITY(old);
    lines->data = GROW_ARRAY(lines->data, line_t,
                             old, lines->capacity);
  }

  lines->data[lines->count].line = line;
  /* Since offset increase by 1 */
  lines->data[lines->count].offset = chunk->count - 1;
  lines->count++;
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
