#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"

#define MAX_STACK_SIZE 256

typedef struct {
  chunk_t *chunk;
  /* XXX It'll be fun setting ip as register variable */
  uint8_t *ip;
  /* XXX And stack as dynamic-sized also */
  val_t stack[MAX_STACK_SIZE];
  val_t *top;
} vm_t;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} interpret_result_t;

void init_VM();
void free_VM();
interpret_result_t interpret(const char *);
void push(val_t);
val_t pop();

#endif /* CLOX_VM_H */
