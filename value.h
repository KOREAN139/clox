#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef double val_t;

typedef struct{
  int count;
  int capacity;
  val_t *values;
} val_array_t;

void init_value_array(val_array_t *);
void write_value_array(val_array_t *, val_t);
void free_value_array(val_array_t *);
void print_value(val_t);

#endif /* CLOX_VALUE_H */
