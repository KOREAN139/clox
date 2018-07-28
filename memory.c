#include <stdlib.h>

#include "common.h"
#include "memory.h"

void *reallocate(void *previous, size_t old, size_t update)
{
  if (!update) {
    free(previous);
    return NULL;
  }

  return realloc(previous, update);
}
