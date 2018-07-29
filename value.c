#include <stdio.h>

#include "memory.h"
#include "value.h"

void init_value_array(val_array_t *array)
{
  array->count = 0;
  array->capacity = 0;
  array->values = NULL;
}

void write_value_array(val_array_t *array, val_t value)
{
  if (array->capacity == array->count) {
    int old = array->capacity;
    array->capacity = GROW_CAPACITY(old);
    array->values = GROW_ARRAY(array->values, val_t,
                               old, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void free_value_array(val_array_t *array)
{
  FREE_ARRAY(val_t, array->values, array->capacity);
  init_value_array(array);
}

void print_value(val_t value)
{
  printf("%g", value);
}
