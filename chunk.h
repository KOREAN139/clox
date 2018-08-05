#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

/* Operation codes */
typedef enum {
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
  OP_RETURN,
} opcode_t;

/*
 * Assume that line and offset is strictly increasing 
 * (If this interpreter walks script more than once, need to
 *  implement in other way)
 * line = line number
 * offset = first offset in line
 */
typedef struct {
  int line;
  int offset;
} line_t;

typedef struct {
  int count;
  int capacity;
  line_t *data;
} line_array_t;

/*
 * Dynamic array implementation
 * Capacity grows exponential when it's full
 */
typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  line_array_t lines;
  val_array_t constants;
} chunk_t;

void init_line_array(line_array_t *);
void init_chunk(chunk_t *);
void free_line_array(line_array_t *);
void free_chunk(chunk_t *);
void write_chunk(chunk_t *, uint8_t, int);
int add_constant(chunk_t *, val_t);

/*
 * Multibyte operand implementation
 * use OP_CONSTANT,      if index is in uint8_t value range
 * use OP_CONSTANT_LONG, otherwise
 */
void write_constant(chunk_t *, val_t, int);

#endif /* CLOX_CHUNK_H */
